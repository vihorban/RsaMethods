#include "myserver.h"
WORD wVersionRequested;
WSADATA wsaData;
int nclients = 0;
bool ServerOnline=true;
bool IsBanned[10];
ClientConnectionInfo ClientData[10];
int server()
{

	for(int i=0;i<10;++i)
	{
		ClientData[i].IsFree=true;
		ClientData[i].ClientId=i;
	}

    printf("Running server...\n");
	wVersionRequested = MAKEWORD( 2, 2 );
    if (WSAStartup(wVersionRequested, &wsaData))
    {
        printf("Error WSAStartup %d\n", WSAGetLastError());
        return -1;
    }

    SOCKET mysocket;

    if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {

        printf("Error socket %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }


    sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(ServerPort); 
    local_addr.sin_addr.s_addr = 0; 


    if (bind(mysocket, (sockaddr *)&local_addr, sizeof(local_addr)))
    {

        printf("Error bind %d\n", WSAGetLastError());
        closesocket(mysocket);
        WSACleanup();
        return -1;
    }



    if (listen(mysocket, 0x100))
    {
   
        printf("Error listen %d\n", WSAGetLastError());
        closesocket(mysocket);
        WSACleanup();
        return -1;
    }

    printf("Waiting for connections...\n");

    SOCKET client_socket; 
    sockaddr_in client_addr;


    int client_addr_size = sizeof(client_addr);


    while ((client_socket = accept(mysocket, (sockaddr *)&client_addr, &client_addr_size))&&ServerOnline)
    {
		if(nclients<10)
		{
        nclients++; 

    
        HOSTENT *hst;
        hst = gethostbyaddr((char *)&client_addr. sin_addr.s_addr, 4, AF_INET);

		int index=findfree(ClientData);

		printf("Client#%d %s [%s] connected!\n",index,
        (hst) ? hst->h_name : "", inet_ntoa(client_addr.sin_addr));
        printusers();
		ClientData[index].IsFree=false;
		ClientData[index].clientSocket=client_socket;
		ClientData[index].client_addr=client_addr;
		ClientData[index].hst=hst;

    
        DWORD thID;
        CreateThread(NULL, NULL, ReceiveFromClient, &ClientData[index], NULL, &thID);
		}
		else
		{
			printf("Trying to connect. Too much clients\nDisconnecting...\n");
			const char* Warn="Too much connections. Try again later\n";
			send(client_socket, Warn, sizeof(Warn), 0);
			closesocket(client_socket);
		}
    }
	printf("Server shutting down...\n");
	closesocket(mysocket);
    WSACleanup();
    return 0;
}
unsigned long __stdcall ReceiveFromClient(void* client_info)
{
	ClientConnectionInfo* pClient=(ClientConnectionInfo*)client_info;
    SOCKET my_sock;
	my_sock = pClient->clientSocket;
	char *buff = new char[bufSize];
	memset(buff, 0, bufSize);
    string shello="1nsHello!";

	send(my_sock, shello.c_str(), shello.length(), 0);

    while(recv(my_sock, buff, bufSize-1, 0)!=SOCKET_ERROR&&ServerOnline)
	{
		if(buff[0]!=0)
		{
			if(strncmp(&buff[3], "#_shut_down", 11)==0)
			{
				printf("Shut down command received from client #%d\n",pClient->ClientId);
				ServerOnline=false;
				break;
			}
			else
			{
				int k=size(buff);
				if(buff[1]=='k') k=27;
				string ToWho, TypeEnc;
				if(buff[0]=='a')
				{
					ToWho="to all";
					buff[2]=pClient->ClientId+48;
					int i;
					for(i=0;i<10;++i)
						if(i!=pClient->ClientId&&ClientData[i].IsFree==false)
							send(ClientData[i].clientSocket, buff, k, 0);
				}
				else
				{
					int numcli=buff[0]-48;
					if(numcli>=0&&numcli<=10&&numcli!=pClient->ClientId&&ClientData[numcli].IsFree==false)
					{
						ToWho="to Client#";
						ToWho.push_back(buff[0]);
						buff[2]=pClient->ClientId+48;
						send(ClientData[numcli].clientSocket, buff, k, 0);
					}
					else
					{
						ToWho="to wrong Client";
						string Err="1nsClient is offline";
						send(my_sock, Err.c_str(), Err.length(), 0);
					}
				}
				if(buff[1]=='e')
				{
					TypeEnc="(encrypted by RSA)";
				}
				else if(buff[1]=='p')
				{
					TypeEnc="(protected by password)";
				}
				else
				{
					TypeEnc="";
				}
				char* temp=&buff[3];
				printf("Client#%d: (%s) %s %s\n",pClient->ClientId,ToWho.c_str(),TypeEnc.c_str(),temp);
				memset(buff, 0, bufSize);
			}
		}
	}
	if(ServerOnline==false) 
	{
			string ShutDown="1nsServer is shutting down...";
			send(my_sock, ShutDown.c_str(), ShutDown.length(), 0);
	}

    nclients--; 
    printf("Client#%d disconnected\n",pClient->ClientId); 
	printusers();
	pClient->IsFree=true;


    closesocket(my_sock);
    return 0;
}
int findfree(ClientConnectionInfo* a)
{
	int i;
	for(i=1;i<10;++i)
		if(a[i].IsFree==true) return i;
	return 0;
}
void printusers()
{
		if (nclients) printf("%d user on-line\n", nclients);
        else printf("No User on line\n");
}
