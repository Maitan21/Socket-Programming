//
//  Server.c
//  socket
//
//  Created by Jho on 2020/03/30.
//  Copyright © 2020 COMP420. All rights reserved.
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
    int hServSock, hClntSock;
    char message[BUF_SIZE];
    int strLen=0;
    int idx=0, readLen=0;
    socklen_t szClntAddr;
    struct sockaddr_in servAddr,clntAddr;
   
    hServSock=socket(PF_INET, SOCK_STREAM, 0);//소켓 개통
    if(hServSock<0)
        ErrorHandling("socket() error");
  
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family=AF_INET;//주소체계 지정
    servAddr.sin_addr.s_addr=htonl(INADDR_ANY);//IP주소 리드
    servAddr.sin_port=htons(35124); //포트 번호 htons->호스트 바이트 순서를 short타입 네트워크바이트 순서로 변환
    
    if(bind(hServSock, (struct sockaddr*) &servAddr, sizeof(servAddr))<0) //주소 할당
        ErrorHandling("bind() error");
    
    if(listen(hServSock, 5)<0) //대기 큐 5개
        ErrorHandling("listen() error");

    

       szClntAddr=sizeof(struct sockaddr_in);
       hClntSock=accept(hServSock, (struct sockaddr*)&clntAddr, &szClntAddr); //연결수락
       if(hClntSock<0)
       ErrorHandling("accept() error");
       
       /* recv, send 이용
       while (1) {
            printf("Waiting Message...\n");
            readLen = recv(hClntSock,message, BUF_SIZE, 0); //read
            if (readLen==0)
                break; // done reading
            strLen = send(hClntSock, message, readLen, 0); //write
          }
        */
    //리눅스 read write
    printf("Connection success\n");
    while((strLen=read(hClntSock,message,BUF_SIZE))!=0){
        write(hClntSock, message, strLen);
        printf("Message from Client: %s",message);
        memset(message,0,BUF_SIZE); //배열 초기화
    }
    
    close(hClntSock);
}

void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

