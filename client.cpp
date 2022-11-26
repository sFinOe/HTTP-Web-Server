


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

int main() {
    int sock = 0; long valread;
    struct sockaddr_in server_addr;
    char request_line[] = "GET / HTTP/1.1\r\nHost:localhost:80\r\nContent-Type: dsfasdff/htadfdml\r\n\r\n";
    char buffer[1024] = { 0 };
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "Socket creation error" << std::endl;
        return -1;
    };
    memset(&server_addr, '0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0)
    {
        std::cout << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cout << "Connection Failed" << std::endl;
        return -1;
    }
    send(sock, request_line, strlen(request_line), 0);
    std::cout << "Request sent" << std::endl;
    valread = recv(sock, buffer, 1024, 0);
    std::cout << buffer << std::endl;
    return 0;

}