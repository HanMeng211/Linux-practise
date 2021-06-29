#include"Tcp_thread_server.hpp"
#include<pthread.h>

void* MyThreadStart(void* arg)
{
    TcpSocket* ts = (TcpSocket*)arg;

    while(1)
    {
        string data = "";
        int recv_ret = ts->Recv(&data);
        if(recv_ret < 0)
        {
            printf("recv fail");
            continue;
        }
        else if(recv_ret == -2)
        {
            printf("client close\n");
            ts->Close();
            delete ts;
            return 0;
        }
        printf("client say:%s\n",data.c_str());

        data.clear();
        data.assign("hello client,i am server");
        ssize_t send_ret = ts->Send(data);
        if(send_ret < 0)
        {
            printf("send fail\n");
            continue;
        }
    }
    return NULL;
}

int main()
{
    TcpSocket tcp;

    int ret = tcp.Socket();
    if(ret < 0)
    {
        return -1;
    }

    ret = tcp.Bind();
    if(ret < 0)
    {
        return -1;
    }

    ret = tcp.Listen();
    if(ret < 0)
    {
        return -1;
    }

    while(1)
    {
        struct sockaddr_in addr;
        int new_sock = tcp.Accept(&addr);
        if(new_sock < 0)
        {
            continue;
        }

        TcpSocket* ts = new TcpSocket();
        if(ts == NULL)
        {
            close(new_sock);
            continue;
        }

        ts->SetSockfd(new_sock);

        pthread_t tid;
        int ret = pthread_create(&tid,NULL,MyThreadStart,(void*)ts);
        if(ret < 0)
        {
            perror("pthread_create");

            ts->Close();
            delete ts;
            continue;
        }

        //线程分离
        pthread_detach(tid);
    }
    return 0;
}
