#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <dlfcn.h>
#include <sys/socket.h>
#include <netdb.h>
static struct hostent *((*orig_gethostbyname)(const char *name)) = NULL;
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
