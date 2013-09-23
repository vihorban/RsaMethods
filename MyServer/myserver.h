#include "..\mysocket.h"
class ClientConnectionInfo
{
public:
	bool IsFree;
	int ClientId;
	SOCKET clientSocket;
	HOSTENT *hst;
	sockaddr_in client_addr;
};
int findfree(ClientConnectionInfo*);
int server();
unsigned long __stdcall ReceiveFromClient(void* client_socket);
unsigned long __stdcall ServerInterface(void* client_socket);
void printusers();