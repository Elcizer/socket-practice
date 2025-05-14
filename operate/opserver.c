#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc,char *argv[]){
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[] = "Hello World";

    if(argc!=2) {printf("Usage %s <port>\n",argv[0]); exit(1);}

    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock==-1) error_handling("socket() error");

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr))==-1) error_handling("bind() error");
    
    if(listen(serv_sock,5)==-1) error_handling("listen() error\n");
    else printf("listen start......\n");

    clnt_addr_size = sizeof(clnt_addr);

    clnt_sock = accept(serv_sock,(struct sockaddr *)&clnt_addr,&clnt_addr_size);
    if(clnt_sock==-1) error_handling("accept() error\n");
    printf("Connected.............\n");

    int cnt;
    if(read(clnt_sock,&cnt,sizeof(cnt))==-1) error_handling("read() error\n");
    printf("Operand count : %d\n",cnt);
    int arr[cnt];
    for(int i=0;i<cnt;i++){
        if(read(clnt_sock,&arr[i],sizeof(arr[0]))==-1) error_handling("read() error\n");
        printf("Operand %d: %d\n",i+1,arr[i]);
    }
    char operator;
    if(read(clnt_sock,&operator,sizeof(operator))==-1) error_handling("read() error\n");
    printf("Operator : %c\n",operator);

    int ans = arr[0];
    if(operator=='-'){
        for(int i=1;i<cnt;i++){
        ans-=arr[i];
        }
    }
    else if(operator=='+'){
        for(int i=1;i<cnt;i++){
        ans+=arr[i];
        }
    }
    else if(operator=='*'){
        for(int i=1;i<cnt;i++){
        ans*=arr[i];
        }
    }
    printf("Operation result : %d\n",ans);
    write(clnt_sock,&ans,sizeof(ans));
    close(clnt_sock);
    close(serv_sock);
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('n',stderr);
    exit(1);
}