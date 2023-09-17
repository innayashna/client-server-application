#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <string>

using namespace std;

const int BUFFER_SIZE = 1024;

void changePort(int& client_socket, struct sockaddr_in& server_address, char* buffer);

int main(int argc, char const *argv[]) {
	int client_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(7300); 
	server_address.sin_addr.s_addr = INADDR_ANY; 

	connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));

	cout << "Connected to server!" << endl;

	while(true) {
		char buffer[BUFFER_SIZE];
		cout << "Client: ";
        cin.getline(buffer, BUFFER_SIZE);
		string str(buffer);

		if (str == "Stop") {
			cout << "Connection closed." << endl;
			close(client_socket);
		 	break;
		}

		if (str.find("NewPort-") == 0) {
			send(client_socket, buffer, strlen(buffer), 0);
			changePort(client_socket, server_address, buffer);

			memset(buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            cout << "Server: " << buffer << endl;
			cout << "Connected to server via new port!" << endl;
		} else {
       		send(client_socket, buffer, strlen(buffer), 0);
		}

		int bytes_received = recv(client_socket, buffer, 1024, 0);

        if (bytes_received <= 0) {
            cout << "Connection closed by server." << endl;
			close(client_socket);
            break;
        }

        buffer[bytes_received] = '\0';
        cout << "Server: " << buffer << endl;

		if (strncmp(buffer, "NewPort-", 8) == 0) {
			changePort(client_socket, server_address, buffer);
			send(client_socket, "Success", strlen("Success"), 0);
			cout << "Connected to server via new port!" << endl;
		}
	}
}

void changePort(int& client_socket, struct sockaddr_in& server_address, char* buffer) {
    int new_port = atoi(buffer + 8);
    cout << "Changing port to " << new_port << endl;

	server_address.sin_port = htons(new_port); 
	connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));
}
