
//
//  client.c
//  socket
//
//  Created by Jho on 2020/04/07.
//  Copyright © jho All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
    int hSocket; //서버에 접속할 소켓
    struct sockaddr_in serveraddr; //접속할 서버의 주소

    char message[BUF_SIZE];
    int str_len;

    if(argc!=3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    
    hSocket=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(hSocket<0)
        ErrorHandling("hSocket() error");
    
    memset(&serveraddr, 0, sizeof(serveraddr)); // 서버쪽 주소 구조체를 0 으로 초기화
    serveraddr.sin_family=AF_INET; //IPv4 정보체계저장
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]); //주소할당
    serveraddr.sin_port=htons(atoi(argv[2])); //포트할당
    
    
    if(connect(hSocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
        ErrorHandling("connect() error!");

    
    while(1)
    {
        
        fputs("전송할 메시지를 입력하세요 (q to quit) : ", stdout);
        fgets(message, BUF_SIZE, stdin);
        
        //q를 입력받으면 클라이언트 종료
        if(!strcmp(message, "q\n") || !strcmp(message,"Q\n"))
            break;
        
        write(hSocket,message,strlen(message)); //소켓을 이용해 메세지 전송
        str_len =read(hSocket,message, BUF_SIZE-1); //에코되어 돌아오는 메세지 수신
        message[str_len]=0; //보낼때 NULL문자 안보내서, 받았을 때 넣어줘야 한다.
        printf("\nMessage from server: %s \n", message);
    }

    close(hSocket);
    return 0;
}

void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
