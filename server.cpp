#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
using namespace std;

int main(int argc, char const *argv[]) {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_adress;
    server_adress.sin_family = AF_INET;
    server_adress.sin_port = htons(7300);
    server_adress.sin_addr.s_addr = INADDR_ANY;
    
    bind(server_socket, (struct sockaddr*)&server_adress, sizeof(server_adress));

    listen(server_socket, 5);

    int client_socket = accept(server_socket, nullptr, nullptr);

    char client_response[1024];

    while(true) {
        int bytes_received = recv(client_socket, client_response, sizeof(client_response), 0);

        if (bytes_received == 0) {
            cout << "Client disconnected." << endl;
            close(server_socket);
            break;
        }
        
        cout << client_response << endl;
    }
}