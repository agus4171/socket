socket
======

socket HTTP downloader


//##################################################//
//## 		HTTP Client			  ##//
//## 		By Kelompok 9			  ##//
//## 	5112100036/5112100049/5112100055	  ##//
//##################################################//

#include<stdio.h>
#include<arpa/inet.h>
#include<string.h>
#include <fcntl.h>

int main()
{
	int sockcli, fd;
	struct sockaddr_in servaddr;
	char *msg = "GET / HTTP/1.0\r\nHOST:its.ac.id\r\n\r\n";
	char buf[1024];

	sockcli = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockcli == -1)
    	{
        	printf("Could not create socket");
    	}

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(80);
	inet_aton("202.46.129.20", &servaddr.sin_addr);

	int retval = connect(sockcli, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if (retval < 0)
    	{
        	printf("Connect error");
		return 1;
    	}
	
	if(write(sockcli, msg, strlen(msg)) < 0)	
	{
        	printf("Send failed");
        	return 1;
    	}
	int readbuf, flag=0, huruf=0, flaghuruf=0, i=0;
	fd = open("file.txt", O_RDWR | O_APPEND | O_CREAT,S_IWUSR | S_IRUSR);
	char getimg[102400];
	
	while(retval = read(sockcli, buf, 1024))
	{
		//int flag;
		//if (buf[0]=='\r'||buf[0]=='\n') flag++;
		//else flag=0;
		//if (flag==4) break;
		buf[retval]='\0';
		for(i=0; i<retval; i++)
		{
			if(buf[i]=='<' && buf[i+1]=='i' && buf[i+2]=='m' && buf[i+3]=='g')
				{
					flag=1;
					
				}
				else if(buf[i]=='<')
				{
					if(flag){
						char getbuf[2];
						getbuf[0]='\n';
						getbuf[1]='\0';		
						write(fd, getbuf, strlen(getbuf));
					}
					flag=0;
				}
				if(flag==1){
					char getbuf[2];
					getbuf[0]=buf[i];
					getbuf[1]='\0';
					write(fd, getbuf, strlen(getbuf));			
					
					if(buf[i-5]=='s' && buf[i-4]=='r' && buf[i-3]=='c' && buf[i-2]=='=' && buf[i-1]=='\"'){
						huruf=0;
						getimg[huruf]=buf[i-1];
						flaghuruf=1;
					}
					
					if(flaghuruf){
						if(buf[i]=='\"'){
							huruf++;
							getimg[huruf]='\"';
							getimg[huruf+1]='\0';
							flaghuruf=0;
							printf("%s\n",getimg);
						}
						else {
							huruf++;
							getimg[huruf]=buf[i];
								
						}
					}			
				}
		}

		write (fd, buf, strlen(buf));
		//printf("%s", buf);
	}

	close(sockcli);
	return 0;
}
