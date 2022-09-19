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
    int sockfd, portno, clilen;
    char buffer[1024];
    struct sockaddr_in serv_addr;
    portno = atoi(argv[1]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    
    //FTP section

    int  command = 1;
    char strcommand[10];
    char filename[40];
    FILE *fp;
    
    while (command != 3)
    {
        printf("Enter 1 for GET request 2 for PUT request 3 to end program\n");
        scanf("%d",&command);
        sprintf(strcommand, "%d", command);
        send(sockfd, strcommand, sizeof(strcommand), 0);
        if(command == 1)
        {
            printf("Enter filename\n");
            scanf("%s",&filename);
            send(sockfd, filename, sizeof(filename), 0);
            bzero(buffer,1024);
            recv(sockfd, buffer, 1024, 0);
            if(strcmp(buffer, "NULL")==0)
                printf("No such a file\n");
            else
            {
                fp = fopen(filename, "w");
                fputs(buffer, fp);
                fclose(fp);
                printf("File downloaded successfully\n");
            }            
        }
        else if(command == 2)
        {
            printf("Enter filename\n");
            scanf("%s",&filename);
            fp = fopen(filename, "r");
            if(fp == NULL)
            {
                send(sockfd, "NULL", 4, 0);
                printf("No such a file\n");
            }
            else
            {
                send(sockfd, filename, sizeof(filename), 0);
                bzero(buffer,1024);
                fgets(buffer, 1024, fp);
                fclose(fp);
                send(sockfd, buffer,sizeof(buffer), 0);
                printf("File uploaded successfully\n");
            }
        }
    }  
}
