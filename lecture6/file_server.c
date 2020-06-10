//
//  file_server.c
//  socket programming
//
//  Created by Jho on 2020/04/20.
//  Copyright © jho All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sd, clnt_sd;
	FILE * fp;
	char buf[BUF_SIZE];
	int read_cnt;
	
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;
	
	if(argc!=2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	
	
    if ((fp= fopen("input.txt", "rb"))== NULL) //파일 존재유무 예외처리
    {
        printf("파일이 존재하지 않습니다.");
        return 1;
    }
	serv_sd=socket(PF_INET, SOCK_STREAM, 0);   
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET; //주소체계 지정
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY); //IP주소 리드
	serv_adr.sin_port=htons(atoi(argv[1])); //포트 번호
	
	bind(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
	listen(serv_sd, 5);
	
	clnt_adr_sz=sizeof(clnt_adr);    
	clnt_sd=accept(serv_sd, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
	
	while(1)
	{
        //file의 끝까지 1byte씩 읽음
		read_cnt=fread((void*)buf, 1, BUF_SIZE, fp);
		if(read_cnt<BUF_SIZE)
		{
            //읽은 데이터를 전송
			write(clnt_sd, buf, read_cnt);
			break;
		}
        else if(read_cnt == -1) //비정상 종료
            break;
        
		write(clnt_sd, buf, BUF_SIZE);
	}
	
	shutdown(clnt_sd, SHUT_WR);	 //송신 스트림 종료(EOF리턴)
	read(clnt_sd, buf, BUF_SIZE);
	printf("Message from client: %s \n", buf);
	
	fclose(fp);
	close(clnt_sd); close(serv_sd);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
