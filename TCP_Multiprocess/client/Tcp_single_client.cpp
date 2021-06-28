#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sockfd < 0)
    {
        perror("socket");
        return 0;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(18989);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    int ret = connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    if(ret < 0)
    {
        perror("connect");
        return 0;
    }


    while(1)
    {

        sleep(1);

       char buf[1024] = {0};
       sprintf(buf,"hello server,i am client2");
       ssize_t send_ret = send(sockfd,buf,strlen(buf),0);
       if(send_ret < 0)
       {
           perror("send");
           continue;
       }


       memset(buf,'\0',sizeof(buf));
       ssize_t recv_ret = recv(sockfd,buf,sizeof(buf)-1,0);
       if(recv_ret < 0)
       {
           perror("recv");
           continue;
       }
       else if(recv_ret == 0)
       {
           printf("server close");

           close(sockfd);

           return 0;
       }

       printf("server say: %s\n",buf);
    }

    close(sockfd);
    return 0;
}
