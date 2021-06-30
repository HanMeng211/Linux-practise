#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main()
{
    int listen_sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(listen_sockfd < 0)
    {
        perror("socket");
        return 0;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(18989);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    int ret = bind(listen_sockfd,(struct sockaddr*)& addr,sizeof(addr));
    if(ret < 0)
    {
        perror("bind");
        return 0;
    }

    ret = listen(listen_sockfd,1);
    if(ret < 0)
    {
        perror("listen");
        return 0;
    }

    while(1)
    {
        sleep(1);
    }
    return 0;
}
