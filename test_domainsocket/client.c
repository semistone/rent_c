#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#define DATA "example.com\n"


int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_un server;
    char buf[1024];




    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("opening stream socket");
        exit(1);
    }
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, "/var/run/dfor/dfor.sock");


    if (connect(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un)) < 0) {
        close(sock);
        perror("connecting stream socket");
        exit(1);
    }
    if (write(sock, DATA, sizeof(DATA)) < 0)
        perror("writing on stream socket");
    if (read(sock, buf, sizeof(buf)) < 0)
        perror("read on stream socket");
    std::string ip = std::string(buf);
    ip.erase(ip.find("\n") + 1);
    printf("read data is %s", ip.c_str());
    close(sock);
}
