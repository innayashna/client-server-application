#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <string>

using namespace std;

const int BUFFER_SIZE = 1024;
const int MAX_CONNECTIONS = 5;
const int MAX_PORT = 49151;
const int MIN_PORT = 1024;
const char* change_port_command = "NewPort-";

void change_port(int& server_socket, struct sockaddr_in& server_address, int& client_socket, int& new_port);

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        cout << "Please pass port number as a command-line argument!" << endl;
        return -1;
    }
    int port_number = atoi(argv[1]);
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_number);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        cout << "[Server] Unable to bind." << endl;
    }

    if (listen(server_socket, MAX_CONNECTIONS) < 0) {
        cout << "[Server] Unable to listen." << endl;
    }

    bool server_disconnected = false;

    while (true) {
        if(server_disconnected) {
            close(server_socket);
            break;
        }

        cout << "[Server] Ready to accep clients!" << endl;
        int client_socket = accept(server_socket, nullptr, nullptr);
        cout << "[Client] Client connected!" << endl;

        while (true) {
            char buffer[BUFFER_SIZE];
            int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

            if (bytes_received == 0) {
                cout << "[Client] Client disconnected." << endl;
                break;
            } 

            buffer[bytes_received] = '\0';
            cout << "Client: " << buffer << endl;

            //Change port on client request
            if (strncmp(buffer, change_port_command, strlen(change_port_command)) == 0) {
                int new_port = atoi(buffer + strlen(change_port_command));

                if (MAX_PORT < new_port || new_port < MIN_PORT) {
                    char failure_message[] = "Failure. Client entered unsupported port number!";
                    send(client_socket, failure_message, strlen(failure_message), 0);
                } else {
                    cout << "[Server] Changing port to: " << new_port << endl;
                    change_port(server_socket, server_address, client_socket, new_port);
                    cout << "[Server] Client connected via new port on client request!" << endl;
                }
            }

            cout << "Server: ";
            cin.getline(buffer, BUFFER_SIZE);
            string str(buffer);

            //Disconnect server
            if (str == "Stop") {
                cout << "[Server] Connection closed." << endl;
                server_disconnected = true;
                break;
            }

            //Change port on server request
            if (str.find(change_port_command) == 0) {
                int new_port = atoi(buffer + strlen(change_port_command));
                cout << "[Server] Changing port to: " << new_port << endl;

                send(client_socket, buffer, strlen(buffer), 0);

                memset(buffer, 0, sizeof(buffer));
                recv(client_socket, buffer, sizeof(buffer), 0);

                if (strcmp(buffer, "Failure") == 0) {
                    cout << "Client: " << buffer << endl;
                } else {
                    if (strcmp(buffer, "Ready") == 0) {
                        change_port(server_socket, server_address, client_socket, new_port);
                    } 
                    
                    memset(buffer, 0, sizeof(buffer));
                    recv(client_socket, buffer, sizeof(buffer), 0);
                    cout << "Client: " << buffer << endl;
                    cout << "[Server] Client connected via new port on server request!" << endl;
                }
            } else {
                send(client_socket, buffer, strlen(buffer), 0);
            }
        }
    }
}

void change_port(int& server_socket, struct sockaddr_in& server_address, int& client_socket, int& new_port) {
    server_address.sin_port = htons(new_port);
    int new_server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (bind(new_server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        cout << "[Server] Unable to bind." << endl;
        send(client_socket, "Failure", strlen("Failure"), 0);
    }

    if (listen(new_server_socket, MAX_CONNECTIONS) < 0) {
        cout << "[Server] Unable to listen." << endl;
        send(client_socket, "Failure", strlen("Failure"), 0);
    }

    send(client_socket, "Success", strlen("Success"), 0);
    close(server_socket);
    server_socket = new_server_socket;
    client_socket = accept(server_socket, nullptr, nullptr);
}