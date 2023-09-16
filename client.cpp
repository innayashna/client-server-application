#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <string>
using namespace std;

const int BUFFER_SIZE = 1024;

int main(int argc, char const *argv[]) {
	int client_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(7300); 
	server_address.sin_addr.s_addr = INADDR_ANY; 

	connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));

	cout << "Connected to server!" << endl;

	char buffer[BUFFER_SIZE];

	while(true) {
		cout << "Client: ";
        cin.getline(buffer, BUFFER_SIZE);
		string str(buffer);

		if (str == "Stop") {
			cout << "Connection closed." << endl;
			close(client_socket);
		 	break;
		}

        send(client_socket, buffer, strlen(buffer), 0);

		int bytes_received = recv(client_socket, buffer, 1024, 0);

        if (bytes_received <= 0) {
            cout << "Connection closed by server." << endl;
			close(client_socket);
            break;
        }

        buffer[bytes_received] = '\0';
        cout << "Server: " << buffer << endl;
	}
}
