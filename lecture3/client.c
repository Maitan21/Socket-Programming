
//
//  client.c
//  socket
//
//  Created by Jho on 2020/03/30.
//  Copyright © 2020 COMP420. All rights reserved.
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
    int hSocket;
    struct sockaddr_in serveraddr;

    char message[30];
    int strLen=0;
    int idx=0, readLen=0;

    if(argc!=3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    
    hSocket=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(hSocket<0)
        ErrorHandling("hSocket() error");
    
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family=AF_INET; //IPv4 정보체계저장
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]); //주소할당
    serveraddr.sin_port=htons(atoi(argv[2])); //포트할당
    
    if(connect(hSocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
        ErrorHandling("connect() error!");

    
    while(1)
    {
        fputs("전송할 메시지를 입력하세요 (q to quit) : ", stdout);
        fgets(message, BUF_SIZE, stdin);
        
        if(!strcmp(message, "q\n"))
            break;
        write(hSocket, message, strlen(message));
        strLen = read(hSocket, message, BUF_SIZE-1);
        message[strLen]=0;
        
       printf("\nMessage from server: %s \n", message);
    }

    close(hSocket);
}

void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
