#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc,char *argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len;

    if(argc!=3){
        printf("Usage %s <IP> <PORT>\n",argv[0]);
        exit(1);
    }

    sock = socket(PF_INET,SOCK_STREAM,0);
    if(sock==-1) error_handling("socket() error");

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr))==-1) error_handling("connect() error\n");
    else printf("Connected..........\n");

    int cnt;
    printf("Operand count: ");
    scanf("%d",&cnt);
    write(sock,&cnt,sizeof(cnt));

    for(int i=0;i<cnt;i++){
        int temp = 0;
        printf("Operand %i: ",i+1);
        scanf("%d",&temp);
        write(sock,&temp,sizeof(temp));
    }
    fgetc(stdin);
    char operator;
    printf("Operator : ");
    scanf("%c",&operator);
    write(sock,&operator,sizeof(operator));

    int num;
    read(sock,&num,sizeof(num));
    printf("Operation result: %d",num);
    close(sock);

}
void error_handling(char *message){
    fputs(message,stderr);
    fputc('n',stderr);
    exit(1);
}