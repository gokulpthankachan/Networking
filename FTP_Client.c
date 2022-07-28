#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n, option;
    char ch, filename[10];

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    
    //file transfer protocol

    bzero(buffer,256);
    printf("Enter option \n 1 for get request \n 2 for put request \n");
    fgets(buffer,255,stdin);
    option = atoi(buffer);
    //option writting to socket
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
        error("ERROR writing to socket");
    //GET request processing
    if (option==1)
    {
        //read file name request
        n = read(sockfd,buffer,255);
        if (n < 0)
            error("ERROR reading from socket");
        printf("%s\n",buffer);
        bzero(buffer,256);
        //read file name request end
        // reading file name
        gets(filename);
        strcpy(buffer,filename);
        //reading file name end
        //writting file name to socket
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) 
            error("ERROR writing to socket");
        bzero(buffer,256);
        //writting file name to socket end
        //reading data
        n = read(sockfd, buffer, 255);
        if (n < 0)
            error("ERROR reading from socket");
        puts(buffer);
    }
    else if (option==2)
    {
        printf("Enter name of file\n");
        gets(filename);
        bzero(buffer,256);
        strcpy(buffer,filename);
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) 
            error("ERROR writing to socket");
        bzero(buffer,256);

        n = read(sockfd,buffer,255);
        if (n < 0)
            error("ERROR reading from socket");
        puts(buffer);
        gets(buffer);
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) 
            error("ERROR writing to socket");
    }
    return 0;
}