/* 
   A simple server in the internet domain using TCP
   Usage:./server port (E.g. ./server 10000 )
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>   // definitions of a number of data types used in socket.h and netinet/in.h
#include <sys/socket.h>  // definitions of structures needed for sockets, e.g. sockaddr
#include <netinet/in.h>  // constants and structures needed for internet domain addresses, e.g. sockaddr_in
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> // open 
#include <fcntl.h>
#define BUFSIZE 1024
#define MAXFILENAME 30
#define HEADER_LEN 30
#define MAXLEN 1024

void error(char *msg)
{
    perror(msg);
    exit(1);
}

/* find file type function */
char *find_content_type (char *filename) {
    char *p;  // pointer to the type found
    int i;
    char buf1[MAXFILENAME]; // used to store the extension of the file
    char buf2[MAXFILENAME];

    p = (char *)malloc(30);
    strcpy(buf1, filename);
    printf("name of file requested: %s \n", buf1);

    /* find the extension: */
    for (i = 0; i<strlen(buf1); i++) {
        if ( buf1[i] == '.' ) {
            strcpy(buf2, &buf1[i]);
        }
    }
    /* find the type: */
    if ( strcmp(buf2, ".html") == 0 || strcmp (buf2, ".hml") == 0) {
        strcpy (buf2, "Content-Type: text/html \r\n");
    }

    else if ( strcmp(buf2, ".txt") == 0) {
        strcpy (buf2, "Content-Type: text/plain \r\n");
    }

    else if ( strcmp(buf2, ".jpg") == 0 || strcmp (buf2, ".jpeg") == 0) {
        strcpy (buf2, "Content-Type: image/jpeg \r\n");
    }

    else if ( strcmp(buf2, ".gif") == 0) {
        strcpy (buf2, "Content-Type: image/gif \r\n");
    }

		else if ( strcmp(buf2, ".png") == 0) {
        strcpy (buf2, "Content-Type: image/png \r\n");
    }
		
		else if ( strcmp(buf2, ".mp3") == 0) {
        strcpy (buf2, "Content-Type: audio/mpeg \r\n");
    }

    else {
        strcpy (buf2, "Content-Type: application/octet-stream \r\n");
    }

     p = buf2;
    printf ("%s\n", p);
    //return "Content-type: image/jpeg\r\n";
    return p;
}

/* response generator function */

void response_generator (int conn_fd, char *filename) {

    /* vars needed for finding the length of the file */
    struct stat filestat;
    FILE *fp;
    int fd;
    int fsize;
    char header_buff [HEADER_LEN];
    char* file_buff;
    char filesize[7];//, name[30]; 

    if ( ((fd = open (filename, O_RDONLY)) < -1) || (fstat(fd, &filestat) < 0) ) {
        printf ("Error in measuring the size of the file\n");
}

    if (filename == NULL) {
        // I have measured the length of my 400.html file
        strcpy (header_buff, "HTTP/1.1 400 Bad Request\r\nContent-Length: 327\r\nContent-Type: text/html\r\n");
        fp = fopen ("400index.html", "r");
    }

    sprintf (filesize, "%zd", filestat.st_size); // put the file size of buffer, so we can add it to the response header

    fp = fopen (filename, "rb");
    if (fp == NULL) {
    printf ("fp is null or filename = 404\n");
        // I have measured the length of my 404.html file
        strcpy (header_buff, "HTTP/1.1 404 Not Found\r\nContent-Length: 165\r\nContent-Type: text/html\r\n");
        fp = fopen ("404index.html", "r");
    }

    else if (fp != NULL) {
        strcpy (header_buff, "HTTP/1.1 200 OK\r\nContent-Length: ");
        /* content-length: */
        strcat (header_buff, filesize);
        strcat (header_buff, "\r\n");
        /* content-type: */
        strcat (header_buff, find_content_type (filename));
        //printf ("%s\n", find_content_type (filename));
    }

    else {
        // I have measured the length of my 500.html file
        strcpy (header_buff, "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 190\r\nContent-Type: text/html\r\n");
        fp = fopen ("500index.html", "r");
    }        

    strcat (header_buff, "Connection: keep-alive\r\n\r\n");
    
    if(fp != NULL) // find file
    {
	    	fseek(fp, 0, SEEK_END); 
			fsize = ftell(fp); 
			fseek(fp, 0, SEEK_SET);
		

		file_buff = (char*)malloc(fsize);

    write (conn_fd, header_buff, strlen(header_buff));

    fread (file_buff, sizeof(char), fsize, fp);
    fclose(fp);

    write (conn_fd, file_buff, fsize);
		free(file_buff);
    close (conn_fd);
		}

		else // not fopund file
		{
			 write (conn_fd, header_buff, strlen(header_buff));
			 close (conn_fd);
		}
}




int main(int argc, char *argv[])

{
     int sockfd, newsockfd; //descriptors rturn from socket and accept system calls
     int portno; // port number
     socklen_t clilen;
     char buf[BUFSIZE];
		 char * filebuf;
			int len;
		 FILE * fp = NULL;
     char buffer[BUFSIZE];
     
     /*sockaddr_in: Structure Containing an Internet Address*/
     struct sockaddr_in serv_addr, cli_addr;
     
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     
     /*Create a new socket
       AF_INET: Address Domain is Internet 
       SOCK_STREAM: Socket Type is STREAM Socket */
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]); //atoi converts from String to Integer
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY; //for the server the IP address is always the address that the server is running on
     serv_addr.sin_port = htons(portno); //convert from host to network byte order
     
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) //Bind the socket to the server address
              error("ERROR on binding");
 
     listen(sockfd,5); // Listen for socket connections. Backlog queue (connections to wait) is 5
     
     clilen = sizeof(cli_addr);
     /*accept function: 
       1) Block until a new connection is established
       2) the new socket descriptor will be used for subsequent communication with the newly connected client.
     */

while(1){

     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,BUFSIZE);
     n = read(newsockfd,buffer,BUFSIZE-1); //Read is a block function. It will read at most 255 bytes

     if (n < 0) error("ERROR reading from socket");
        
		 printf("%s\n",buffer);

     strcpy(buf,buffer);
		
  char * ptr = strtok(buf," ");

			for(int i=0;i<1;i++)
					{
							ptr=strtok(NULL, " ");
					}

			ptr+=1;

			if(strcmp(ptr,"favicon.ico") != 0) // clome favicon http request
			{
				response_generator(newsockfd,ptr);
			}
/*
			if((fp = fopen(ptr,"rb")) == NULL)
					{
							printf("File open error\n");
							exit(1);
						}

				fseek(fp, 0, SEEK_END); 
				int fsize = ftell(fp); 
				fseek(fp, 0, SEEK_SET);

				printf("%i\n",fsize);

 				filebuf = (char*)malloc(fsize);

				while((len=fread(filebuf, sizeof(char), fsize, fp)) != 0 )
						{
									write(newsockfd, filebuf, len);
						}

     // n = write(newsockfd,"I got your message",18); //NOTE: write function returns the number of bytes actually sent out Ñ> this might be less than the number you told it to send
     //if (n < 0) error("ERROR writing to socket");

			close(newsockfd);
			free(filebuf);
*/
}
     
     close(sockfd);
     
     return 0; 
}




