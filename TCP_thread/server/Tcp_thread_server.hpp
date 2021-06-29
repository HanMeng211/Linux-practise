#pragma once
#include<stdio.h>
#include<string>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<signal.h>
#include<sys/wait.h>
#include<pthread.h>

using namespace std;

class TcpSocket
{
    public:

        TcpSocket():sockfd_(-1)
        {}
        ~TcpSocket()
        {}

        void SetSockfd(int sockfd)
        {
            sockfd_ = sockfd;
        }
        //创建套接字
        int Socket()
        {
            sockfd_ = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
            if(sockfd_ < 0)
            {
                perror("socket");
                return -1;
            }
            return sockfd_;
        }

        //绑定地址信息
        int Bind(const string& ip = "0.0.0.0",uint16_t port = 18989)
        {
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            int ret = bind(sockfd_,(struct sockaddr*)&addr,sizeof(addr));
            if(ret < 0)
            {
                perror("bind");
                return -1;
            }
            return ret;
        }

        //侦听
        int Listen(int backlog = 5)
        {
            int ret = listen(sockfd_,backlog);
            if(ret < 0)
            {
                perror("listen");
                return -1;
            }
            return ret;
        }

        //接收新连接
        int Accept(struct sockaddr_in* addr)
        {
            socklen_t socklen = sizeof(struct sockaddr_in);
            int new_sockfd = accept(sockfd_,(struct sockaddr*)addr,&socklen);
            if(new_sockfd < 0)
            {
                perror("accept");
                addr = NULL;
                return -1;
            }
            return new_sockfd;
        }

        //接收
        ssize_t Recv(string* data)
        {
            data->clear();
            char buf[1024] = {0};
            ssize_t recv_ret = recv(sockfd_,buf,sizeof(buf)-1,0);
            if(recv_ret < 0)
            {
                perror("recv");
                return -1;
            }
            else if(recv_ret == 0)
            {
                printf("client close");

                return -2;
            }
            data->assign(buf,strlen(buf));
            return recv_ret;
        }

        //发送
        ssize_t Send(const string& data)
        {
            size_t send_ret = send(sockfd_,data.c_str(),data.size(),0);
            if(send_ret < 0)
            {
                perror("send");
                return -1;
            }
            return send_ret;
        }

        //关闭套接字
        void Close()
        {
            close(sockfd_);

            sockfd_ = -1;
        }

    private:
        int sockfd_;
};


