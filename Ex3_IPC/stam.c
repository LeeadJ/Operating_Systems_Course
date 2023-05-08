#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 6) {
        fprintf(stderr, "Usage: %s -c IP PORT -p TYPE PARAM\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Parse command-line arguments
    char *ip_address = argv[2];
    int port = atoi(argv[3]);
    char *type = argv[5];
    char *param = argv[6];

    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_address);
    server_addr.sin_port = htons(port);

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // Send data to server
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, BUFFER_SIZE, "%s,%s", type, param);
    write(sockfd, buffer, strlen(buffer));

    // Receive response from server
    memset(buffer, 0, sizeof(buffer));
    read(sockfd, buffer, sizeof(buffer));

    printf("%s\n", buffer);

    // Close socket
    close(sockfd);

    return 0;
}
