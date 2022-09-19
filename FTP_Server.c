#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

void main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("No port given\n");
        exit(0);
    }
    int sockfd, newsockfd, portno, clilen;
    char buffer[1024];
    struct sockaddr_in serv_addr, cli_addr;
    portno = atoi(argv[1]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    
    //FTP section

    int command = 1;
    char strcommand[10];
    char filename[40];
    FILE *fp;
    while (command != 3)
    {
        bzero(strcommand, 10);
        recv(newsockfd, strcommand, 10, 0);
        command = atoi(strcommand);
        if(command == 1)
        {
            bzero(filename, 40);
            recv(newsockfd, filename, 40, 0);
            fp = fopen(filename, "r");
            if(fp == NULL)
                send(newsockfd, "NULL", 4, 0);
            else
            {
                bzero(buffer,1024);
                fgets(buffer, 1024, fp);
                fclose(fp);
                send(newsockfd, buffer,sizeof(buffer), 0);
            }
        }
        else if(command == 2)
        {
            bzero(filename, 40);
            recv(newsockfd, filename, 40, 0);
            if(strcmp(filename, "NULL")!=0)
            {
                bzero(buffer,1024);
                recv(newsockfd, buffer, 1024, 0);
                fp = fopen(filename, "w");
                fputs(buffer, fp);
                fclose(fp);
            }
        }
    }
}
