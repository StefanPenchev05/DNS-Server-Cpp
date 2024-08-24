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

    

    // Send the response back to the client
    int send_len = sendto(this->socket_fd, responseBuffer, 0, 0, (struct sockaddr *)&this->client_addr, addr_len);
    if (send_len < 0)
    {
        std::cerr << "Send failed: " << strerror(errno) << std::endl;
    }
}