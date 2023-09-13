#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

int main(int argc, char const *argv[])
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_adress;
    server_adress.sin_family = AF_INET;
    server_adress.sin_port = htons(7300);
    server_adress.sin_addr.s_addr = INADDR_ANY;
    
    bind(server_socket, (struct sockaddr*)&server_adress, sizeof(server_adress));

    listen(server_socket, 5);

    int client_socket = accept(server_socket, nullptr, nullptr);

    char response[1024];
	recv(client_socket, response, sizeof(response), 0);

    std::cout << response << std::endl;
}
