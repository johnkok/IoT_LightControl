#include<stdio.h> //for printf
#include<string.h> //memset
#include<sys/socket.h>    //for socket ofcourse
#include<stdlib.h> //for exit(0);
#include<errno.h> //For errno - the error number
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header

int main(){
   int sockfd,n;
   struct sockaddr_in servaddr,cliaddr;
   char sendline[10];
   char recvline[10];

   sockfd=socket(AF_INET,SOCK_STREAM,0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=inet_addr("192.168.1.51");
   servaddr.sin_port=htons(8888);

   connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

sendline[0] = 0x19;
sendline[1] = 0x78;
sendline[2] = 0x11;
sendline[3] = 0x00;
sendline[4] = sendline[0] + sendline[1] + sendline[2] + sendline[3];

      sendto(sockfd,sendline,5,0,
             (struct sockaddr *)&servaddr,sizeof(servaddr));
      n=recvfrom(sockfd,recvline,1,0,NULL,NULL);
      printf("RECV %X\n", recvline[0]);
sleep(1);



sendline[2] = 0x21;
sendline[4] = sendline[0] + sendline[1] + sendline[2] + sendline[3];

      sendto(sockfd,sendline,5,0,
             (struct sockaddr *)&servaddr,sizeof(servaddr));
      n=recvfrom(sockfd,recvline,1,0,NULL,NULL);
      printf("RECV %X\n", recvline[0]);
sleep(1);

sendline[2] = 0x31;
sendline[4] = sendline[0] + sendline[1] + sendline[2] + sendline[3];

      sendto(sockfd,sendline,5,0,
             (struct sockaddr *)&servaddr,sizeof(servaddr));
      n=recvfrom(sockfd,recvline,1,0,NULL,NULL);
      printf("RECV %X\n", recvline[0]);
sleep(1);

sendline[2] = 0x00;
sendline[4] = sendline[0] + sendline[1] + sendline[2] + sendline[3];

      sendto(sockfd,sendline,5,0,
             (struct sockaddr *)&servaddr,sizeof(servaddr));
      n=recvfrom(sockfd,recvline,1,0,NULL,NULL);
      printf("RECV %X\n", recvline[0]);
sleep(1);

sendline[2] = 0x30;
sendline[4] = sendline[0] + sendline[1] + sendline[2] + sendline[3];

      sendto(sockfd,sendline,5,0,
             (struct sockaddr *)&servaddr,sizeof(servaddr));
      n=recvfrom(sockfd,recvline,1,0,NULL,NULL);
      printf("RECV %X\n", recvline[0]);
sleep(1);

sendline[2] = 0x20;
sendline[4] = sendline[0] + sendline[1] + sendline[2] + sendline[3];

      sendto(sockfd,sendline,5,0,
             (struct sockaddr *)&servaddr,sizeof(servaddr));
      n=recvfrom(sockfd,recvline,1,0,NULL,NULL);
      printf("RECV %X\n", recvline[0]);
sleep(1);

sendline[2] = 0x10;
sendline[4] = sendline[0] + sendline[1] + sendline[2] + sendline[3];

      sendto(sockfd,sendline,5,0,
             (struct sockaddr *)&servaddr,sizeof(servaddr));
      n=recvfrom(sockfd,recvline,1,0,NULL,NULL);
      printf("RECV %X\n", recvline[0]);
sleep(1);

   return 0;
}
