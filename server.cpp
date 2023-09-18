#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <string>

using namespace std;

const int BUFFER_SIZE = 1024;

void changePort(int& server_socket, struct sockaddr_in& server_address, int& client_socket, int& new_port);

int main(int argc, char const *argv[]) {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(7300);
    server_address.sin_addr.s_addr = INADDR_ANY;
    
    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    listen(server_socket, 5);

    int client_socket = accept(server_socket, nullptr, nullptr);
    cout << "Client connected!" << endl;

    while(true) {
        char buffer[BUFFER_SIZE];
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

        if (bytes_received == 0) {
            cout << "Client disconnected." << endl;
            close(server_socket);
            break;
        } 

        buffer[bytes_received] = '\0';
        cout << "Client: " << buffer << endl;

        if (strncmp(buffer, "NewPort-", 8) == 0) {
            int new_port = atoi(buffer + 8);
            cout << "[Server] Changing port to " << new_port << endl;
            changePort(server_socket, server_address, client_socket, new_port);
            cout << "Client connected via new port on client request!" << endl;
        }

        cout << "Server: ";
        cin.getline(buffer, BUFFER_SIZE);
        string str(buffer);

		if (str == "Stop") {
			cout << "Connection closed." << endl;
			close(server_socket);
		 	break;
		}

        if (str.find("NewPort-") == 0) {
            int new_port = atoi(buffer + 8);
            cout << "[Server] Changing port to " << new_port << endl;

            send(client_socket, buffer, strlen(buffer), 0);

            memset(buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);

            if (strcmp(buffer, "Ready") == 0) {
                changePort(server_socket, server_address, client_socket, new_port);
            } 
            
            memset(buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            cout << "Client: " << buffer << endl;
            cout << "Client connected via new port on server request!" << endl;
        } else {
            send(client_socket, buffer, strlen(buffer), 0);
        }
    }
}

void changePort(int& server_socket, struct sockaddr_in& server_address, int& client_socket, int& new_port) {
    server_address.sin_port = htons(new_port);
    int new_server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (bind(new_server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        cout << "Unable to bind." << endl;
    }

    if (listen(new_server_socket, 5) < 0) {
        cout << "Unable to listen." << endl;
    }

    send(client_socket, "Success", strlen("Success"), 0);
    close(server_socket);
    server_socket = new_server_socket;
    client_socket = accept(server_socket, nullptr, nullptr);
}