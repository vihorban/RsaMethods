#include <cmath>
#include <iostream>
#include <ctime>
#include "RSAmethods.h"
long long gcdex (long long a, long long b, long long & x, long long & y)
{
        if (a == 0)
		{
                x = 0; y = 1;
                return b;
        }
        long long x1, y1;
        long long d = gcdex (b%a, a, x1, y1);
        x = y1 - (b / a) * x1;
        y = x1;
        return d;
}
long long ModExp(long long a, long long b, long long n)
{	
	long long	r=1;
	while (b>0)
	if (b&1)
	{
		r=(r*a)%n;
		--b;
	}
	else 
	{
		a=(a*a)%n;
		b>>=1;
	}
	return r;
	/*long long i,r=1,t=a%n;
	for(i=0;i<b;++i)
	{
		r*=t;
		r%=n;
	}
	return r;*/
}
long long inverce(long long a, long long m)
{
	long long x=0,y=0;
	long long g=gcdex(a,m,x,y);
	if (g!=1) return 0;
	return x;
}
long long gcd(long long a, long long b)
{
	if(a<0) a=-a;
	if(b<0) b=-b;
	if (a*b==0) return a+b;
	long long c;
	while (b>0)
	{
		c=a%b;
		a=b;
		b=c;
	}
	return a;
}
bool issquare(long long n)
{
	long long k=(long long)sqrt((double)n);
	if(k*k==n) return true;
	return false;
}
bool ferma(long long n,long long&d1,long long&d2) 
{
   long long x, xx, y, rt,t;
   rt = (long long)sqrt(double(n));
   if (rt*rt==n)
   {
	   d1=d2=rt;
       return true;
   }
   t=(n+1)/2;
   for(x=rt;x<=t;++x)
   {
       xx = (x*x)-n;
	   y = (long long)sqrt(double(xx));
       if(y*y==xx) 
       {
          d1 = (x-y);
		  d2 = (x+y);
          return true;	
       }
   }
   return false;
}
bool is_simple_by_ferma(long long n)
{
	long long a,b;
	ferma(n,a,b);
	if(n<2||a==1||b==1) return true;
	return false;
}
bool solovey (long long p)
{
	srand (unsigned(time (0)));
	long long w=rand()%(p-1)+1;
	if (gcd (p,w)!=1) return 0;
	long long q=ModExp(w,((p-1)/2),p);
	if (q==p-1) q=-1;
	long long ja= Jakobi (w,p); 
	if (q!=ja) return false;
	return true;
}
bool is_simple_by_solovey(long long p, long long n)
{
	bool ok=1;
	long long i;
	for (i=0;i<n&&ok;i++)
	{
		ok=solovey(p);
	}
	if(i==n) return true;
	return false;
}
long long Jakobi (long long a, long long b)
{
	if (b%2==0) return -2;
	if (gcd(a,b)!=1) return 0;
	long long  t=0,c;
	long long r=1;
	if(a<0)
	{
			a=-a;
			if(b%4==3) r=-r;
	}
	while (a!=0)
	{
		t=0;
		while (a&1==0)
		{
			t++;
			a>>=1;
		}
		if (t&1==1)
			if (b%8==3 || b%8==5) r=-r;
		if ((a%4==3)&&(b%4==3)) r=-r;
		c=a; a=b%c; b=c;
	}
	return r;
}
void gen_key_rand(long long & n, long long & e, long long & d)
{
	srand(unsigned(time(0)));
	long long p,q;
	do
	{
		p=rand();
	}
	while(!is_simple_by_solovey(p,10));
	do
	{
		q=rand();
	}
	while(!is_simple_by_solovey(q,10));
	n=p*q;
	long long euler = (p-1)*(q-1);
	do
	{
		e=(rand())%(n-3)+2;
	}
	while (gcd(e,euler)!=1);
    d = inverce(e,euler);
	while (d<0) d+=euler;
}
void gen_key_manual(long long & p, long long & q, long long & n, long long & e, long long & d)
{
	n = p*q;
	long long euler = (p-1)*(q-1);
	if(gcd(e,euler)!=1)
	{
		cout<<"Wrong e. Generating new e\n";
		do
		{
		e=(rand())%(n-3)+2;
		}
		while (gcd(e,euler)!=1);
	}
	d = inverce(e,euler);
	while (d<0) d+=euler;
}
long long RSA_encode(long long m, long long n, long long e)
{
	return ModExp(m,e,n);
}
long long RSA_decode(long long c, long long n, long long  d)
{
	return ModExp(c,d,n); 
}
string encode_string(string data, long long n, long long e)
{
	if(data.size()&1) data.push_back(' ');
	string res;
	int i,k=0,newsize=4*data.size(),temp;
	long long twobytes;
	char* ptwobytes;
	res.resize(newsize);
	for(i=0;i<newsize;++i)
		res[i]=0;
	for(i=0,k=0;i<(int)data.size();i+=2,k+=8)
	{
		temp=256*data[i]+data[i+1];
		twobytes=RSA_encode(temp,n,e);
		ptwobytes=(char*)(&twobytes);
		for(int s=k;s<k+8;++s)
			res[s]=ptwobytes[s-k];
	}
	return res;
}
string decode_string(string data, long long n, long long d)
{
	string res;
	int i,k,s,newsize=data.size()/4,temp;
	long long *twobytes;
	char* ptwobytes=new char[8];
	res.resize(newsize);
	for(i=0;i<newsize;++i)
		res[i]=0;
	for(k=0,s=0;k<(int)data.size();k+=8,s+=2)
	{
		for(i=k;i<k+8;++i)
			ptwobytes[i-k]=data[i];
		twobytes=(long long*)ptwobytes;
		temp=(int)RSA_decode((*twobytes),n,d);
		res[s]=temp/256;
		res[s+1]=temp%256;
	}
	return res;
}