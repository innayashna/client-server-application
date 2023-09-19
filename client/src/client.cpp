#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <string>

using namespace std;

const int BUFFER_SIZE = 1024;

void change_port(int& client_socket, struct sockaddr_in& server_address, int& new_port);

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        cout << "Please pass port number as a command-line argument" << endl;
        return -1;
    }
    int port_number = atoi(argv[1]);
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_number); 
    server_address.sin_addr.s_addr = INADDR_ANY; 

    connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    cout << "Connected to server!" << endl;

    while (true) {
        char buffer[BUFFER_SIZE];
        cout << "Client >>> ";
        cin.getline(buffer, BUFFER_SIZE);
        string str(buffer);

        if (str == "Stop") {
            cout << "Connection closed." << endl;
            close(client_socket);
             break;
        }

        if (str.find("NewPort-") == 0) {
            int new_port = atoi(buffer + 8);
            cout << "[Client] Changing port to " << new_port << endl;

            send(client_socket, buffer, strlen(buffer), 0);

            memset(buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            cout << "Server: " << buffer << endl;

            if (strcmp(buffer, "Success") == 0) {
                change_port(client_socket, server_address, new_port);
            }

            cout << "Connected to server via new port on client request!" << endl;
        } else {
               send(client_socket, buffer, strlen(buffer), 0);
        }

        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            cout << "Connection closed by server." << endl;
            close(client_socket);
            break;
        }

        buffer[bytes_received] = '\0';
        cout << "Server: " << buffer << endl;

        if (strncmp(buffer, "NewPort-", 8) == 0) {
            int new_port = atoi(buffer + 8);
            cout << "[Client] Changing port to " << new_port << endl;
            send(client_socket, "Ready", strlen("Ready"), 0);

            memset(buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);

            if (strcmp(buffer, "Success") == 0) {
                memset(buffer, 0, sizeof(buffer));
                change_port(client_socket, server_address, new_port);
                send(client_socket, "Success", strlen("Success"), 0);
            }
            cout << "Connected to server via new port on server request!" << endl;
        }
    }
}

void change_port(int& client_socket, struct sockaddr_in& server_address, int& new_port) {
    server_address.sin_port = htons(new_port);
    int new_client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (connect(new_client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == 0) {
        cout << "Successfull connection to new server port" << endl;
    };

    close(client_socket);
    client_socket = new_client_socket;
}
