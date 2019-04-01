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

void dec_to_bin(int dec,int bin[8])
{
	int i,t;

	for(i=0;i<8;i++)
	bin[i]=0;
	
	i=0;
	
	//printf("\nDecimal is : %d",dec);
	
	while(dec>=2)
	{
		bin[i]=dec%2;
		dec/=2;
		i++;
	}

	bin[i]=dec;

	//printf("\nBinary is : ");
	
	//for(i=0;i<8;i++)
	//printf("%d ",bin[i]);

	//printf("\n");
}

int bin_to_dec(int bin[8])
{
	int pow=1,sum=0,i;

	//printf("\nBinary is : ");
	
	for(i=0;i<8;i++)
	{
		sum+=pow*bin[i];
		pow*=2;
		//printf("%d ",bin[i]);
	}

	//printf("\nDecimal is : %d\n",sum);
	
	return sum;
}

void calcsubnet(int subnet[4],int class,int nosub)
{
	int pow=1;
	int i,j,k,l,r;
	int subnet_bin[4][8];
	
	for(i=0;i<4;i++)
	for(j=0;j<8;j++)
	subnet_bin[i][j]=0;
	
	for(i=0;i<class;i++)
	{
		subnet[i]=255;
		dec_to_bin(subnet[i],subnet_bin[i]);	
	}
	j=0;
	while(j>=0)
	{
		if(pow<nosub)
		{		
			pow*=2;
			j++;
		}
		else
		break;
	}
	
	r=j;
	
	for(k=i,j=0;k<4&&j<r;k++)
	{
		
		for(l=7;l>=0&&j<r;l--,j++)
		{
			subnet_bin[k][l]=1;
		}

	}
	
	for(i=0;i<4;i++)
	{
		subnet[i] = bin_to_dec(subnet_bin[i]);
	}

}

void calcsubnetad(int destip[4],int subnet[4],int subnetad[4])
{
	int i,j;
	
	int destip_bin[4][8];

	int subnetad_bin[4][8];

	int subnet_bin[4][8];

	for(i=0;i<4;i++)
	for(j=0;j<8;j++)
	{
		subnetad_bin[i][j]=destip_bin[i][j]=subnet_bin[i][j]=0;
	}

	for(i=0;i<4;i++)
	dec_to_bin(destip[i],destip_bin[i]);

	for(i=0;i<4;i++)
	dec_to_bin(subnet[i],subnet_bin[i]);

	for(i=0;i<4;i++)
	for(j=0;j<8;j++)
	if(subnet_bin[i][j]==1 && destip_bin[i][j]==1)
	subnetad_bin[i][j]=1;

	for(i=0;i<4;i++)
	subnetad[i]=bin_to_dec(subnetad_bin[i]);
	
}

int ValIP(int ip[4])
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
			printf("Invalid Address!");
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
			printf("Invalid Address!");
		}

	}while(flag==0);

	if(ip[0]>=0&&ip[0]<=127)
	return 1;
	
	else if(ip[0]>=128&&ip[0]<=191)
	return 2;	
	
	else if(ip[0]>=192&&ip[0]<=223)
	return 3;

	else
	return 0;
}

int match(int ip[4],int subnetad[4])
{
	int i;
	
	for(i=0;i<4;i++)
	if(ip[i]!=subnetad[i])
	return 0;
	
	return 1;
}

int main()
{
	int netip[4],destip[4],subnetad[4],ip[4],i,j;
	int nosub,netclass,subclass;
	int subnet[4];
	
	struct sdata p,temp;	

	int server_socketfd;
	int new_socketfd[20];
	int flag=0;
		
	char userdata[20];

	struct sockaddr_in servaddr,cliaddr;

	bzero(&servaddr,sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(8889);

	int clientlen = sizeof(cliaddr);

	printf("\nEnter the network address : ");
	netclass = ValIP(netip);

	//printf("\nClass %d\n",netclass);
		
	printf("\nEnter the number of subnets : ");	
	scanf("%d",&nosub);

	printf("\nCalculating Subnet Mask...\n");

	calcsubnet(subnet,netclass,nosub);

	printf("\nSubnet Mask is : ");

	for(i=0;i<4;i++)
	printf("%d.",subnet[i]);
	
	printf("\b \n");


	if ( (server_socketfd = socket(AF_INET,SOCK_STREAM,0)) < 0 )
	perror("\nSocket Error!");

	if ( bind(server_socketfd,(struct sockaddr*)&servaddr, sizeof(servaddr)) < 0 )
	perror("\nBind Error");

	if ( listen(server_socketfd,5) < 0 )
	perror("\nListen Error");

	printf("\nListening\n");
	
	for(i=0;i<nosub;i++)
	{
		if( (new_socketfd[i] = accept(server_socketfd,(struct sockaddr*)&cliaddr, &clientlen)) < 0 )
		perror("\nCan't Accept");
	}
	
	printf("\nAll clients accepted\n");

	printf("\nEnter the destination IP address : ");
	subclass = ValIP(destip);

	for(i=0;i<4;i++)
	p.ip[i]=destip[i];

	printf("\nEnter the 16 bit data : ");
	scanf("%s",userdata);

	strcpy(p.buffer,userdata);
	
	if(netclass!=subclass)
	printf("\nDifferent Classes!");

	printf("\nCalculating Subnet Address...\n");

	calcsubnetad(destip,subnet,subnetad);

	printf("\nThe subnet address is : ");

	for(i=0;i<4;i++)
	printf("%d.",subnetad[i]);
	
	printf("\b \n");
	
	strcpy(temp.buffer,"no");

	printf("\nTransmitting packet to : ");

	for(i=0;i<4;i++)
	printf("%d.",subnetad[i]);
	
	printf("\b \n\n");

	for(i=0;i<nosub;i++)
	{
		if( read(new_socketfd[i],(char*)ip,sizeof(ip)) < 0 )
		perror("\nRead Error");

		if(match(ip,subnetad)==1)
		{
			if( write(new_socketfd[i],(char*)&p,sizeof(struct sdata)) < 0 )
			perror("\nWrite Error");
			
			flag=1;
			
			for(j=0;j<4;j++)
			printf("%d.",destip[j]);
	
			printf("\b || ");			
			
			printf("%s\n",userdata);
			
			break;
		}

		else
		{
			if( write(new_socketfd[i],(char*)&temp,sizeof(struct sdata)) < 0 )
			perror("\nWrite Error");
		}
		
	}

	for(;i<nosub;i++)
	if( write(new_socketfd[i],(char*)&temp,sizeof(struct sdata)) < 0 )
	perror("\nWrite Error");	
	
	if(flag==0)
	printf("\nData not sent to the destination IP!\n");

	else
	printf("\nData sent to the destination IP!\n");
	close(server_socketfd);

	for(i=0;i<nosub;i++)
	close(new_socketfd[i]);

	return 0;
}
