//
//  Server.c
//  socket
//
//  Created by Jho on 2020/04/07.
//  Copyright © Jho All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void ErrorHandling(char* message);

int main()
{
    int hServSock, hClntSock; //서버 및 클라이언트 소켓
    char message[BUF_SIZE];
    int strLen=0; //수신한 데이터의 길이의 제어변수
    socklen_t szClntAddr; //클라이언트 주소 구조체의 바이트 크기
    struct sockaddr_in servAddr,clntAddr; // 서버용 소켓, 클라이언트용 소켓
   
    hServSock=socket(PF_INET, SOCK_STREAM, 0);//소켓 개통
    if(hServSock<0)
        ErrorHandling("socket() error");
  
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family=AF_INET;//주소체계 지정
    servAddr.sin_addr.s_addr=htonl(INADDR_ANY);//IP주소 리드
    servAddr.sin_port=htons(35124); //포트 번호 htons->호스트 바이트 순서를 short타입 네트워크바이트 순서로 변환
    
    if(bind(hServSock, (struct sockaddr*) &servAddr, sizeof(servAddr))<0) //주소 할당
        ErrorHandling("bind() error");
    
    if(listen(hServSock, 3)<0) //대기 큐 5개
        ErrorHandling("listen() error");


       szClntAddr=sizeof(struct sockaddr_in);
       
       for (int i=0; i<3;i++)
       {
         hClntSock=accept(hServSock, (struct sockaddr*)&clntAddr, &szClntAddr); //연결수락
         if (hClntSock == -1)
           ErrorHandling("accept() error");
         else 
             printf("Connected client %d \n",i+1);
            //accept 성공시 클라이언트 소켓 생성
            //클라이언트가 소켓을 닫아버리면 NULL(0) 반환
         while ((strLen = recv(hClntSock,message, BUF_SIZE, 0))!=0)
         {
             //send(hClntSock,message,  strLen,0);
             write(hClntSock,message,strLen);
         }
           close(hClntSock);
           printf("Client %d is disconnected \n",i+1);
           
       }
  
    printf("서버 프로그램을 종료합니다.\n");
    close(hServSock);
      // if(hClntSock<0)
       //ErrorHandling("accept() error");
       
       /* recv, send 이용
       
    printf("Connection success\n");
    while((strLen=read(hClntSock,message,BUF_SIZE))!=0){
        write(hClntSock, message, strLen);
        printf("Message from Client: %s",message);
        memset(message,0,BUF_SIZE); //배열 초기화
    }
    
    close(hClntSock);
    */
}

void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

