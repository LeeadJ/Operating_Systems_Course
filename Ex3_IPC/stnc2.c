#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define CHUNK_SIZE (100 * 1024 * 1024) // 100 MB
#define BUFFER_SIZE 1024

unsigned int compute_checksum(const char *data, size_t len);
char *generate_data(size_t size);

typedef struct {
    char *data;      // pointer to the data chunk
    unsigned int cksum;  // checksum for the data chunk
} data_t;




int main(int argc, char *argv[]) 
{
    // Checking if the command line arguments are fine:
    if (argc != 6) 
    {
        fprintf(stderr, "Usage: %s -c IP PORT -p PARAM TYPE \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* --Parse command-line arguments-- */
    char *ip_address;
    int port = atoi(argv[3]);
    char *type = argv[5];
    char *param = argv[4];

    if (strcmp(argv[1], "-c") == 0)
    {
        ip_address = argv[2];
    }

    /* --Generating DATA of size 100MB-- */
    uint32_t cksum;
    data_t *payload = malloc(sizeof(dara_t));
    // Generate the data block of size 100MB:
    char *data_ptr = generate_data(CHUNK_SIZE);
    // Compute the checksum
    cksum = compute_checksum(data_ptr, CHUNK_SIZE);
    payload->data = data_ptr;
    payload->cksum = cksum;
    

    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) 
    {
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
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) 
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // Send data to server
    char buffer[CHUNK_SIZE];
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, CHUNK_SIZE, "%u", CHUNK_SIZE);
    memcpy(buffer, data_ptr, CHUNK_SIZE);
    write(sockfd, buffer, CHUNK_SIZE);

    // Receive response from server
    memset(buffer, 0, sizeof(buffer));
    read(sockfd, buffer, sizeof(buffer));

    printf("%s\n", buffer);

    // Close socket
    close(sockfd);
    free(data_ptr);
    return EXIT_SUCCESS;
}




unsigned int compute_checksum(const char *data, size_t len) {
    unsigned int sum = 0;
    for (size_t i = 0; i < len; i++) {
        sum += (unsigned char)data[i];
    }
    return sum;
}

char *generate_data(size_t size) {
    char *data = malloc(size);
    if (data == NULL) {
        fprintf(stderr, "Error: failed to allocate memory for data.\n");
        exit(1);
    }

    // Fill the data block with random data
    for (size_t i = 0; i < size; i++) {
        data[i] = rand() % 256;  // Generate a random byte value (0-255)
    }

    return data;
}


