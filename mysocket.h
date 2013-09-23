#include <winsock2.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <conio.h>
#define bufSize 200
using namespace std;
#define ServerPort 800
#define DefaultPort 800
#define DefaultServerAddress "127.0.0.1"
int size(char*,int count=bufSize);