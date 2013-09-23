#include "mysocket.h"
int size(char*a,int count)
{
	int i;
	for(i=count-1;i>=0&&a[i]==0;--i)
	return i+1;
}