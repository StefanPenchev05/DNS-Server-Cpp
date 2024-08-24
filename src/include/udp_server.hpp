#ifndef UPD_SERVER_HPP
#define UPD_SERVER_HPP

#include <netinet/in.h>

class UDPServer
{
private:
    int socket_fd;
    int port;
    struct sockaddr_in server_addr, client_addr;
    bool running;

public:
    UDPServer(int port);
    ~UDPServer();
    void start();
    void stop();
    void handleRequest();
    void sendResponse(const char *response, int reponse_len);
};

#endif