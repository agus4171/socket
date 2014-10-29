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
					printf("Logging in\n");
					sprintf(sbuffer, "331 Password required\r\n");
					write(sd_data, sbuffer, strlen(sbuffer));
				}
				
				if (strncmp(rbuffer,"PASS",4)==0)
				{
					sscanf(pass, "PASS %s", rbuffer);
					if (strcmp(pass, "agus")==0)
					printf("Logging in\n");
					sprintf(sbuffer, "230 Successfully login\r\n");
					write(sd_data, sbuffer, strlen(sbuffer));
				}
								
				if (strncmp(rbuffer,"SYST",4)==0)
				{
					printf("Information about the system\n");
					sprintf(sbuffer, "257 A very naive FTP system\r\n");
					write(sd_data, sbuffer, strlen(sbuffer));
				}
				
				/*if (strncmp(rbuffer,"PASV",4)==0)
				{
					printf("Passive mode\n");
					sprintf(sbuffer, "227 Passive mode (%d,%d,%d,%d,%d,%d)\r\n", 127,0,0,1(1220>>8), (1220 & 0x00FF));
					bytes = send(newsd, sbuffer, strlen(sbuffer), 0);
					//newsd_data = accept(s_data)
				}*/
				
				if (strncmp(rbuffer,"QUIT",3)==0)
				{
					printf("Quit\n");
					sprintf(sbuffer, "221 Bye bye\n");
					write(sd_data, sbuffer, strlen(sbuffer));
					close(sd_data);
					//printf("Disconnect %s", inet_ntoa(cliaddr.sin_addr));
					return 0;
				}
			}

		close(sd_data);
		}	
	}
	close (sd);
}
