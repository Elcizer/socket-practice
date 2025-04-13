#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345

int main() {
    int server_fd, client_fd;
    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[1024] = {0};

    // 1. 소켓 생성
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. 주소 설정
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    // 3. 바인드
    bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    // 4. 리슨
    listen(server_fd, 5);
    printf("서버 실행 중... 포트 %d\n", PORT);

    // 5. 클라이언트 연결 대기
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    printf("클라이언트 연결됨\n");

    // 6. 메시지 읽기
    read(client_fd, buffer, 1024);
    printf("클라이언트 메시지: %s\n", buffer);

    // 7. 응답 보내기
    send(client_fd, "Hello from server!", strlen("Hello from server!"), 0);

    // 8. 종료
    close(client_fd);
    close(server_fd);

    return 0;
}