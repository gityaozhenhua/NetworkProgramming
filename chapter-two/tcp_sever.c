#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char * message);

void error_handling(char * message) {
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc, char *argv[])
{
    int serv_sock;  // 服务端fd
    int clnt_sock;  // 客服端fd

    struct sockaddr_in serv_addr; // 描述服务端的信息 
    struct sockaddr_in clnt_addr; // 描述客户端的信息
    socklen_t clnt_addr_size;     // int类型的客户端地址长度

    char message[] = "hello world!"; // 要发送的信息

    if(argc != 2) {
        printf("usage : %s <port> \n",argv[0]);
        exit(0);
    }

    // 创建socket
    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock == -1) {
        error_handling("socket() error");
    }
    // 写入服务端的信息 协议簇 端口地址  htonl htons
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));

    // 绑定
    if(bind(serv_sock,(struct sockaddr*) &serv_addr,sizeof(serv_addr)) == -1) {
        error_handling("bind() error");
    }
    // 监听
    if(listen(serv_sock,5) == -1) {
        error_handling("listen() error");
    }
    // 一个int类型的 存的是客户端的地址
    clnt_addr_size = sizeof(clnt_addr);
    // accept 阻塞函数
    clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
    if(clnt_sock == -1) {
        error_handling("accept() error");
    }
    // 写数据
    write(clnt_sock,message,sizeof(message));
    close(clnt_sock);
    close(serv_sock);



    return 0;

}