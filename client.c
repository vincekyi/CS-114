#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "Ping.h"
#include "Pong.h"

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
    pong_t responsepong;

    /* Usage check */
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
    n = write(sockfd,&testping,sizeof(ping_t));
    if (n < 0) 
         error("ERROR writing to socket");

    /* Read from socket */
    n = read(sockfd,&responsepong,sizeof(pong_t));
    if (n < 0) 
         error("ERROR reading from socket");
    printf("\nPong received\n");
    printf("descriptorID: %s\n", responsepong.desc.descriptorID);
    printf("payloadDescriptor: %d\n", responsepong.desc.payloadDescriptor);
    printf("TTL: %d\n", responsepong.desc.TTL);
    printf("Hops: %d\n", responsepong.desc.hops);
    printf("Port: %d\n", responsepong.port);
    printf("IP: %d\n", responsepong.ipaddress);
    printf("Files: %d\n", responsepong.numFiles);
    printf("Kilobytes: %d\n", responsepong.numKilobytes);
    close(sockfd);
    return 0;
}
