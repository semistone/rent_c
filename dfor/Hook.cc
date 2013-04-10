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

static struct hostent *((*orig_gethostbyname)(const char *name)) = NULL;
static int (*orig_getaddrinfo)(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res) = NULL;
static dfor::DB *db = new dfor::DB("/var/run/dfor/cache.db");
struct hostent *gethostbyname(const char *name)
{
    if (orig_gethostbyname == NULL)
    {
         orig_gethostbyname = (struct hostent *((*)(const char *name)))dlsym(RTLD_NEXT, "gethostbyname");
    }
    printf("hook success\n");
    return (*orig_gethostbyname)(name);
    // TODO: print desired message from caller. 
}

int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res){
    if (orig_getaddrinfo == NULL)
    {
         orig_getaddrinfo = (int (*)(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res))
                             dlsym(RTLD_NEXT, "getaddrinfo");
    }
    printf("hook success query %s\n", node);
    // TODO: print desired message from caller. 
    std::string ip = db->query(std::string(node), dfor::FAILOVER);
    if (ip == "") {
        return (*orig_getaddrinfo)(node,service,hints,res);
    } else {
        struct sockaddr_in sock;
        struct addrinfo  addr;
        memset(&addr, 0, sizeof(struct addrinfo));
        addr.ai_family = AF_INET;
        addr.ai_socktype = SOCK_STREAM;
        addr.ai_canonname = NULL;
        addr.ai_addr = (sockaddr*)&sock;
        addr.ai_next = NULL;
        memset(&sock, 0, sizeof(struct sockaddr_in));
        sock.sin_family = AF_INET;
        sock.sin_addr.s_addr = inet_addr("192.168.0.1");
        *res = &addr;
        return 0;
    }

}
