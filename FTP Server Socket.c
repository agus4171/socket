//######################################################//
// 			Title : FTP Server Socket in c 				//
// 					Programmer : 						//
//				I Made Agus Adi Wirawan					//
//					5112100036							//
//######################################################//

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char * argv[])
{
	int sd, sd_data, cli, bd, clilen, addrlen, port;
	char sbuffer[80], rbuffer[80], user[80], pass[80];
	int n, bytes;
	struct sockaddr_in servaddr, cliaddr;
	
	printf("Enter the port address :\n");
	scanf("%d", &port);
	sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sd < 0)
		printf("Can't create socket\n");
	else
		printf("Socket is created\n");
	
	memset(&servaddr, 0 , sizeof(servaddr));
	memset(&cliaddr, 0 , sizeof(cliaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);
	
	bd = bind(sd,(struct sockaddr *)&servaddr, sizeof(servaddr));
	if(bd < 0)
		printf("Cant bind\n");
	else
		printf("Binded\n");

	listen(sd,5);
	while (1)
	{	
		clilen = sizeof(cliaddr);
		sd_data = accept(sd,(struct sockaddr *)&cliaddr, &clilen);
		if(sd_data < 0)
		{
			printf("Cant accept\n");
		}
		else
		{
			printf("Accepted\n");
			printf("Connected to %s", inet_ntoa(cliaddr.sin_addr));
			
			sprintf(sbuffer,"200 Welcome to the jungle\r\n");
				write(sd_data, sbuffer, strlen(sbuffer));
			while (1)
			{
				n = 0;
				memset(rbuffer, 0, sizeof(rbuffer));
				bytes = read(sd_data, &rbuffer[n], sizeof(rbuffer));
				printf("Command : %s\n", rbuffer);
				
				if (strncmp(rbuffer,"USER",4)==0)
				{
					sscanf(user, "USER %s", rbuffer);
					if (strcmp(user, "agus")==0)
						{
							sprintf(sbuffer, "331 Password required\r\n");
							write(sd_data, sbuffer, strlen(sbuffer));
						}
				}
								
				if (strncmp(rbuffer,"PASS",4)==0)
				{
					sscanf(pass, "PASS %s", rbuffer);
					if (strcmp(pass, "agus")==0)
						{
							sprintf(sbuffer, "230 Successfully login\r\n");
							write(sd_data, sbuffer, strlen(sbuffer));
						}
					else
						{
							sprintf(sbuffer, "430 Invalid Password\r\n");
							write(sd_data, sbuffer, strlen(sbuffer));
						}
				}
								
				if (strncmp(rbuffer,"SYST",4)==0)
				{
					sprintf(sbuffer, "257 System is turn on\r\n");
					write(sd_data, sbuffer, strlen(sbuffer));
				}
				
				if (strncmp(rbuffer,"QUIT",3)==0)
				{
					sprintf(sbuffer, "221 Bye bye\n");
					write(sd_data, sbuffer, strlen(sbuffer));
					close(sd_data);
					printf("Disconnected from client %s\n",inet_ntoa(cliaddr.sin_addr));
					return 0;
				}
			}
		}	
	}
	close (sd);
}
