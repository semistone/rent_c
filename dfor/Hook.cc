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


struct hostent *gethostbyname(const char *name){//{{{
    if (orig_gethostbyname == NULL)
    {
         orig_gethostbyname = (struct hostent *((*)(const char *name)))dlsym(RTLD_NEXT, "gethostbyname");
    }
    return (*orig_gethostbyname)(name);
    // TODO: print desired message from caller. 
}//}}}

int 
init_dfor(){//{{{
    std::string dbfile = "/var/run/dfor/cache.db";
    FILE *fp = fopen(dbfile.c_str(), "r");
    if(fp){
        fclose(fp);
    } else {
        return -1; // if not exist.
    }
    log4cpp::PropertyConfigurator::configure("/usr/local/etc/dfor/log4cpp.properties");
    db = new dfor::DB(dbfile);
}//}}}

int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res){//{{{
    if (orig_getaddrinfo == NULL)
    {
         orig_getaddrinfo = (int (*)(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res))
                             dlsym(RTLD_NEXT, "getaddrinfo");
    }
    // init 
    if (db == NULL){
        if(init_dfor() != 0) return -1;
    }
    std::string hostname = std::string(node);
    std::string ip = db->query(hostname);
    if (ip == "") {
        return (*orig_getaddrinfo)(node,service,hints,res);
    } else {
        struct sockaddr_in *sock;
        struct addrinfo  *addr;
        char* name = (char*) malloc(100);
        snprintf(name, 100, "%s", "dfor canonname"); 
        addr = (addrinfo*) malloc(sizeof(addrinfo));
        sock = (sockaddr_in*) malloc(sizeof(sockaddr_in));
        memset(addr, 0, sizeof(struct addrinfo));
        memset(sock, 0, sizeof(struct sockaddr_in));
        addr->ai_family = AF_INET;
        addr->ai_socktype = SOCK_STREAM;
        addr->ai_addrlen = sizeof(sockaddr_in);
        addr->ai_addr = (sockaddr*)sock;
        addr->ai_canonname = name;
        addr->ai_next = NULL;
        sock->sin_family = AF_INET;
        sock->sin_addr.s_addr = inet_addr(ip.c_str());
        *res = addr;
        db->updateCount(hostname, ip);
        return 0;
    }

}//}}}
