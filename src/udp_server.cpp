#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include "udp_server.hpp"
#include "dns_message.hpp"

UDPServer::UDPServer(int port) : port(port), running(false)
{
    // create a UPD socket
    this->socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (this->socket_fd == -1)
    {
        std::cerr << "Socket creation failed: " << strerror(errno) << "..." << std::endl;
        return;
    }
}

UDPServer::~UDPServer()
{
    if (this->running == true)
    {
        this->stop();
    }
}

void UDPServer::start()
{
    // Bind the socket to the specific port
    this->server_addr.sin_family = AF_INET;
    this->server_addr.sin_port = htons(this->port);
    this->server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(this->server_addr.sin_zero, '\0', sizeof(this->server_addr.sin_zero));

    if (bind(this->socket_fd, (struct sockaddr *)&this->server_addr, sizeof(this->server_addr)) != 0)
    {
        std::cerr << "Bind failed: " << strerror(errno) << std::endl;
        return;
    }

    // set the server running flag to true
    this->running = true;

    while (this->running)
    {
        this->handleRequest();
    }
}

void UDPServer::stop()
{
    this->running = false;
    close(this->socket_fd);
}

void UDPServer::handleRequest()
{
    char buffer[1024];
    socklen_t addr_len = sizeof(this->client_addr);

    // recieve data from client
    int recv_len = recvfrom(this->socket_fd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&this->client_addr, &addr_len);
    if (recv_len < 0)
    {
        std::cerr << "Receive failed: " << strerror(errno) << std::endl;
        return;
    }

    // Parse the received DNS message
    Header requestHeader;
    requestHeader.parse(reinterpret_cast<std::uint8_t *>(buffer));

    // Construct a response header
    Header responseHeader;
    responseHeader.tran_id = requestHeader.tran_id;
    responseHeader.flags.qr = 1;
    responseHeader.flags.opcode = requestHeader.flags.opcode;
    responseHeader.flags.aa = 0;
    responseHeader.flags.tc = 0;
    responseHeader.flags.rd = requestHeader.flags.rd;
    responseHeader.flags.ra = 1;
    responseHeader.flags.z = 0; 
    responseHeader.flags.rcode = 0;
    responseHeader.question_count = requestHeader.question_count;
    responseHeader.answer_count = htons(1);
    responseHeader.authority_count = 0;
    responseHeader.additional_count = 0;

    // Serialize the response header
    std::uint8_t responseBuffer[1024];
    responseHeader.serialize(responseBuffer);

     // Copy the question section from the request to the response
    std::memcpy(responseBuffer + 12, buffer + 12, recv_len - 12);

    // Prepare a simple answer (not a real DNS answer, just an example)
    int response_len = recv_len; // Include the question in the response length
    std::uint8_t* answer_ptr = responseBuffer + response_len;
    std::memcpy(answer_ptr, "\xc0\x0c\x00\x01\x00\x01\x00\x00\x00\x3c\x00\x04", 12); // Example answer header
    answer_ptr += 12;
    std::uint32_t ip_addr = htonl(0x7F000001); // 127.0.0.1 in network byte order
    std::memcpy(answer_ptr, &ip_addr, sizeof(ip_addr));
    response_len += 16; // 12 bytes header + 4 bytes IP address

    // Send the response back to the client
    int send_len = sendto(this->socket_fd, responseBuffer, response_len, 0, (struct sockaddr *)&this->client_addr, addr_len);
    if (send_len < 0)
    {
        std::cerr << "Send failed: " << strerror(errno) << std::endl;
    }
}