#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "Ping.h"

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    /* Variable declaration */
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    ping_t testping;
        char descriptorID[] = "Test Ping";

    /* Usage check */
    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    /* Open socket */
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    /* Connect to remote host */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    /* Message */
    strncpy(testping.desc.descriptorID, descriptorID, 10);
    testping.desc.payloadDescriptor = PING;
    testping.desc.TTL = 1;
    testping.desc.hops = 0;
    testping.desc.payloadLength = 0;
    printf("descriptorID: %s\n", testping.desc.descriptorID);
    printf("payloadDescriptor: %d\n", testping.desc.payloadDescriptor);
    printf("TTL: %d\n", testping.desc.TTL);
    printf("Hops: %d\n", testping.desc.hops);

    /* Write to socket */
    n = write(sockfd,&testping,sizeof(testping));
    if (n < 0) 
         error("ERROR writing to socket");

    /* Read from socket */
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    close(sockfd);
    return 0;
}
