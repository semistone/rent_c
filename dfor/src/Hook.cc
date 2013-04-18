#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>
#include <sys/socket.h>
#include <netdb.h>
#include "DB.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include "log4cpp/PropertyConfigurator.hh"

static struct hostent *((*orig_gethostbyname)(const char *name)) = NULL;
static int (*orig_getaddrinfo)(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res) = NULL;
static dfor::DB *db = NULL;
static log4cpp::Category* logger = &log4cpp::Category::getInstance(std::string("dfor"));

int 
init_dfor(){//{{{
    if (orig_getaddrinfo == NULL)
    {
         orig_getaddrinfo = (int (*)(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res))
                             dlsym(RTLD_NEXT, "getaddrinfo");
    }
    if (orig_gethostbyname == NULL)
    {
         orig_gethostbyname = (struct hostent *((*)(const char *name)))dlsym(RTLD_NEXT, "gethostbyname");
    }
    logger->debugStream()<<"init logger";
    log4cpp::PropertyConfigurator::configure("/usr/local/etc/dfor/log4cpp.properties");

    logger->debugStream()<<"check cache.db file exist";
    std::string dbfile = "/var/run/dfor/cache.db";
    FILE *fp = fopen(dbfile.c_str(), "r");
    if(fp){
        fclose(fp);
    } else {
        return -1; // if not exist.
    }

    logger->debugStream()<<"init sqlite db";
    db = new dfor::DB(dbfile);
    return 0;
}//}}}

/**
 * for debug
 */
void dump_addr(addrinfo* res, const char* service){//{{{
    if (service != NULL) {
        printf("service is %s\n", service);
    } else {
        printf("service is null\n");
    }
    if (res == NULL) {
        printf("addr is null\n");
        return;
    }
    void *ptr;
    char addrstr[100];
    struct sockaddr_in *sock = (struct sockaddr_in *)res->ai_addr;
    if (sock != NULL) {
        printf("family is (%d) port (%d) zero(%s)\n", sock->sin_family, sock->sin_port, sock->sin_zero);
    } else {
        printf("sock is null\n");
    }
    ptr = &sock->sin_addr;
    inet_ntop (res->ai_family, ptr, addrstr, 100);
    printf ("IPv%d address: %s (%s) flag (%d) socket type (%d) protocol (%d) len (%d)\n", 
            res->ai_family == PF_INET6 ? 6 : 4,
            addrstr,
            res->ai_canonname,
            res->ai_flags,
            res->ai_socktype,
            res->ai_protocol,
            res->ai_addrlen);
}//}}}

struct hostent *gethostbyname(const char *name){//{{{
    // init 
    if (db == NULL){
        if(init_dfor() != 0)
            return (*orig_gethostbyname)(name);
    }
    logger->debugStream()<<" call gethostbyname";
    std::string hostname = std::string(name);
    std::string ip = db->query(hostname);
    struct hostent *host;
    struct in_addr *addr;
    if (ip == "") {
        logger->debugStream()<<"ip is empty";
        return (*orig_gethostbyname)(name);
    } else {
        logger->debugStream()<<"ip is not empty";
        host = (hostent*) malloc(sizeof(hostent));
        addr = (in_addr*) malloc(sizeof(in_addr));
        host->h_name = strdup(name);
        host->h_length = sizeof(struct in_addr);
        host->h_addrtype = AF_INET;
        inet_aton(ip.c_str(), addr);
        host->h_aliases = NULL;
        host->h_addr_list = (char**)malloc(2 * sizeof(char *));
        host->h_addr_list[0] = (char*)addr;
        host->h_addr_list[1] = NULL;
        return host;
    }
}//}}}

int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res){//{{{
    // init 
    //dump_addr(*res, service);
    //dump_addr((struct addrinfo*)hints, service);
    if (db == NULL){
        if(init_dfor() != 0) return -1;
    }
    logger->debugStream()<<" call getaddrinfo node:"<<node;
    if (node == NULL) {
        logger->debugStream()<<" call getaddrinfo node is empty";
        return (*orig_getaddrinfo)(node,service,hints,res);
    }
    std::string hostname = std::string(node);
    std::string ip = db->query(hostname);
    if (ip == "") {
        logger->debugStream()<<"ip is empty";
        return (*orig_getaddrinfo)(node,service,hints,res);
    } else {
        logger->debugStream()<<"ip is not empty";
        struct sockaddr_in *sock;
        struct addrinfo  *addr;
        addr = (addrinfo*) malloc(sizeof(addrinfo));
        sock = (sockaddr_in*) malloc(sizeof(sockaddr_in));
        memset(addr, 0, sizeof(struct addrinfo));
        memset(sock, 0, sizeof(struct sockaddr_in));
        addr->ai_family = AF_INET;
        addr->ai_flags = hints->ai_flags;
        addr->ai_socktype = 1;
        addr->ai_addr = (sockaddr*)sock;
        addr->ai_addrlen = sizeof(sockaddr_in);
        addr->ai_protocol = 6;
        addr->ai_canonname =  strdup(node);
        addr->ai_next = NULL;
        sock->sin_family = AF_INET;
        if (service != NULL) {
            sock->sin_port = htons(atoi(service));
        }
        sock->sin_addr.s_addr = inet_addr(ip.c_str());
        *res = addr;
        db->updateCount(hostname, ip);
        //dump_addr(*res, service);
        return 0;
    }

}//}}}
