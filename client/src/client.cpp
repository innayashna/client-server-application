#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <string>

using namespace std;

const int BUFFER_SIZE = 1024;
const int MAX_PORT = 49151;
const int MIN_PORT = 1024;
const char* change_port_command = "NewPort-";

void change_port(int& client_socket, struct sockaddr_in& server_address, int& new_port);

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        cout << "Please pass port number as a command-line argument!" << endl;
        return -1;
    }
    int port_number = atoi(argv[1]);
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_number); 
    server_address.sin_addr.s_addr = INADDR_ANY; 

    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == 0) {
        cout << "[Client] Connected to server!" << endl;
    } else {
        cout << "[Client] Can not reach server!" << endl;
        return -1;
    }

    while (true) {
        char buffer[BUFFER_SIZE];
        cout << "Client: ";
        cin.getline(buffer, BUFFER_SIZE);
        string str(buffer);

        //Disconnect client
        if (str == "Stop") {
            cout << "[Client] Connection closed." << endl;
            close(client_socket);
             break;
        }

        //Change port on client request
        if (str.find(change_port_command) == 0) {
            int new_port = atoi(buffer + strlen(change_port_command));
            cout << "[Client] Changing port to: " << new_port << endl;

            send(client_socket, buffer, strlen(buffer), 0);

            memset(buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            cout << "Server: " << buffer << endl;

            if (strcmp(buffer, "Success") == 0) {
                change_port(client_socket, server_address, new_port);
                cout << "[Client] Connected to server via new port on client request!" << endl;
            }

        } else {
               send(client_socket, buffer, strlen(buffer), 0);
        }

        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            cout << "[Server] Connection closed." << endl;
            close(client_socket);
            break;
        }

        buffer[bytes_received] = '\0';
        cout << "Server: " << buffer << endl;

        //Change port on server request
        if (strncmp(buffer, change_port_command, strlen(change_port_command)) == 0) {
            int new_port = atoi(buffer + strlen(change_port_command));

            if (MAX_PORT < new_port || new_port < MIN_PORT) {
                char failure_message[] = "Failure. Server entered unsupported port number!";
                send(client_socket, failure_message, strlen(failure_message), 0);
            } else {
                cout << "[Client] Changing port to " << new_port << endl;
                send(client_socket, "Ready", strlen("Ready"), 0);

                memset(buffer, 0, sizeof(buffer));
                recv(client_socket, buffer, sizeof(buffer), 0);

                if (strcmp(buffer, "Success") == 0) {
                    memset(buffer, 0, sizeof(buffer));
                    change_port(client_socket, server_address, new_port);
                    send(client_socket, "Success", strlen("Success"), 0);
                    cout << "[Client] Connected to server via new port on server request!" << endl;
                }
            }
        }
    }
}

void change_port(int& client_socket, struct sockaddr_in& server_address, int& new_port) {
    server_address.sin_port = htons(new_port);
    int new_client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (connect(new_client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        cout << "[Client] Unable to connect to new server port" << endl;
    };

    close(client_socket);
    client_socket = new_client_socket;
}
