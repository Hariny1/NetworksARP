#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

struct sdata
{
	int ip[4];
	char buffer[20];
};

void ValIP(int ip[4])
{
	int i,j,flag;
	char string[20];
	
	do
	{	
		for(i=0;i<4;i++)
		ip[i]=0;

		scanf("%s",string);
				
		flag=1;
		i=0;

		for(j=0;string[j]!='\0';j++)
		{
			if(string[j]=='.')
			{	
				i++;
				continue;
			}		
				
			ip[i]*=10;
			
			ip[i]+= string[j] - '0';	
		}		
	
		if(i!=3)
		{
			printf("\nInvalid Address!\n\nEnter IP ");
			flag=0;
		}

		else
		{
			for(i=0;i<4;i++)
			if(ip[i] < 0 || ip[i] > 255)
			{	
				flag=0;
				break;
			}
	
			if(flag==0)
			printf("\nInvalid Address!\n\nEnter IP ");
		}

	}while(flag==0);

}

int main()
{
	int sockfd,i;

	struct sdata p;

	int ip[4];

	struct sockaddr_in servaddr,cliaddr;

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8889);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if ( (sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0 )
	printf("\nSocket Error!");

	printf("Enter the subnet address : ");
	ValIP(ip);
		
		
	if ( connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) < 0 )
	printf("\nConnect Error");

	printf("\n\nConnection established.\n");
	
	fflush(stdout);
	
	if( write(sockfd,(char*)ip,sizeof(ip)) < 0 )
	perror("\nWrite Error");
	
	if( read(sockfd,(char*)&p,sizeof(struct sdata)) < 0 )
	perror("\nRead Error");

	if(strcmp(p.buffer,"no")==0)
	{
		close(sockfd);
		return 0;
	}

	printf("\nReceived Data is : ");
	
	for(i=0;i<4;i++)
	printf("%d.",p.ip[i]);
	
	printf("\b || ");
	
	printf("%s\n\n",p.buffer);
	
	close(sockfd);

	return 0;
}
