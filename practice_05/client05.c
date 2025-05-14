#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);


int main(int argc, char* argv[]) {
    int sock;
    struct sockaddr_in serv_addr;
    int str_len;

    if(argc!=3){
        printf("Usage : %s <IP> <port> \n",argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock==-1) error_handling("socket() error");

    memset(&serv_addr, 0 , sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    char message[BUF_SIZE] =' ';

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1) error_handling("connect() error");
    for(int i=0;i<3;i++){
        printf("Length : ");
        scanf("%d",(int *)&message[0]);
        printf("Message :");
        scanf("%s",&message[4]);
        write(sock,&message,(int)message[0]+4);

        int str_len;
        int rcv_cnt;
        int rcv_idx = 0;

        str_len = read(sock,&str_len,4);
        while(rcv_idx<str_len){
            rcv_cnt = read(sock,&message[rcv_idx],BUF_SIZE-1);
            if(rcv_cnt == -1) error_handling("read error()");
            rcv_idx += rcv_cnt;
        }
        printf("Message from server : ");
        for(int j=0;j<str_len;j++) printf("%c",message[j]);
        printf("\n");
    }
    
    close(sock);
    return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}