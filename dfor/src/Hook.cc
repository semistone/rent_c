#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "log4cpp/PropertyConfigurator.hh"
#include "log4cpp/Category.hh"

static struct hostent *((*orig_gethostbyname)(const char *name)) = NULL;
static int (*orig_getaddrinfo)(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res) = NULL;
static log4cpp::Category* logger = &log4cpp::Category::getInstance(std::string("dfor"));
static bool is_init = false;
int 
init_dfor(){//{{{
    is_init = true;
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

    std::string dbfile = "/var/run/dfor/cache.db";
    FILE *fp = fopen(dbfile.c_str(), "r");
    if(fp){
        fclose(fp);
    } else {
        is_init = false;
        logger->debugStream()<<"check cache.db file but not exist";
        return -1; // if not exist.
    }

    return 0;
}//}}}

int query(std::string& hostname, std::string& ip){//{{{
    int sock;
    struct sockaddr_un server;
    char buf[1024];
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        logger->debugStream()<<"opening stream socket";
        return -1;
    }
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, "/var/run/dfor/dfor.sock");
    if (connect(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un)) < 0) {
        close(sock);
        logger->debugStream()<<"connecting stream socket but fail";
        return -1;
    }
    if (write(sock, hostname.append("\n").c_str(), hostname.length() + 1) < 0){
        logger->debugStream()<<"write data into socket fail";
        close(sock);
        return -1;
    }
    if (read(sock, buf, sizeof(buf)) < 0){
        close(sock);
        logger->debugStream()<<"query "<< hostname<< " return empty";
        return -1;
    } else {
        close(sock);
        ip = std::string(buf);
        int idx = ip.find("\n");
        if (idx >= 0) {
            ip.erase(ip.find("\n"));
        }else{
            logger->debugStream()<<"query "<< hostname<< " return without \\n";
            return -1;
        }
        logger->debugStream()<<"query result is ["<<ip<<"]";
    }
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
    if (!is_init){
        if(init_dfor() != 0)
            return (*orig_gethostbyname)(name);
    }
    logger->debugStream() << "call gethostbyname";
    std::string hostname = std::string(name);
    std::string ip;
    int ret = query(hostname, ip);
    struct hostent *host;
    struct in_addr *addr;
    if (ret != 0 || ip == "") {
        logger->debugStream() << "query fail or ip is empty";
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
    if (!is_init){
        if(init_dfor() != 0) return -1;
    }
    logger->debugStream() << "call getaddrinfo node:"<<node;
    if (node == NULL) {
        logger->debugStream() << "call getaddrinfo node is empty";
        return (*orig_getaddrinfo)(node,service,hints,res);
    }
    std::string hostname = std::string(node);
    std::string ip;
    int ret = query(hostname, ip);
    if (ret != 0 || ip == "") {
        logger->debugStream()<<"query fail or ip is empty";
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
        //dump_addr(*res, service);
        return 0;
    }

}//}}}
