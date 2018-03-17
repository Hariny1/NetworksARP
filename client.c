#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

struct details
{
	int rep;
	char srcip[20];
	char srcmac[20];
	char destip[20];
	char destmac[20];
	char data[17];
};

void ValIP(char string[20])
{
	int i,j,flag=1;
	int ip[4];
	fflush(stdin);

	do
	{
		for(i=0;i<4;i++)
		ip[i]=0;

		scanf("%s",string);

		flag=1;
		i=0;

		printf("\n");

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
			printf("Invalid Address!\nEnter IP Address : ");
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
			printf("Invalid Address!\nEnter IP Address : ");
		}

	}while(flag==0);
}

int main()
{
	int sockfd;

	char buffer[10];

	char ip[20];
	char mac[20];
	//char str[2048];
	struct details req,reply,recv;

	struct sockaddr_in servaddr,cliaddr;

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(6770);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if ( (sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0 )
	printf("\nSocket Error!");
//Sending Message
//printf("Enter the message  ");
//scanf("%s",buff);
	printf("Enter the IP : ");
	ValIP(ip);

	fflush(stdout);

	printf("Enter the MAC : ");
	scanf("%s",mac);

	if ( connect(sockfd,(struct sockaddr*) &servaddr,sizeof(servaddr)) < 0 )
	printf("\nConnect Error");

	if( read(sockfd,(char*)&req,sizeof(struct details)) < 0 )
	perror("\nRead Error");

	printf("\nARP Request received : %s | %s | %s\n",req.srcip,req.srcmac,req.destip);

	reply = req;

	if(strcmp(req.destip,ip)!=0)
	{
		printf("\nIP Doesn't match\n");
		reply.rep = 0;
		//if( write(sockfd,(char*)&reply,sizeof(struct details)) < 0 )
		//perror("\nCould not send ARP Reply properly!\n");
		close(sockfd);
		return 0;
	}

	reply.rep = 1;

	strcpy(reply.destmac,mac);

	if( write(sockfd,(char*)&reply,sizeof(struct details)) < 0 )
	perror("\nCould not send ARP Reply properly!\n");

	else
	printf("\nARP Reply sent : %s | %s | %s | %s\n",reply.srcip,reply.srcmac,reply.destip,reply.destmac);

	if( read(sockfd,(char*)&recv,sizeof(struct details)) < 0 )
	printf("\nCould not receive packet from the server !\n");

	else
	printf("Received packet is : %s | %s |\n %s | %s | %s",recv.srcip,recv.srcmac,recv.destip,recv.destmac,recv.data);

	close(sockfd);

	printf("\n");

	return 0;
}
