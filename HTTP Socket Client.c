//##################################################//
//## 		              HTTP Client			            ##//
//## 		              By Kelompok 9			          ##//
//## 	    5112100036/5112100049/5112100055	      ##//
//##################################################//

#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int sockcli, fd, i, ffd, cntntlgth;
	struct sockaddr_in servaddr;
	char buf[2], address[512], filetodl[512], filename[125], msg[512];
	
	bzero (address, sizeof address);
	bzero (filetodl, sizeof filetodl);
	bzero (filename, sizeof filename);
	bzero (filename, sizeof msg);
	sockcli = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockcli < 0)
    	{
        	perror(strerror(errno));
		return errno;
	}	
		
	if (argc < 2)
	{
		puts("Please enter an address\n");
		return -1;
	}    	
	
	else if (argc < 2)
	{
		puts ("Too many arguments!\n");
	}
	
	sscanf(argv[1], "%[^/]%s", address, filetodl);
	if (strlen(filetodl)==1)
	{
		strcat (filename, "index.html");
	}
	else 
	{
		for (i=0; i< strlen(filetodl)-1; i++)
		{
			filename[i] = filetodl[i+1];
		}
	}

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(80);
	inet_aton(address, &servaddr.sin_addr);

	sprintf (msg,"GET %s HTTP/1.1\r\nHOST:%s\r\n\r\n", filetodl, address );


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
 
	int flag =0;
	int done =0;
	
	ffd = open(filename, O_RDWR | O_APPEND | O_CREAT, S_IWUSR | S_IRUSR);
		
	while(retval = recv(sockcli, buf, (sizeof buf)-1, 0))
	{
		if (buf[0]=='\r' && flag==0 && done==0) flag++;
		else if (buf[0]=='\n' && flag==1 && done==0) flag++;
		else if (buf[0]=='\r' && flag==2 && done==0) flag++;
		else if (buf[0]=='\n' && flag==3 && done==0) flag++;
		else flag = 0;
		buf[retval]= 0 ;
		if(done==0) printf ("%s", buf);
		if(done==1) write (ffd, buf, 1);
		if(flag==4) done=1;
	}
	close(filename);
	close(sockcli);
	return 0;
}

