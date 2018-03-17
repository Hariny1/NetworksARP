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

void input(char string[20])
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
	int sockfd,i,n;
	int newfd[5];
	int flag=0;

	struct details req,reply,send;

	char buffer[10];

	struct sockaddr_in server_sockad,client_sockad;

	bzero(&server_sockad,sizeof(server_sockad));

	server_sockad.sin_family = AF_INET;
	server_sockad.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_sockad.sin_port = htons(6770);

	int clientlen = sizeof(client_sockad);

	if ( (sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0 )
	perror("\nSocket Error!");

	if ( bind(sockfd,(struct sockaddr*)&server_sockad, sizeof(server_sockad)) < 0 )
	perror("\nBind Error");

	if ( listen(sockfd,5) < 0 )
	perror("\nListen Error");

	printf("\nType the number of clients : ");
	scanf("%d",&n);
//newfd[j]=accept(sockfd,(struct sockaddr*)&cliaddr,&len);//newfd[]
//j++;
//for(i=0;i<10;i++)
//{
	//newfd=accept(sockfd,(struct sockaddr*)&cliaddr,&len);//newfd[]
	//perror("Can't accept");

//}

	for(i=0;i<n;i++)
	{
		if( (newfd[i] = accept(sockfd,(struct sockaddr*)&client_sockad, &clientlen)) < 0 )
		perror("\nCan't Accept");
	}

	//printf("\nEnter the details of packet received.\n");

	printf("\nDestination IP : ");
	input(req.destip);

	printf("Source IP : ");
	input(req.srcip);

	printf("Source MAC : ");
	scanf("%s",req.srcmac);

	printf("\n16 bit Data : ");
	scanf("%s",send.data);

	strcpy(send.destip,req.destip);
	strcpy(send.srcip,req.srcip);
	strcpy(send.srcmac,req.srcmac);
/*n=read(i,buff,sizeof(buff));
	printf("\nReceived Message is %s",buff);//
	n=write(i,buff,sizeof(buff));*/
//close(sockfd);//
//close(newfd);
	printf("\nDeveloping ARP Request packet\n");
	printf("%s | %s | %s",req.srcip,req.srcmac,req.destip);

	for(i=0;i<n;i++)
	{
		if( write(newfd[i],(char*)&req,sizeof(struct details)) < 0 )
		perror("\nARP request not sent properly!\n");
	}

	printf("\nThe ARP Request packet is being sent.\n");

	for(i=0;i<n;i++)
	{
		if( read(newfd[i],(char*)&reply,sizeof(struct details)) < 0 )
		perror("\nCould not read ARP Reply properly!\n");

		if(reply.rep == 1)
		{
			flag=1;
			break;
		}
	}

	if(flag==0)
	{
		printf("\nDidn't receive any ARP reply from the clients !");

		close(sockfd);

		for(i=0;i<2;i++)
		close(newfd[i]);

		return 0;
	}

	printf("\nARP Reply received %s | %s | %s | %s\n",reply.srcip,reply.srcmac,reply.destip,reply.destmac);

	printf("\nSending the packet to : %s\n",reply.destmac);

	strcpy(send.destmac,reply.destmac);

	if( write(newfd[i],(char*)&send,sizeof(struct details)) < 0 )
	printf("\nCould not send packet to the client !\n");

	else
	printf("Packet sent : %s | %s |\n %s | %s | %s",send.srcip,send.srcmac,send.destip,send.destmac,send.data);
/*n=read(i,buff,sizeof(buff));
	printf("\nReceived Message is %s",buff);//
	n=write(i,buff,sizeof(buff));*/
//close(sockfd);//
//close(newfd);
	close(sockfd);

	for(i=0;i<n;i++)
	close(newfd[i]);

	printf("\n");

	return 0;
}
