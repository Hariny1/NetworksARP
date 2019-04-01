#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include <time.h>
int fra;
struct frame
{
	int seqno;
	int d[5];
};


struct ackpak
{
	int ackn;
	int frameno;
	int timeout;
};

int error(struct frame frbuff)
{
	int i;
	int noofone=0;
	for(i=0;i<fra;i++)
		if(frbuff.d[i])
			noofone++;
		if(frbuff.d[i]==noofone%2)
			return 0;
		else
			return 1;
}

int main(int argc,char **argv)
{
	int fr=0,ftbr=0,i,j,num,cNum,newfd,len,frasize,ch;
	//int userdata[16];
	int sd,cli_sockaddlen,m,n;
	int server_sockaddlen;
	struct sockaddr_in server_sockadd,cli_sockadd;
	char buf[20];
	int cho;//,frabuf[1],numbuf[1];


	struct frame frbuff;
	struct ackpak ack;

	sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd<0)
	perror("Could not create socket!");

	server_sockaddlen = sizeof(server_sockadd);

	bzero(&server_sockadd,server_sockaddlen);

	server_sockadd.sin_family = AF_INET;
	server_sockadd.sin_port = htons(7714);
	server_sockadd.sin_addr.s_addr =inet_addr(argv[1]); //inet_addr("127.0.0.1");
	//servaddr.sin_addr.s_addr=INADDR_ANY;
	cli_sockaddlen = sizeof(cli_sockadd);

	if(bind(sd,(struct sockaddr*)&server_sockadd,sizeof(server_sockadd))<0)
	{
		perror("Bind Error");
		return 0;
	}
	if(listen(sd, 5)<0)
         perror("Cannot listen");
	len=sizeof(cli_sockadd);
	newfd=accept(sd,(struct sockaddr*)&cli_sockadd,&len);


	read(newfd,&cNum,sizeof(cNum),0);
	num=ntohl(cNum);
	printf("Data Size is %d ",num);

	read(newfd,&frasize,sizeof(frasize),0);
	fra=ntohl(frasize);
	printf("Frame Size is %d ",fra);
	struct frame f[fra];
	int userdata[num+100];
	if((int)((num/frasize)+1)!=num/frasize)
		num=num+1;
	while(fr!=(int)(num/fra))
	{
		m=recvfrom(newfd,(char*)&frbuff,sizeof(struct frame),0,(struct sockaddr*)&cli_sockadd,&cli_sockaddlen);

		printf("\n\nReceived Frame %d - ",frbuff.seqno);
		printf("Size is %d ",fra);
		for(j=0;j<fra;j++)
		printf("%d",frbuff.d[j]);

		printf(" - %d\n",frbuff.d[j]);

		printf("Frame to be received is %d and received frame seq no is %d",ftbr,frbuff.seqno);
		//printf("\nSend ACK ? ");
		//int milliseconds=5000;
		if(frbuff.seqno==ftbr)
		{
			//printf("\nSend ACK ? ");
			if(!error(frbuff))
			{
				int a,b;
				printf("\nSend ACK ? ");
				scanf("%d",&ch);
			  cho=1;
				for(a=100;a<=0;a++)
				{
					for(b=100;b<=0;b++)
					{
					cho=0;
					int milliseconds=5000;
					printf("\nTimeout");
						if (milliseconds <= 0)
					{
		        printf( "Timeout\n");
		        //return;
    			}
    			int milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
    			int end = milliseconds_since + milliseconds;
    			do
						{
		        		milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
		    		}	while (milliseconds_since <= end);
						//ack.timeout=1;
						//n = sendto(newfd,(char*)&ack,sizeof(struct ackpak),0,(struct sockaddr*)&cli_sockadd,cli_sockaddlen);
				}
			}
				fflush(stdin);
				//cho=1;
				if(cho==1&&ch==1)
				{
					f[fr]=frbuff;
					fr++;
					ftbr=(!ftbr);

					ack.ackn=1;
					ack.frameno=ftbr;
					ack.timeout=0;

					n = sendto(newfd,(char*)&ack,sizeof(struct ackpak),0,(struct sockaddr*)&cli_sockadd,cli_sockaddlen);

					if(n>0)
					printf("ACK Sent");
					else
					printf("ACK could not be Sent");
				}

				else
				{
					int milliseconds=5000;
					printf("\nTimeout");
						if (milliseconds <= 0)
					{
		        printf( "Timeout\n");
		        //return;
    			}
    			int milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
    			int end = milliseconds_since + milliseconds;
    			do
						{
		        		milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
		    		}		 while (milliseconds_since <= end);
						printf( "Timeout\n");
						ack.timeout=1;
					n = sendto(newfd,(char*)&ack,sizeof(struct ackpak),0,(struct  sockaddr*)&cli_sockadd,cli_sockaddlen);
				}


			}

			else
			{
				ack.ackn=0;
				ack.frameno=ftbr;
				ack.timeout=0;
				n = sendto(newfd,(char*)&ack,sizeof(struct ackpak),0,(struct sockaddr*)&cli_sockadd,cli_sockaddlen);
				printf("\nError in Data. NACK sent.");
			}

			fflush(stdout);
			fflush(stdin);
		}

		else
		{
			ack.ackn=0;
			ack.frameno=ftbr;
			n = sendto(newfd,(char*)&ack,sizeof(struct ackpak),0,(struct sockaddr*)&cli_sockadd,cli_sockaddlen);
			printf("\nWrong frame received. NACK sent.%d",ack.ackn);
		}
	}

	printf("\n\nThe data  is : ");

	for(i=0;i<(int)(num/fra);i++)
	for(j=0;j<fra;j++)
	printf("%d",f[i].d[j]);

	printf("\n");
	//close(newfd);
	close(sd);
	close(newfd);
	return 0;
	//close(sockfd);
	//close(newfd);
}
