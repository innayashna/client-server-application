#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
using namespace std;

const int BUFFER_SIZE = 1024;

int main(int argc, char const *argv[]) {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_adress;
    server_adress.sin_family = AF_INET;
    server_adress.sin_port = htons(7300);
    server_adress.sin_addr.s_addr = INADDR_ANY;
    
    bind(server_socket, (struct sockaddr*)&server_adress, sizeof(server_adress));

    listen(server_socket, 5);

    int client_socket = accept(server_socket, nullptr, nullptr);

    cout << "Client connected!" << endl;

    char buffer[BUFFER_SIZE];

    while(true) {
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

        if (bytes_received == 0) {
            cout << "Client disconnected." << endl;
            close(server_socket);
            break;
        }

        buffer[bytes_received] = '\0';
        cout << "Client: " << buffer << endl;

        cout << "Server: ";
        cin.getline(buffer, 1024);
        string str(buffer);

		if (str == "Stop") {
			cout << "Connection closed." << endl;
			close(server_socket);
		 	break;
		}
        
        send(client_socket, buffer, strlen(buffer), 0);
    }
}