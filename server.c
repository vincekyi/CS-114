/* A simple server in the internet domain using TCP
The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Ping.h"
#include "Pong.h"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    /* Variable declaration */
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    ping_t receivedping;
    pong_t responsepong;

    /* Usage check */
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    /* Open socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
       error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Bind to socket */
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
             error("ERROR on binding");

    /* listen/accept (blocking call) */
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
                (struct sockaddr *) &cli_addr,
                &clilen);
    if (newsockfd < 0)
         error("ERROR on accept");

    /* Read from socket */
    bzero(buffer,256);
    n = read(newsockfd,&receivedping,sizeof(ping_t));
    if (n < 0) error("ERROR reading from socket");
    printf("Here is the message: \n");
    printf("descriptorID: %s\n", receivedping.desc.descriptorID);
    printf("payloadDescriptor: %d\n", receivedping.desc.payloadDescriptor);
    printf("TTL: %d\n", receivedping.desc.TTL);
    printf("Hops: %d\n", receivedping.desc.hops);
    
    /* Write to socket */
    strncpy(responsepong.desc.descriptorID,receivedping.desc.descriptorID,sizeof(responsepong.desc.descriptorID));
    responsepong.desc.descriptorID[6] = 'o';
    responsepong.desc.payloadDescriptor = PONG;
    responsepong.desc.TTL = 1;
    responsepong.desc.hops = 0;
    responsepong.port = portno;
    responsepong.ipaddress = 0;
    responsepong.numFiles = 1;
    responsepong.numKilobytes = 1;
    n = write(newsockfd,&responsepong,sizeof(pong_t));
    if (n < 0) error("ERROR writing to socket");
    printf("\nPong sent\n");
    printf("descriptorID: %s\n", responsepong.desc.descriptorID);
    printf("payloadDescriptor: %d\n", responsepong.desc.payloadDescriptor);
    printf("TTL: %d\n", responsepong.desc.TTL);
    printf("Hops: %d\n", responsepong.desc.hops);
    printf("Port: %d\n", responsepong.port);
    printf("IP: %d\n", responsepong.ipaddress);
    printf("Files: %d\n", responsepong.numFiles);
    printf("Kilobytes: %d\n", responsepong.numKilobytes);
    close(newsockfd);
    close(sockfd);
    return 0;
}
