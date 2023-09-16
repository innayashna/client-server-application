#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char const *argv[]) {
	int client_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(7300); 
	server_address.sin_addr.s_addr = INADDR_ANY; 

	connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));

	cout << "Write your message to server here:" << "\n";

	while(true) {
		string input_line;
		getline(cin, input_line);

		if (input_line.empty()) {
            break; 
        } 

		if (input_line == "Stop") {
			cout << "Connection closed" << endl;
			close(client_socket);
			break;
		}

		send(client_socket, input_line.c_str(), input_line.length(), 0);
	}
}
