#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<signal.h>
#include<sys/wait.h>


void sigcallback(int signo)
{
    wait(NULL);
}

int main()
{

    signal(SIGCHLD,sigcallback);


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


    while(1)
    {
        //接收连接
        struct sockaddr_in peer_addr;
        socklen_t socklen = sizeof(peer_addr);
        int new_sockfd = accept(listen_sockfd,(struct sockaddr*)&peer_addr,&socklen);
        if(new_sockfd < 0)
        {
            perror("accept");
            return 0;
        }

        pid_t f_ret = fork();
        if(f_ret < 0)
        {
            perror("fork");
            continue;
        }
        else if(f_ret == 0)
        {
            close(listen_sockfd);
            //child

            while(1)
            {
                //接收
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
                    return 0;
                }

                printf("client new_sockfd:%d say:%s\n",new_sockfd,buf);


                //发送
                memset(buf,'\0',sizeof(buf));
                sprintf(buf,"hello,i am server,i recv client%d ip is %s,port is %d",new_sockfd,inet_ntoa(peer_addr.sin_addr),ntohs(peer_addr.sin_port));
                ssize_t send_ret = send(new_sockfd,buf,strlen(buf),0);
                if(send_ret < 0)
                {
                    perror("send");
                    continue;
                }

            }
        }
        else
        {
            //father
            //防止客户端关闭，子进程直接return 0退出产生僵尸进程
            //所以父进程需要进行等待，但不能在此次使用wait或waitpid
            //因为阻塞等待，若子进程一直不退出，则父进程一直在等待，永远无法接收新连接
            //需要使用自定义信号处理方式
            close(new_sockfd);
            continue;
        }
    }
    return 0;
}
