#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define SERVER_IP "127.0.0.1"  // Codespaces에서는 같은 환경이므로 localhost 사용 가능

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char message[1024], buffer[1024];

    // 소켓 생성
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(1);
    }

    // 서버 주소 설정
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // 서버에 연결
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("connect");
        exit(1);
    }

    printf("Connected to server. Type message:\n");

    // 메시지 입력 후 서버로 전송
    while (1) {
        printf("You: ");
        fgets(message, sizeof(message), stdin);

        // quit 입력 시 종료
        if (strncmp(message, "quit", 4) == 0)
            break;

        write(sock, message, strlen(message));

        int len = read(sock, buffer, sizeof(buffer) - 1);
        buffer[len] = '\0';
        printf("Server: %s", buffer);
    }

    close(sock);
    return 0;
}