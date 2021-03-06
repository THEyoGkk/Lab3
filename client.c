#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT_NUMBER 5000
#define BUFFER_SIZE 256
#define FILENAME_MAX_SIZE 256

int main(int argc, char* argv[])
{
	
	int socketId = 0, recieved_bytes = 0;
	char fileName[FILENAME_MAX_SIZE], buffer[BUFFER_SIZE];	
	struct sockaddr_in  serverSocket;
	FILE *file;
	
	memset(buffer, '0', sizeof(buffer));
	
	printf("Input file name: ");
	scanf("%s", fileName);
	
	//Creating socket
	
	if((socketId = socket(AF_INET,SOCK_STREAM,0) ) < 0) {
		printf("Error : Socket could not be created.\n");
		return 1;
	}
	
	//Initializing socket data
	serverSocket.sin_family = AF_INET;
	serverSocket.sin_port = htons(PORT_NUMBER);
	serverSocket.sin_addr.s_addr = inet_addr("127.0.0.1"); //localhost
	
	
	//Initialize socket connection
	if (connect(socketId,(struct sockaddr*)&serverSocket, sizeof(serverSocket)) < 0) {
		printf("Error : Could not connect.\n");
		close(socketId);
		return 1;
	}
	
	//Send name of file
	if (send(socketId,fileName,strlen(fileName), 0) < 0) {
		printf("Error : Could not send file name");
		return 1;
	}
	printf("Filename send.\n");
	if ((recieved_bytes = read(socketId, buffer, BUFFER_SIZE)) > 0) {
		file = fopen(fileName, "wb");
		if (file == NULL) {
			printf("Error opening file.\n");
			close(socketId);
			return 1;
		}
		
		fwrite(buffer, 1, recieved_bytes, file);
	} else {
		printf("Error : File not found.\n");
		close(socketId);
		return 1;
	}
	
	while ((recieved_bytes = read(socketId, buffer, BUFFER_SIZE)) > 0)
		fwrite(buffer, 1,recieved_bytes, file);

	if (recieved_bytes < 0)
		printf("Error : Reading file failed.\n");
	
	printf("File reading finished.\n");
	fclose(file);
	close(socketId);
	return 0;
}
