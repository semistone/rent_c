#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <dlfcn.h>
#include <sys/socket.h>
#include <netdb.h>
static struct hostent *((*orig_gethostbyname)(const char *name)) = NULL;
static int (*orig_getaddrinfo)(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res) = NULL;

struct hostent *gethostbyname(const char *name)
{
    if (orig_gethostbyname == NULL)
    {
         orig_gethostbyname = (struct hostent *((*)(const char *name)))dlsym(RTLD_NEXT, "gethostbyname");
    }
    printf("hook success\n");
    // TODO: print desired message from caller. 
    return (*orig_gethostbyname)(name);
}

int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res){
    if (orig_getaddrinfo == NULL)
    {
         orig_getaddrinfo = (int (*)(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res))dlsym(RTLD_NEXT, "getaddrinfo");
    }
    printf("hook success\n");
    // TODO: print desired message from caller. 
    return (*orig_getaddrinfo)(node,service,hints,res);

}
