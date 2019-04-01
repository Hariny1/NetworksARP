#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

int main(int argc,char **argv)
{
	int sockfd,bindfd,n,newfd,res;
	int b[10],c[10],j,k=0,m,f,cnt,temp=-1;
	
	struct sockaddr_in servaddr,cliaddr;
	struct timeval tout;
	fd_set m_set;
	
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
		perror("\nCannot Create Socket!");

	bzero(&servaddr,sizeof(servaddr));

	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=INADDR_ANY;
	servaddr.sin_port=htons(atoi(argv[1]));

	bindfd=bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	if(bindfd<0)
		perror("\nBind Error!");
	
	listen(sockfd,2);

	n=sizeof(cliaddr);
	newfd=accept(sockfd,(struct sockaddr*)&cliaddr,&n);
	
	n=read(newfd,&f,sizeof(f));
	n=read(newfd,&res,sizeof(res));
	
	label:		
		FD_ZERO(&m_set);
		FD_SET(newfd,&m_set);
		tout.tv_sec=5;
		tout.tv_usec=0;	
		k=temp+1;
		printf("\n");
		while(select(newfd+1,&m_set,NULL,NULL,&tout) > 0 && FD_ISSET(newfd,&m_set))
		{
			if(read(newfd,b,sizeof(b)) > 0)
			{
				sleep(1);
				printf("Receiving Frame-%d\t",k);
				for(m=0;m<=f;m++)
				{
					if(m==f)
						printf("---");
					printf(" %d",b[m]);
				}
				printf("\n");
				cnt=0;
				for(j=0;j<f;j++)
				{
					if(b[j]==1)
						cnt++;
				}
				if(b[j]==cnt%2)
					c[k]=k;
				else
					c[k]=-1;
				k++;
	
				FD_ZERO(&m_set);
				FD_SET(newfd,&m_set);
				tout.tv_sec=3;
				tout.tv_usec=0;
			}
			else
				break;
		}
		printf("\n");
		for(j=temp+1;j<k;j++)
		{
			if(c[j]==j)
			{
				temp=j;
				printf("NO Error!\n\tTransmitting ACK%d...\n\n",temp);
				write(newfd,&temp,sizeof(temp));
				sleep(1);			
			}
			else
			{
				printf("\nError No ACK\n");
				break;
			}
		}
		if(temp<res-1)
		{
			goto label;
		}
	close(sockfd);	
	close(newfd);
	
	return 0;
}
