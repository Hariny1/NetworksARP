#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>

int main(int argc,char **argv)
{
	int sockfd,confd;
	int n,c=-1,i,j,k=0,m,f,pos,res,cnt,wz,a[40],b[10][10],temp,z,tmp;	

	struct sockaddr_in servaddr,cliaddr;
	struct timeval timeout;
	fd_set m_set;
	
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
		perror("\nCannot Create Socket!");

	bzero(&servaddr,sizeof(servaddr));	

	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=inet_addr(argv[1]);
	servaddr.sin_port=htons(atoi(argv[2]));

	confd=connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	if(confd<0)
		perror("\nCannot Establish Connection!");
		
	printf("\nEnter n: ");
	scanf("%d",&n);
	printf("\nEnter data: ");
	for(i=0;i<n;i++)
		scanf("%d",&a[i]);
	
	printf("\nEnter Frame & Window Size: ");
	scanf("%d%d",&f,&wz);
	temp=wz;
	
	write(sockfd,&f,sizeof(int));
	res=n/f;
	write(sockfd,&res,sizeof(int));

	for(i=0;i<res;i++)
	{
		cnt=0;
		printf("\nFrame-%d\t",i);
		for(j=0;j<f;j++,k++)
		{
			b[i][j]=a[k];
			printf(" %d",b[i][j]);
			if(b[i][j]==1)
				cnt++;
		}
		if(cnt%2==0)
			b[i][j]=0;
		else
			b[i][j]=1;
		
		b[i][++j]=i;
		printf("--%d",b[i][j]);
	}
	tmp=c+1;

	label:	
		printf("\n");
		for(i=c+1;i<wz && i!=res;i++)
		{
			if(tmp==i)
			{
				printf("\n\n");
				for(z=0;z<res;z++)
				{
					if(z==i)
						printf("||");
					printf("%d\t",z);
					if(z==wz-1)
						printf("||");
				}
			}
			printf("\n\n");
			printf("Sending Frame-%d\t",i);
			for(m=0;m<=f;m++)
			{
				if(m==f)
					printf("---");
				printf(" %d",b[i][m]);
			}
			printf("\nWish 2 Introduce Error(0/1)...");
			scanf("%d",&k);
			if(k==1)
			{
				pos=0;
				b[i][pos]=b[i][pos]==0 ? 1 : 0;
				write(sockfd,b[i],4*(f+1));
				b[i][pos]=b[i][pos]==0 ? 1 : 0;
			}
			else
				write(sockfd,b[i],4*(f+2));
			sleep(1);
		}	
		FD_ZERO(&m_set);
		FD_SET(sockfd,&m_set);//
		timeout.tv_sec=5;
		timeout.tv_usec=0;
		printf("\n");
		while(select(sockfd+1,&m_set,NULL,NULL,&timeout) > 0 && FD_ISSET(sockfd,&m_set))//
		{
			if(read(sockfd,&c,sizeof(c)) > 0)
			{
				sleep(1);
				printf("Receiving ACK%d...\n",c);
				wz=wz+1<(res+temp)?wz+1:wz;

				FD_ZERO(&m_set);
				FD_SET(sockfd,&m_set);
				timeout.tv_sec=3;
				timeout.tv_usec=0;
			}
			else
				break;
		}
		if(c<res-1)
		{ 
			goto label;
		}

	return 0;
}	
