# socket-practice

## 2025 / 04 / 14
지금 server는 응답을 하나만 받고 바로 프로그램을 종료하는 상태    
client는 계속해서 quit을 리턴하기 전까지 계속 보내는 프로그램

서버가 한번 응답을 받고 종료 되고    
클라이언트는 그 상태에서 한 번더 보낼 수 있는데
이때는 아무것도 오지 않음   
그리고 한번더 텍스트를 입력하면 연결이 종료됨.. 

## 2025 / 05 / 12
### iterative   
서버 : 5번의 connect를 수신    
클라이언트 : 한번의 송신 후 바로 연결을 끊지 않고 q Q가 나올때 까지 반복 

### operate   
클라이언트가 operator(+,-,*)와 operand를 보내면 결과를 전달하는 서버

## 2025 / 05 / 15   
### UDP      
UDP기반 서버 / 클라 구현    

### DNS   
IP -> DNS NAME / DNS NAME -> IP 함수 실행하는 프로그램


## 2025 / 05 / 17
### 멀티 프로세스   
좀비 프로세스 + 시그널 처리   

## 2025 / 05 / 18
### select   
select를 이용해 멀티플렉싱 기반 서버 구현

## 2025 / 05 / 19
브로드 캐스트 sender / receiver 작성

first 파일의 에코 클라이언트 입출력을 표준 입출력(fgets, fputs)으로 수정
