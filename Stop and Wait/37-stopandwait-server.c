#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<math.h>

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

int main(int argc,char **argv)
{
	struct ackpak ack;
	int fs=0,num=0,frasize;
	printf("Enter frame size\n");
	scanf("%d",&frasize);

	printf("Enter the number of bits ");
	scanf("%d",&num);
	int userdata[num+100];

	int i,j,k,pos;
	int cho;
	int noofone;

	struct frame temp,f[(int)((num/frasize)+1)];

	int sd,cli_sockaddlen,m,n;
	int server_sockaddlen;

	struct sockaddr_in server_sockadd,cli_sockadd;
	char buffer[20];
	int cNum=htonl(num);
	int fra=htonl(frasize);
	//sd = socket(AF_INET,SOCK_DGRAM,0);
	sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd<0)
	perror("Could not create socket ");

	server_sockaddlen = sizeof(server_sockadd);

	bzero(&server_sockadd,server_sockaddlen);

	server_sockadd.sin_family = AF_INET;
	server_sockadd.sin_port = htons(7714);
	server_sockadd.sin_addr.s_addr = inet_addr("127.0.0.1");



	connect(sd,(struct sockaddr*)&server_sockadd,sizeof(server_sockadd));

	write(sd,&cNum,sizeof(cNum),0);
	write(sd,&fra,sizeof(fra),0);

	printf("\nEnter %d data : ",num);

	for(i=0;i<num;i++)
	{
		scanf("%d",&userdata[i]);
		//pos=i;
	}
	if((int)((num/frasize)+1)!=num/frasize)
	{
		num=num+1;
		for(i=num;i<num+((int)((num/frasize)+1)-num/frasize);i++)
			userdata[i]=0;
	}

	/*for(i=0;i<num+(int)((num/frasize)+1)-1;i++) //output data
	{
		printf("%d",userdata[i]);
	}*/

	for(i=0,k=0;i<(int)(num/frasize);i++)
	{
		f[i].seqno = i%2;

		noofone=0;

		for(j=0;j<frasize;j++,k++)
		{
			f[i].d[j] = userdata[k];

			if(userdata[k]==1)
			noofone++;
		}


		f[i].d[j]=noofone%2;
	}

     fs=0;

     while(fs!=(int)(num/frasize))
     {
	temp = f[fs];

	printf("\n\nFrame %d - ",temp.seqno);

	for(j=0;j<frasize;j++)
	printf("%d",temp.d[j]);

	printf(" - %d\n",temp.d[j]);

	printf("\nWant to introduce an error ? ");
	scanf("%d",&cho);

	if(cho==1)
	{
		printf("Introduce error in position : ");
		scanf("%d",&pos);

		temp.d[pos-1]=(!temp.d[pos-1]);
	}

	fflush(stdin);

	printf("\n\nSending Frame %d - ",f[fs].seqno);

	for(j=0;j<frasize;j++)
	printf("%d",temp.d[j]);

	printf(" - %d\n",temp.d[j]);

	m = sendto(sd,(char*)&temp,sizeof(struct frame),0,(struct sockaddr*)&server_sockadd,server_sockaddlen);

	n = recvfrom(sd,(char*)&ack,sizeof(struct ackpak),0,(struct sockaddr*)&server_sockadd,&server_sockaddlen);

	printf("ACK = %d, SEQNO = %d\n",ack.ackn,ack.frameno);

	if(ack.timeout==1)
	printf("\n\nTime Out. Resending Frame %d",temp.seqno);

	else if(ack.ackn==1)
	{

		fs++;
		printf("\nFrames sent : %d",fs);
	}

	else if(ack.ackn==0)
	printf("\n\nNACK Received. Resending Frame %d %d",temp.seqno,ack.ackn);

     }

	printf("\n");


	close(sd);

	return 0;
}
