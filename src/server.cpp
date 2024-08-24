#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // Disable output buffering
    setbuf(stdout, NULL);

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cout << "Logs from your program will appear here!" << std::endl;

    int udpSocket;
    struct sockaddr_in clientAddress;

    udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSocket == -1) {
        std::cerr << "Socket creation failed: " << strerror(errno) << "..." << std::endl;
        return 1;
    }

    // Since the tester restarts your program quite often, setting REUSE_PORT
    // ensures that we don't run into 'Address already in use' errors
    int reuse = 1;
    if (setsockopt(udpSocket, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) < 0) {
        std::cerr << "SO_REUSEPORT failed: " << strerror(errno) << std::endl;
        return 1;
    }

    sockaddr_in serv_addr = { .sin_family = AF_INET,
                              .sin_port = htons(2053),
                              .sin_addr = { htonl(INADDR_ANY) },
                            };

    if (bind(udpSocket, reinterpret_cast<struct sockaddr*>(&serv_addr), sizeof(serv_addr)) != 0) {
        std::cerr << "Bind failed: " << strerror(errno) << std::endl;
        return 1;
    }

    int bytesRead;
    char buffer[512];
    socklen_t clientAddrLen = sizeof(clientAddress);

    while (true) {
        // Receive data
        bytesRead = recvfrom(udpSocket, buffer, sizeof(buffer), 0, reinterpret_cast<struct sockaddr*>(&clientAddress), &clientAddrLen);
        if (bytesRead == -1) {
            perror("Error receiving data");
            break;
        }

        buffer[bytesRead] = '\0';
        std::cout << "Received " << bytesRead << " bytes: " << buffer << std::endl;

        // Create a minimal DNS response
        unsigned char response[512];
        memset(response, 0, sizeof(response));

        // Copy the ID from the request
        response[0] = buffer[0];
        response[1] = buffer[1];

        // Set the flags: response, authoritative answer, recursion available, no error
        response[2] = 0x81;
        response[3] = 0x80;

        // Questions: 1
        response[4] = buffer[4];
        response[5] = buffer[5];

        // Answers: 1
        response[6] = 0x00;
        response[7] = 0x01;

        // Authority RRs: 0
        response[8] = 0x00;
        response[9] = 0x00;

        // Additional RRs: 0
        response[10] = 0x00;
        response[11] = 0x00;

        // Copy the original query
        memcpy(response + 12, buffer + 12, bytesRead - 12);

        // Answer section
        int answerOffset = bytesRead;
        response[answerOffset++] = 0xc0; // Name pointer
        response[answerOffset++] = 0x0c; // Offset to the domain name in the query

        // Type: A (host address)
        response[answerOffset++] = 0x00;
        response[answerOffset++] = 0x01;

        // Class: IN (Internet)
        response[answerOffset++] = 0x00;
        response[answerOffset++] = 0x01;

        // TTL: 0 (no caching)
        response[answerOffset++] = 0x00;
        response[answerOffset++] = 0x00;
        response[answerOffset++] = 0x00;
        response[answerOffset++] = 0x00;

        // Data length: 4 bytes (IPv4 address)
        response[answerOffset++] = 0x00;
        response[answerOffset++] = 0x04;

        // Address: 127.0.0.1
        response[answerOffset++] = 127;
        response[answerOffset++] = 0;
        response[answerOffset++] = 0;
        response[answerOffset++] = 1;

        // Send response
        if (sendto(udpSocket, response, answerOffset, 0, reinterpret_cast<struct sockaddr*>(&clientAddress), sizeof(clientAddress)) == -1) {
            perror("Failed to send response");
        }
    }

    close(udpSocket);

    return 0;
}