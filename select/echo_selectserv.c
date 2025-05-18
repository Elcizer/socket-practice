#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
void error_handling(char *message);

int main(int argc,char * argv[]){
    int serv_sock,clnt_sock;
    struct sockaddr_in serv_adr,clnt_adr;
    struct timeval timeout;
    fd_set reads,cpy_reads;

    socklen_t adr_sz;

    int fd_max, str_len, fd_num, i;
    char buf[BUF_SIZE];
    if(argc!=2){
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    memset(&serv_adr,0,sizeof(serv_sock));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr *)&serv_adr,sizeof(serv_adr))==-1) error_handling("bind() error");

    if(listen(serv_sock,5)==-1) error_handling("listen() error");

    FD_ZERO(&reads);
    FD_SET(serv_sock,&reads);
    fd_max = serv_sock;

    while(1){
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        if((fd_num = select(fd_max+1,&cpy_reads,0,0,&timeout))==-1) // 오류 발생
            break;
        if(fd_num==0) // 타임 아웃 다시 수행 
            continue;
        // 여기로 넘어오면 변화가 생겨서 처리해야하는 경우 
        for(i=0;i<fd_max+1;i++){
            if(FD_ISSET(i,&cpy_reads)){ // i에 변화가 생겼는지 확인 (수신, 송신)
                if(i==serv_sock){ // 만약 serv_sock에 변화 생김 -> 새로운 클라이언트 접근 
                    adr_sz = sizeof(clnt_adr);
                    clnt_sock = accept(serv_sock,(struct sockaddr *)&clnt_adr,&adr_sz);
                    FD_SET(clnt_sock,&reads); // clnt_sock을 fd_set에 등록
                    if(fd_max<clnt_sock) fd_max = clnt_sock; // 만약 clnt_sock이 기존 fd_max보다 크다면 업데이트 해야함.
                    printf("connected client : %d\n",clnt_sock);
                }
                else{ // read message -> serv_sock이 아닌 다른 파일 디스크립터가 반응 -> 등록해둔 클라이언트의 파일디스크립터 (메세지 수신 가능 )
                    str_len = read(i,buf,BUF_SIZE); 
                    if(str_len==0){ // 클라이언트에서 EOF를 보냄 (FIN을 전송) -> 소켓 소멸 
                        FD_CLR(i,&reads); // fd_set에서 삭제
                        close(i); 
                        printf("close client : %d\n",i);
                    }
                    else write(i, buf, str_len);
                }
            }
        }
    }
    close(serv_sock);
    return 0;
}
void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
