#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	int client_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(7300); 
	server_address.sin_addr.s_addr = INADDR_ANY; 

	connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));

	char buffer[1024] = "Hello Server!";
	send(client_socket, buffer, sizeof(buffer), 0);

	close(client_socket);
}
