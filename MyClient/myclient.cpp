#include <fstream>
#include "myclient.h"
#include "RSAmethods.h"
bool Quit=false;
long long n=9173503;
long long e=3;
long long d=6111579;
string password;
WORD wVersionRequested;
WSADATA wsaData;
int client()
{
    printf("Running client...\n");
	wVersionRequested = MAKEWORD( 2, 2 );
    if (WSAStartup(wVersionRequested, &wsaData))
    {
        printf("WSAStart error %d\n", WSAGetLastError());
        return -1;
    }
    SOCKET my_sock;
    my_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (my_sock < 0)
    {
        printf("Socket() error %d\n", WSAGetLastError());
        return -1;
    }
    sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(DefaultPort);
    HOSTENT *hst;
    if (inet_addr(DefaultServerAddress) != INADDR_NONE)
        dest_addr.sin_addr.s_addr = inet_addr(DefaultServerAddress);
    else
    {
        if (hst = gethostbyname(DefaultServerAddress))
            ((unsigned long *)&dest_addr.sin_addr)[0] =
            ((unsigned long **)hst->h_addr_list)[0][0];
        else
        {
            printf("Invalid address %s\n", DefaultServerAddress);
            closesocket(my_sock);
            WSACleanup();
            return -1;
        }
    }

    if (connect(my_sock, (sockaddr *)&dest_addr, sizeof(dest_addr)))
    {
        printf("Connect error %d\n", WSAGetLastError());
        return -1;
    }

    printf("Connected to %s successfully\n \
		   Type #_quit for quit\n\n", DefaultServerAddress);

	DWORD thID;

	CreateThread(NULL, NULL, ReceiveFromServer, &my_sock, NULL, &thID);

	string dataToSend;

	string WhoToSend="a",TypeEnc="n",FromWho="x";


	if(readstring(dataToSend,WhoToSend,TypeEnc,FromWho)==-1)
		{
			closesocket(my_sock);
            WSACleanup();
            return 0;
		}
	int k=dataToSend.size();

	char* buff=new char[bufSize];

	for(int i=0;i<k;++i)
		buff[i]=dataToSend[i];

	while(send(my_sock, buff, k, 0)!=SOCKET_ERROR)
    {
		if(readstring(dataToSend,WhoToSend,TypeEnc,FromWho)==-1)
		{
			closesocket(my_sock);
            WSACleanup();
            return 0;
		}
		k=dataToSend.size();
		for(int i=0;i<k;++i)
			buff[i]=dataToSend[i];
    }
	printf("Error sending\n");
    closesocket(my_sock);
    WSACleanup();
    return -1;
}
unsigned long __stdcall ReceiveFromServer(void* server_socket)
{
	SOCKET my_sock = ((SOCKET *)server_socket)[0];
	char *buff = new char[bufSize];
	memset(buff, 0, bufSize);

    while(recv(my_sock, buff, bufSize-1, 0)!=SOCKET_ERROR&&strncmp(&(buff[3]), "#_banned", 8)!=0)
	{
		if(buff[0]!=0)
		{
			{
				char* temp=&buff[3];
				if(buff[2]=='s')
				{
					printf("Server#: %s\n",temp);
				}
				else if(buff[1]=='k')
				{
					printf("Received RSA key from Client#%d\n",buff[2]-48);
					long long *Pkey=(long long*)temp;
					n=Pkey[0];
					e=Pkey[1];
					d=Pkey[2];
					cout<<"General: n="<<n<<endl;
					cout<<"Public: e="<<e<<endl;
					cout<<"Secret: d="<<d<<endl;
					{//Потрібний запис в файл чи ні!!!!!?????????
						ofstream file1;
						ofstream file2;
						file1.open("public key.txt");
						file1<<n<<endl<<e;
						file1.close();
						file2.open("secret key.txt");
						file2<<n<<endl<<d;
						file2.close();
					}
				}
				else
				{
					string ToWho, TypeEnc;
					if(buff[0]=='a')
					{
						ToWho="to all";
					}
					else
					{
						ToWho="to you";
					}
					if(buff[1]=='e')
					{
						TypeEnc="(encrypted by RSA)";
						string s1;
						int s=size(buff);
						while((s-2)%8!=0) s++;
						s1.resize(s-2);
						for(s=0;s<s1.size();++s)
							s1[s]=temp[s];
						s1=decode_string(s1,n,d);
						for(s=0;s<s1.size();++s)
							temp[s]=s1[s];
						temp[s1.size()]=0;
					}
					else if(buff[1]=='p')
					{
						TypeEnc="(protected by password)";
					}
					else
					{
						TypeEnc="";
					}
					if(buff[2]=='x')
						printf("Anonymus#: (%s) %s %s\n",ToWho.c_str(),TypeEnc.c_str(),temp);
					else
						printf("Client#%d: (%s) %s %s\n",buff[2]-48,ToWho.c_str(),TypeEnc.c_str(),temp);
				}
				memset(buff, 0, bufSize);
			}
		}
	}

	if(strncmp(&(buff[3]), "#_banned", 8)==0)
	{
		printf("You are banned\n");
	}
	else
	{
		if(Quit==false)printf("Server offline\n");
	}

    closesocket(my_sock);
	WSACleanup();
	_getch();
    exit(0);
	return -1;
}
int readstring(string& dataToSend, string& WhoToSend, string& TypeEnc, string& FromWho)
{
	getline(cin,dataToSend);

	bool analyse=true;
		if(dataToSend=="#_quit")
		{
			Quit=true;
			printf("Exit...\n");
			return -1;
		}
		else if(dataToSend=="#_gen_auto_key")
		{
			char y;
			gen_key_rand(n,e,d);
			cout<<"General: n="<<n<<endl;
			cout<<"Public: e="<<e<<endl;
			cout<<"Secret: d="<<d<<endl;
			cout<<"Write to file?(y or n)\n";
			cin>>y;
			if(y=='y'||y=='Y')
			{
				ofstream file1;
				ofstream file2;
				file1.open("public key.txt");
				file1<<n<<endl<<e;
				file1.close();
				file2.open("secret key.txt");
				file2<<n<<endl<<d;
				file2.close();
			}
			cout<<"Send by network?(y or n)\n";
			cin>>y;
			if(y=='y'||y=='Y')
			{
				analyse=false;
				long long temp[4];
				temp[0]=n;
				temp[1]=e;
				temp[2]=d;
				temp[3]=0;
				dataToSend.resize(35);
				char* Ptemp=(char*)temp;
				dataToSend[0]='a';
				dataToSend[1]='n';
				dataToSend[2]='x';
				for(int i=3;i<35;++i)
					dataToSend[i]=Ptemp[i-3];
			}
		}
		else if(dataToSend=="#_gen_key")
		{
			char y;
			long long p,q;
			cout<<"Enter p(simple):\n";
			cin>>p;
			cout<<"Enter q(simple):\n";
			cin>>q;
			cout<<"Enter e(not nessesary):\n";
			cin>>e;
			gen_key_manual(p,q,n,e,d);
			cout<<"General: n="<<n<<endl;
			cout<<"Public: e="<<e<<endl;
			cout<<"Secret: d="<<d<<endl;
			cout<<"Write to file?(y or n)\n";
			cin>>y;
			if(y=='y'||y=='Y')
			{
				ofstream file1;
				ofstream file2;
				file1.open("public key.txt");
				file1<<n<<endl<<e;
				file1.close();
				file2.open("secret key.txt");
				file2<<n<<endl<<d;
				file2.close();
			}
			cout<<"Send by network?(y or n)\n";
			cin>>y;
			if(y=='y'||y=='Y')
			{
				analyse=false;
				long long temp[3];
				temp[0]=n;
				temp[1]=e;
				temp[2]=d;
				dataToSend.resize(27);
				char* Ptemp=(char*)temp;
				dataToSend[0]='a';
				dataToSend[1]='k';
				dataToSend[2]='x';
				for(int i=3;i<27;++i)
					dataToSend[i]=Ptemp[i-3];
			}
		}
		else if(dataToSend=="#_load_key")
		{
			char y;
			cout<<"Use key from file?(y or n)\n";
			cin>>y;
			if(y=='y'||y=='Y')
			{
				ifstream file1;
				file1.open("public key.txt");
				file1>>n;
				file1>>e;
				file1.close();
				file1.open("secret key.txt");
				file1>>n;
				file1>>d;
				file1.close();
			}
			else
			{
				cout<<"Enter n(general):\n";
				cin>>n;
				cout<<"Enter e(public):\n";
				cin>>e;
				cout<<"Enter d(secret):\n";
				cin>>d;
			}
			cout<<"Send by network?(y or n)\n";
			cin>>y;
			if(y=='y'||y=='Y')
			{
				analyse=false;
				long long temp[3];
				temp[0]=n;
				temp[1]=e;
				temp[2]=d;
				dataToSend.resize(27);
				char* Ptemp=(char*)temp;
				dataToSend[0]='a';
				dataToSend[1]='k';
				dataToSend[2]='x';
				for(int i=3;i<27;++i)
					dataToSend[i]=Ptemp[i-3];
			}
		}

		if(analyse) AnalyseToSend(dataToSend,WhoToSend,TypeEnc,FromWho);
		return 0;
}
void AnalyseToSend(string& dataToSend, string& WhoToSend, string& TypeEnc, string& FromWho)
{

	int poscom;

	if((poscom=dataToSend.find("#_i_want_to_be_anonymus"))!=-1)
	{
		dataToSend.erase(poscom,24);
		FromWho="x";
	}
	if((poscom=dataToSend.find("#encryption"))!=-1)
	{
		dataToSend.erase(poscom,12);
		TypeEnc="e";
	}
	else if((poscom=dataToSend.find("#encrypt"))!=-1)
	{
		dataToSend.erase(poscom,9);
		TypeEnc="e";
	}
	else if((poscom=dataToSend.find("#enc"))!=-1)
	{
		dataToSend.erase(poscom,5);
		TypeEnc="e";
	}
	else if((poscom=dataToSend.find("#e"))!=-1)
	{
		dataToSend.erase(poscom,3);
		TypeEnc="e";
	}
	else if((poscom=dataToSend.find("#noencryption"))!=-1)
	{
		dataToSend.erase(poscom,14);
		TypeEnc="n";
	}
	else if((poscom=dataToSend.find("#noencrypt"))!=-1)
	{
		dataToSend.erase(poscom,11);
		TypeEnc="n";
	}
	else if((poscom=dataToSend.find("#noenc"))!=-1)
	{
		dataToSend.erase(poscom,7);
		TypeEnc="n";
	}
	else if((poscom=dataToSend.find("#no"))!=-1)
	{
		dataToSend.erase(poscom,4);
		TypeEnc="n";
	}
	else if((poscom=dataToSend.find("#n"))!=-1)
	{
		dataToSend.erase(poscom,3);
		TypeEnc="n";
	}
	else if((poscom=dataToSend.find("#password"))!=-1)
	{
		dataToSend.erase(poscom,10);
		TypeEnc="p";
	}
	else if((poscom=dataToSend.find("#pass"))!=-1)
	{
		dataToSend.erase(poscom,6);
		TypeEnc="p";
	}
	else if((poscom=dataToSend.find("#p"))!=-1)
	{
		dataToSend.erase(poscom,3);
		TypeEnc="p";
	} 

	if((poscom=dataToSend.find("@all"))!=-1)
	{
		dataToSend.erase(poscom,5);
		WhoToSend="a";
	}
	else if((poscom=dataToSend.find("@a"))!=-1)
	{
		dataToSend.erase(poscom,3);
		WhoToSend="a";
	}
	else if((poscom=dataToSend.find("@client"))!=-1)
	{
		WhoToSend=dataToSend.substr(7,1);
		dataToSend.erase(poscom,9);
	}
	else if((poscom=dataToSend.find("@cl"))!=-1)
	{
		WhoToSend=dataToSend.substr(3,1);
		dataToSend.erase(poscom,5);
	}
	else if((poscom=dataToSend.find("@"))!=-1)
	{
		WhoToSend=dataToSend.substr(1,1);
		dataToSend.erase(poscom,3);
	}
	if(TypeEnc=="e") dataToSend=encode_string(dataToSend,n,e);

	dataToSend=WhoToSend+TypeEnc+FromWho+dataToSend;
}