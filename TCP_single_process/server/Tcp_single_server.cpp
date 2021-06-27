#include<stdio.h>
#include<string.h>
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
    int ret = bind(listen_sockfd,(struct sockaddr*)&addr,sizeof(addr));
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

    struct sockaddr_in peer_addr;
    socklen_t socklen = sizeof(peer_addr);
    int new_sockfd = accept(listen_sockfd,(struct sockaddr*)&peer_addr,&socklen);
    if(new_sockfd < 0)
    {
        perror("accept");
        return 0;
    }

    while(1)
    {
        char buf[1024] = {0};
        ssize_t recv_ret = recv(new_sockfd,buf,sizeof(buf)-1,0);
        if(recv_ret < 0)
        {
            perror("recv");
            continue;
        }
        else if(recv_ret == 0)
        {
            printf("client close");
            close(new_sockfd);
            close(listen_sockfd);
            return 0;
        }

        printf("client%d say:%s\n",new_sockfd,buf);

        memset(buf,'\0',sizeof(buf));
        sprintf(buf,"hello,i am server ip is %s,port is %d\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
        ssize_t send_ret = send(new_sockfd,buf,strlen(buf),0);
        if(send_ret < 0)
        {
            perror("send");
            continue;
        }

    }
    close(new_sockfd);
    close(listen_sockfd);
    return 0;
}
