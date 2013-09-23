#include "longn.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
//#include <complex>
#include <cmath>
#include <limits.h>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#define NUMBER 200
//#define DEEP 30
#define PI 3.14159265358979323846264338332795
const int mods = 7340033;
const int root = 5;
const int root_1 = 4404020;
const int root_pw = 1<<20;
int typemult=0;
int base=10;
using namespace std;
LongN::LongN()
{
	size=0;
	sign='+';
}
LongN::LongN(LongN& a)
{
	sign=a.sign;
	size=a.size;
	for(int i=0;i<size;++i)
		num.push_back(a.num[i]);
}
LongN::LongN(int a)
{
	sign=(a>=0)?'+':'-';
	if(a==0) num.push_back(0);
	while(a)
	{
		num.push_back(a%base);
		a/=base;
	}
	size=num.size();
}
void LongN::read_k()
{
	string s0;
	cin>>s0;
	if(s0.length()==0)
	{
		size=1;
		num.push_back(0);
		sign='+';
		return;
	}
	if(s0[0]!='-')
	{
		sign='+';
		size=s0.length();
		for(int i=size-1;i>=0;--i)
			num.push_back(s0[i]-48);
	}
	else
	{
		sign='-';
		size=s0.length()-1;
		for(int i=size;i>=1;--i)
			num.push_back(s0[i]-48);
	}
}
void LongN::write_m()
{
	if(sign=='-') cout<<sign;
	for(int i=size-1;i>=0;--i)
		cout<<num[i];
	cout<<endl;
}
void LongN::conv10_to2(LongN a)
{
	while(a.num.size()!=1||a.num[0]!=0)
	{
		num.push_back(a.num[0]%2);
		int per=0,t,p;
		for(int i=a.size-1;i>=0;--i)
		{
			p=10*per+a.num[i];
			t=p%2;
			a.num[i]=p/2;
			per=t;
		}
		int k;
		for(k=a.size-1;a.num[k]==0&&k>=1;--k)
			a.num.erase(a.num.begin()+k);
		a.size=a.num.size();
	}
	while(num.size()>1&&num.back()==0) num.pop_back();
	size=num.size();
	sign=a.sign;
	base=2;
}
LongN::~LongN()
{
	size=0;
	num.clear();
}
LongN LongN::add(LongN &data)
{
	LongN res;
	int k,temp,per=0;
	if(size>data.size)
		for(k=0;k<size;++k)
		{
			temp=num[k]+per+((k<data.size)?data.num[k]:0);
			per=temp>=base;
			if(per) temp-=base;
			res.num.push_back(temp);
		}
	else
		for(k=0;k<data.size;++k)
		{
			temp=((k<size)?num[k]:0)+data.num[k]+per;
			per=temp>=base;
			if(per) temp-=base;
			res.num.push_back(temp);
		}
	if(per) res.num.push_back(1);
	res.size=res.num.size();
	res.sign=sign;
	return res;
}
LongN LongN::substr(LongN &data)
{
	LongN res;
	res.sign=sign;
	int k,temp,per=0;
	if(size>data.size)
		for(k=0;k<size;++k)
		{
			temp=num[k]-per-((k<data.size)?data.num[k]:0);
			per=temp<0;
			if(per) temp+=base;
			res.num.push_back(temp);
		}
	else
		for(k=0;k<data.size;++k)
		{
			temp=((k<size)?num[k]:0)-data.num[k]-per;
			per=temp<0;
			if(per) temp+=base;
			res.num.push_back(temp);
		}
	if(per) cout<<"Error\n";
	while (res.num.size() > 1 && res.num.back() == 0)
	res.num.pop_back();
	if(res.num.size()==1&&res.num[0]==0) res.sign='+';
	res.size=res.num.size();
	return res;
}
bool LongN::operator >=(LongN &data)
{
	if(size>data.size) return true;
	if(size<data.size) return false;
	int i;
	for(i=size-1;num[i]==data.num[i]&&i>0;--i);
	if(num[i]>=data.num[i])return true;
	else return false;
}
LongN LongN::operator +(LongN &data)
{
	LongN res;
	if(sign==data.sign) res=this->add(data);
	else if(*(this)>=data) res=this->substr(data);
		     else res=data.substr(*(this));
	return res;
}
LongN LongN::operator -(LongN &data)
{
    LongN res;
	if(sign!=data.sign) res=this->add(data);
	else if(*(this)>=data) res=this->substr(data);
		     else 
			 {
				 res=data.substr(*(this));
				 if(data.sign=='+') res.sign='-';
				 else res.sign='+';
	         }
	return res;
}
LongN LongN::operator *(LongN &data)
{
	LongN res;
	if(typemult==0) res=this->usmult(data);
	else if(typemult==1) res=this->caratsubi(data);
	else if(typemult==2) res=this->toomcook(data);
	else if(typemult==3) res=this->shenkhage(data);
	else if(typemult==4) res=this->shtrassen(data);
	return res;
}
LongN LongN::mult10(int k)
{
	LongN res;
	res.sign=sign;
	for(int i=0;i<size;++i)
		res.num.push_back(num[i]);
	res.num.insert(res.num.begin(),k,0);
	while(res.num.size()>1&&res.num.back()==0) res.num.pop_back();
	res.size=res.num.size();
	return res;
}
LongN LongN::div10(int k)
{
	LongN res;
	res.sign=sign;
	if(k>=size) res.num.push_back(0);
	for(int i=0;i<size-k;++i)
		res.num.push_back(num[i+k]);
	res.size=res.num.size();
	return res;
}
LongN LongN::caratsubi(LongN &data)
{
	LongN res;
	if(size<=NUMBER||data.size<=NUMBER) res=this->usmult(data);
	else
	{
		LongN a1,a2,b1,b2,tmp1,tmp2,tmp3;
		int k;
		this->dividecar(data,a1,a2,b1,b2,k);
		tmp1=a1.caratsubi(b1);
		tmp2=a2.caratsubi(b2);
		tmp3=(a1+a2).caratsubi(b1+b2)-tmp1-tmp2;
		res=tmp1+tmp3.mult10(k)+tmp2.mult10(2*k);
	}
	return res;
}
void LongN::dividecar(LongN &data, LongN &a1, LongN &a2, LongN &b1, LongN &b2, int &k)
{
	if(size>=data.size) k=data.size;
	else k=size;
	k/=2;
	a1.sign=sign;
	a2.sign=sign;
	b1.sign=data.sign;
	b2.sign=data.sign;
	for(int i=0;i<k;++i) a1.num.push_back(num[i]);
	for(int i=k;i<size;++i) a2.num.push_back(num[i]);
	for(int i=0;i<k;++i) b1.num.push_back(data.num[i]);
	for(int i=k;i<data.size;++i) b2.num.push_back(data.num[i]);
	a1.size=a1.num.size();
	a2.size=a2.num.size();
	b1.size=b1.num.size();
	b2.size=b2.num.size();
}
void LongN::read_f(char * text)
{
	ifstream f1;
	f1.open(text);
	string s0;
	getline(f1,s0);
	if(s0.length()==0)
	{
		size=1;
		num.push_back(0);
		sign='+';
		return;
	}
	if(s0[0]!='-')
	{
		sign='+';
		size=s0.length();
		for(int i=size-1;i>=0;--i)
			num.push_back(s0[i]-48);
	}
	else
	{
		sign='-';
		size=s0.length()-1;
		for(int i=size;i>=1;--i)
			num.push_back(s0[i]-48);
	}
	f1.close();
}
void LongN::write_f(char * text)
{
	ofstream f1;
	f1.open(text);
	if(sign=='-') f1<<sign;
	for(int i=size-1;i>=0;--i)
		f1<<num[i];
	f1<<endl;
	f1.close();
}

LongN LongN::toomcook(LongN &data)
{
	LongN res;
	if(size<=NUMBER||data.size<=NUMBER) res=this->usmult(data);
	else
	{
		LongN a1,a2,a3,b1,b2,b3,tmp1,tmp2,tmp3,tmp4,tmp5,tmp6;
		int k;
		this->dividetom(data,a1,a2,a3,b1,b2,b3,k);
		tmp1=a1.toomcook(b1);
		tmp2=a2.toomcook(b2);
		tmp3=a3.toomcook(b3);
		tmp4=(a1+a2).toomcook(b1+b2)-tmp1-tmp2;
		tmp5=(a2+a3).toomcook(b2+b3)-tmp3-tmp2;
		tmp6=(a1+a3).toomcook(b1+b3)+tmp2-tmp1-tmp3;
		res=tmp1+tmp4.mult10(k)+tmp6.mult10(2*k)+tmp5.mult10(3*k)+tmp3.mult10(4*k);
	}
	return res;
}
void LongN::dividetom(LongN &data, LongN &a1, LongN &a2, LongN &a3, LongN &b1, LongN &b2, LongN &b3, int &k)
{
	if(size>=data.size) k=data.size;
	else k=size;
	k/=3;
	a1.sign=sign;
	a2.sign=sign;
	a3.sign=sign;
	b1.sign=data.sign;
	b2.sign=data.sign;
	b3.sign=data.sign;
	for(int i=0;i<k;++i) a1.num.push_back(num[i]);
	for(int i=k;i<2*k;++i) a2.num.push_back(num[i]);
	for(int i=2*k;i<size;++i) a3.num.push_back(num[i]);
	for(int i=0;i<k;++i) b1.num.push_back(data.num[i]);
	for(int i=k;i<2*k;++i) b2.num.push_back(data.num[i]);
	for(int i=2*k;i<data.size;++i) b3.num.push_back(data.num[i]);
	a1.size=a1.num.size();
	a2.size=a2.num.size();
	a3.size=a3.num.size();
	b1.size=b1.num.size();
	b2.size=b2.num.size();
	b3.size=b3.num.size();
}

LongN LongN::div(int d, int &per)
{
	LongN res;
	per = 0;
	for (int i=size-1; i>=0; --i)
	{
		int cur = num[i]+per*base;
		res.num.insert(res.num.begin(),1,cur/d);
		per=cur%d;
	}
	while (res.num.size() > 1 && res.num.back() == 0)
	res.num.pop_back();
	res.sign=sign;
	res.size=res.num.size();
	return res;
}
LongN LongN::div(int d)
{
	LongN res;
	int per = 0;
	for (int i=size-1; i>=0; --i)
	{
		int cur = num[i]+per*base;
		res.num.insert(res.num.begin(),1,cur/d);
		per=cur%d;
	}
	while (res.num.size() > 1 && res.num.back() == 0)
	res.num.pop_back();
	res.sign=sign;
	res.size=res.num.size();
	return res;
}
LongN LongN::sqrt()
{ 
	LongN res;
	int i,temp,sq;
	if(size<=2)
	{
		res.num.push_back(dsqrt(10*((size==2)?num[1]:0)+num[0],sq));
	}
	else
	{
	if(size%2)
	{
		i=size-2;
		temp=num[size-1];
	}
	else
	{
		i=size-3;
		temp=10*num[size-1]+num[size-2];
	}
	res.num.push_back(dsqrt(temp,sq));
	res.size=res.num.size();
	LongN tmp,mult;
	tmp.convtolong(temp);
	mult.convtolong(sq);
	for(;i>=0;i-=2)
	{
		tmp=tmp-mult;
		tmp.num.insert(tmp.num.begin(),1,num[i]);
		tmp.num.insert(tmp.num.begin(),1,num[i-1]);
		while(tmp.num.back()==0&&tmp.num.size()>1) tmp.num.pop_back();
		tmp.size=tmp.num.size();
		int next;
		bool ok1=true,ok2=true;
		mult=(res*2).mult10(1);
		for(next=0;ok1&&ok2;++next)
		{
			LongN d=(mult+next)*next;
			ok1=(tmp>=d);
			ok2=next<10;
		};
		next-=2;
		mult=(mult+next)*next;
		res.num.insert(res.num.begin(),1,next);
		res.size=res.num.size();
	}
	}
	res.sign=sign;
	res.size=res.num.size();
	return res;
}
int dsqrt(int a,int&sq)
{
	if(a<1) {sq=0;return 0;}
	if(a<4) {sq=1;return 1;}
	if(a<9) {sq=4;return 2;}
	if(a<16) {sq=9;return 3;}
	if(a<25) {sq=16;return 4;}
	if(a<36) {sq=25;return 5;}
	if(a<49) {sq=36;return 6;}
	if(a<64) {sq=49;return 7;}
	if(a<81) {sq=64;return 8;}
	sq=81;
	return 9;
}

void LongN::convtolong(int a)
{
	num.clear();
	sign=(a>=0)?'+':'-';
	if(a==0) num.push_back(0);
	while(a)
	{
		num.push_back(a%base);
		a/=base;
	}
	size=num.size();
}
LongN LongN::operator *(int a)
{
	LongN res,data;
	if(a==0)
	{
		res.num.push_back(0);
		res.size=1;
	}
	else
	{
	data.convtolong(a);
	res=(*(this))*data;
	}
	return res;
}
LongN LongN::operator +(int a)
{
	LongN res,data;
	data.convtolong(a);
	res=(*(this))+data;
	return res;
}
LongN LongN::operator -(int a)
{
	LongN res,data;
	data.convtolong(a);
	res=(*(this))-data;
	return res;
}
bool LongN::operator ==(LongN &data)
{
	if(size!=data.size) return 0;
	if(sign!=data.sign) return 0;
	for(int i=0;i<size;++i)
		if(num[i]!=data.num[i]) return 0;
	return 1;
}
LongN LongN::usmult(LongN &data)
{
	LongN res;
	for(int i=0;i<size;++i)
	{
		int temp=0;
		for(int k=0;k<data.size;++k)
		{
			temp=num[i]*data.num[k]+temp;
			if(i+k>=(int)res.num.size())
				res.num.push_back(temp%base);
			else res.num[i+k]+=temp%base;
			temp/=base;
		}
		if(temp)
			if(i+data.size>=(int)res.num.size())
				res.num.push_back(temp);
			else res.num[i+data.size]+=temp;
	}
	int p=res.num.size()-1;
	for(int s=0;s<p;++s)
	{
		int r=res.num[s];
		if(r>=base)
		{
			res.num[s+1]+=r/base;
			res.num[s]%=base;
		}
	}
	if(res.num[p]>=base)
	{
		res.num.push_back(res.num[p]/base);
		res.num[p]%=base;
	}
	while(res.num.size()>1&&res.num.back()==0) res.num.pop_back();
	res.size=res.num.size();
	if(sign==data.sign) res.sign='+';
	else res.sign='-';
	return res;
}

LongN LongN::shtrassen(LongN& data) 
{
	LongN res;
	mytype *one, *two;
	int n = 1;
	while (n < (int)max (num.size(), data.num.size()))  n <<= 1;
	n <<= 1;
	one=new mytype[n];  
	two=new mytype[n];
	for(int i=0;i<(int)size;++i) one[i]=mytype(num[i]);
	for(int i=0;i<(int)data.size;++i) two[i]=mytype(data.num[i]);
	fft (one, false,n),  fft (two, false,n);
	for (int i=0; i<n; ++i)
		one[i]*=two[i];
	fft (one, true,n);
	res.num.resize (n);
	for (int i=0; i<n; ++i)
		res.num[i] = int (one[i].real() + 0.5);
	int per = 0;
	for (int i=0; i<n; ++i)
	{
		res.num[i] += per;
		per = res.num[i] / base;
		if(per) res.num[i] %= base;
	}
	while(res.num.back()==0) res.num.pop_back();
	res.size=res.num.size();
	res.sign='+';
	return res;
}
Complex::Complex():Re(0.0),Im(0.0){}
Complex::Complex(double x):Re(x),Im(0.0){}
Complex::Complex(double x,double y):Re(x),Im(y){}
Complex::Complex(Complex &a)
{
	Re=a.Re;
	Im=a.Im;
}
Complex Complex::operator +(Complex &data)
{
	Complex res(Re+data.Re,Im+data.Im);
	return res;
}
Complex Complex::operator -(Complex &data)
{
	Complex res(Re-data.Re,Im-data.Im);
	return res;
}
Complex Complex::operator *(Complex &data)
{
	Complex res(Re*data.Re-Im*data.Im,Re*data.Im+Im*data.Re);
	return res;
}
Complex Complex::operator /(int a)
{
	Complex res(Re/a,Im/a);
	return res;
}
Complex Complex::operator *=(Complex &data)
{
	Complex res(Re*data.Re-Im*data.Im,Re*data.Im+Im*data.Re);
	Re=res.Re;
	Im=res.Im;
	return *this;
}
Complex Complex::operator /=(int a)
{
	Re/=a;
	Im/=a;
	return *(this);
}
double Complex::real()
{
	return Re;
}
Complex::~Complex()
{
	Re=0;
	Im=0;
}
void fft (mytype *a, bool invert,int n) {
	for (int i=1, j=0; i<n; ++i) {
		int bit = n >> 1;
		for (; j>=bit; bit>>=1)
			j -= bit;
		j += bit;
		if (i < j)
			swap (a[i], a[j]);
	}
	for (int len=2; len<=n; len<<=1) {
		double ang = 2*PI/len * (invert ? -1 : 1);
		mytype temp (cos(ang), sin(ang));
		for (int i=0; i<n; i+=len) {
			mytype w (1,0);
			for (int j=0; j<len/2; ++j) {
				mytype u(a[i+j]);
				mytype v(a[i+j+len/2]*w);
				a[i+j] = u + v;
				a[i+j+len/2] = u - v;
				w*=temp;
			}
		}
	}
	if (invert)
		for (int i=0; i<n; ++i)
			a[i]/=n;
}
void swap(int&a,int&b)
{
	int t=a;
	a=b;
	b=t;
}
void swap(Complex &a,Complex &b)
{
	Complex t(a);
	a=b;
	b=t;
}
LongN LongN::div(LongN &data)
{
	LongN res;
	if(data>*(this)) res.convtolong(0);
	else
	{
		int i=size-data.size;
		LongN tmp1;
		tmp1=this->div10(i);
		for(int k=1;i>=0;--i,++k)
		{
			tmp1.size=tmp1.num.size();
			int d;
			if(tmp1.size>data.size)
				d=(tmp1.num[tmp1.size-1]*base+tmp1.num[tmp1.size-2])/(data.num[data.size-1]+1);
			else if(tmp1.size==data.size)
				d=tmp1.num[tmp1.size-1]/(data.num[data.size-1]+1);
			else d=0;
			while(data*d<=tmp1&&d<base) d++;
			d--;
			tmp1=tmp1-data*d;
			if(i)tmp1=tmp1.mult10(1)+num[i-1];
			res.num.insert(res.num.begin(),1,d);
		}
	}
	while(res.num.back()==0&&res.num.size()>1) res.num.pop_back();
	res.size=res.num.size();
	return res;
}
LongN LongN::mod(LongN &data)
{
	if(data>*(this)) return *(this);
	else
	{
		int i=size-data.size;
		LongN tmp1;
		tmp1=this->div10(i);
		for(int k=1;i>=0;--i,++k)
		{
			tmp1.size=tmp1.num.size();
			int d;
			if(tmp1.size>data.size)
				d=(tmp1.num[tmp1.size-1]*base+tmp1.num[tmp1.size-2])/(data.num[data.size-1]+1);
			else if(tmp1.size==data.size)
				d=tmp1.num[tmp1.size-1]/(data.num[data.size-1]+1);
			else d=0;
			while(data*d<=tmp1&&d<base) d++;
			d--;
			tmp1=tmp1-data*d;
			if(i)tmp1=tmp1.mult10(1)+num[i-1];
		}
		tmp1.size=tmp1.num.size();
		return tmp1;
	}
}
LongN GSD(LongN & data1, LongN &data2)
{
	if(data1==0) return data2;
	else if(data2==0) return data1;
	if(data1==1||data2==1) 
	{
		LongN tmp(1);
		return tmp;
	}
	int mod1,mod2;
	LongN div1=data1.div(2,mod1);
	LongN div2=data2.div(2,mod2);
	if(mod1==1&&mod2==1) return (data1>=data2)?GSD(data2,data1-data2):GSD(data2-data1,data1);
	if(mod1==0&&mod2==0) return GSD(div2,div1)*2;
	if(mod1==1) return GSD(data1,div2);
	return GSD(div1,data2);
}
bool LongN::operator ==(int a)
{
	LongN tmp;
	tmp.convtolong(a);
	return (tmp==(*this));
}

bool LongN::operator >(LongN &data)
{
	if(size>data.size) return true;
	if(size<data.size) return false;
	int i;
	for(i=size-1;num[i]==data.num[i]&&i>0;--i);
	if(num[i]>data.num[i])return true;
	else return false;
}
bool LongN::operator <(LongN &data)
{
	if(size>data.size) return false;
	if(size<data.size) return true;
	int i;
	for(i=size-1;num[i]==data.num[i]&&i>0;--i);
	if(num[i]<data.num[i])return true;
	else return false;
}
bool LongN::operator <=(LongN &data)
{
	if(size>data.size) return false;
	if(size<data.size) return true;
	int i;
	for(i=size-1;num[i]==data.num[i]&&i>0;--i);
	if(num[i]<=data.num[i])return true;
	else return false;
}
LongN dicson(LongN &n) 
{
   LongN x, xx, y, d, q, rt,one(1);
   //int i, j;
   rt = n.sqrt();
   if (issquare(n)) 
       return rt;
   x = rt;
   while(x<n) 
   {
	   x=x+1;
       d = GSD(x, n);
       if (one<d && d<n) 
          return d;
       xx = (x*x)%n;
       if(issquare(xx)) 
       {
		  y = xx.sqrt();
          q = (x-y)%n;
          d = GSD(q, n);
          if(one<d && d<n) 
             return d;	
       }
   }
   LongN tmp(0);
   return tmp;
}
bool issquare(LongN n)
{
	LongN tmp;
	tmp=n.sqrt();
	return(tmp*tmp==n);
}
int LongN::mod(int a)
{
	int p;
	this->div(a,p);
	return p;
}
LongN LongN::operator /(LongN &data)
{
	LongN res;
	res=this->div(data);
	return res;
}
LongN LongN::operator %(LongN &data)
{
	LongN res;
	res=this->mod(data);
	return res;
}
LongN longrand(LongN & data)
{
	LongN res;
	res.sign='+';
	res.size=data.size;
	res.num.resize(res.size);
	for(int i=0;i<res.size;++i)
		res.num[i]=rand()%base;
	while(res>=data) res=res-data;
	while(res.num.back()==0&&res.num.size()>1) res.num.pop_back();
	res.size=res.num.size();
	return res;
}
LongN func(LongN& x,LongN& data,int c)
{
	return((x*x+c)%data);
}
LongN pollard(LongN& data)
{
	LongN x0,x1,d,dif;
	int j=7,k=100;
	for(int s=1; s<k;++s)
	{
		x0=longrand(data);
	for(int i=0; i<j;++i)
	{
		x1=func(x0,data,s);
		if(x1>=x0) dif=x1-x0;
		else dif=x0-x1;
		d=GSD(dif,data);
		dif.convtolong(1);
		if(d>dif) return d;
		x0=x1;
	}
	}
	return dif;
}
LongN& LongN::operator =(LongN& data)
{
	if(this == &data) return *this;
	num.clear();
	num.resize(data.size);
	for(int i=0;i<data.size;++i)
	{
		num[i]=data.num[i];
	}
	sign=data.sign;
	size=data.size;
	return *(this);
}

int LongN::convtoint()
{
	if(*this>int_max) return 0;
	int res=0;
	for(int i=0;i<size;++i)
		res+=pw(base,i)*num[i];
	return res;
}
int pw(int x, int i)
{
	int res=1;
	for(int k=0;k<i;++k) res*=x;
	return res;
}
vector<int> erat (int n)
{
	vector<char> prime (n+1, true);
	vector<int> primebase;
	prime[0] = prime[1] = false;
	for (int i=2; i<=n; ++i)
		if (prime[i])
		{
			primebase.push_back(i);
			for (int j=i+i; j<=n; j+=i)
				prime[j] = false;
		}
	return primebase;
}
vector<int> factbase;//=erat(INT_MAX/10000);
LongN quadraticsieve(LongN& data)
{
	int DEEP=5;
	while(1)
	{
	factbase=erat(10000);
	int k,i;
	for(i=0,k=0;k<DEEP&&i<(int)factbase.size()&&LongN(factbase[i])<=data;++i)
	{
		if (legandr(data,factbase[i])==1) k++;
		else
		{
			factbase.erase(factbase.begin()+i);
			--i;
		}
	}
	if((int)factbase.size()>k) factbase.erase(factbase.begin()+k,factbase.end());
	int a=DEEP*factbase[factbase.size()-1];
	LongN x0(data.sqrt()+1);
	LongN x(x0);
	LongN *tmp=new LongN[a];
	tmp[0]=x*x-data;
	for(int i=1;i<a;++i)
	{
		tmp[i]=tmp[i-1]+x*2+1;
		x=x+1;
	}
	vector<int> sieve(a);
	for(int i=0;i<a;++i) sieve[i]=tmp[i].convtoint();
	vector<int> sievecopy(sieve.begin(),sieve.end());
	vector<vector<int>> matrix(a);
	for(int i=0;i<a;++i) matrix[i].resize(factbase.size());
	for(int i=0;i<(int)factbase.size();)
	{
		int ok(1);
		while(ok==1)
		{
			ok=0;
			for(int s=0;s<a;++s)
				if(sievecopy[s]%factbase[i]==0)
				{
					ok=1;
					sievecopy[s]/=factbase[i];
					matrix[s][i]++;
				}
		}
		i++;
	}
	vector<int> index(a);
	for(int i=0;i<a;++i) index[i]=i;
	for(int i=0;i<(int)sievecopy.size();++i)
	{
		if(sievecopy[i]!=1)
		{
			sievecopy.erase(sievecopy.begin()+i);
			matrix.erase(matrix.begin()+i);
			sieve.erase(sieve.begin()+i);
			index.erase(index.begin()+i);
			--i;
		}
	}
	int n=matrix.size(),m=factbase.size();
	if(n==0)
	{
		cout<<"Small factor base\nIncrease it?\n";
		char coord;
		cin>>coord;
		if(coord=='y'||coord=='Y')
		{
		DEEP+=5;
		continue;
		}
		else break;
	}
	for(int j=0;j<m;++j)
		for(int i=0;i<n;++i)
			matrix[i][j]%=2;
	for(int i=0;i<n;++i)
	{
		int s=0;
		for(int j=0;j<m;++j) s+=matrix[i][j];
		if(s==0)
		{
			int a=(int)sqrt(double(sieve[i]));
			x=x0+index[i];
			LongN gsd1(GSD(x+a,data));
			if(gsd1!=1&&gsd1!=data)
				return gsd1;
		}
	}
	int *used=new int[m];
	for(int r=0;r<m;++r) used[r]=0;
	for(int k=0;k<n;++k)
	{
		for(int r=0;r<m;++r)
			if(matrix[k][r]&&(used[r]==0))
			{
				for(int i=0;i<m;++i)
					if(matrix[k][i]&&(i!=r))
						for(int j=0;j<n;++j)
							matrix[j][i]=matrix[j][i]^matrix[j][r];
				used[r]=1;
			}
	}
	int numlinnezal=0,ok=1;
	for(int i=0;(i<n)&&ok;++i)
	{
		int s=0;
		for(int j=0;j<m;++j) s+=matrix[i][j];
		if(s==1) numlinnezal++;
		else ok=0;
	}
	int numfundsystrozv=n-numlinnezal;
	if(numfundsystrozv==0)
	{
		cout<<"Small factor base\nIncrease it?\n";
		char coord;
		cin>>coord;
		if(coord=='y'||coord=='Y')
		{
		DEEP+=5;
		continue;
		}
		else break;
	}
	else
	{
		int indexofbasis=0;
		int *basis=new int[numfundsystrozv];
		while(indexofbasis<numfundsystrozv)
		{
		for(int i=0;i<numfundsystrozv;++i)
			if(i==indexofbasis)
			{
				basis[i]=1;
				sievecopy[numlinnezal+i]=1;
			}
			else
			{
				basis[i]=0;
				sievecopy[numlinnezal+i]=0;
			}
		for(int i=0;i<numlinnezal;++i)
			{
				int j=0;
				while(matrix[i][j]!=1) j++;
				int sum=0;
				for(int p=0;p<numfundsystrozv;++p)
				{
					sum+=matrix[numlinnezal+p][j]&&basis[p];
					sievecopy[i]=sum%2;
				}
			}
		LongN X(1),A(1);
		for(int i=0;i<n;++i)
		{
			if(sievecopy[i])
			{
				A=A*sieve[i];
				x=x0+index[i];
				X=X*x;
			}	
		}
		A=A.sqrt();
		LongN gsd1(GSD(X+A,data));
		if(gsd1!=1&&gsd1!=data)
		return gsd1;
		indexofbasis++;
		}
		for(int i=0;i<numfundsystrozv;++i)
		{
			basis[i]=1;
			sievecopy[numlinnezal+i]=1;
		}
		for(int i=0;i<numlinnezal;++i)
			{
				int j=0;
				while(matrix[i][j]!=1) j++;
				int sum=0;
				for(int p=0;p<numfundsystrozv;++p)
				{
					sum+=matrix[numlinnezal+p][j]&&basis[p];
					sievecopy[i]=sum%2;
				}
			}
		LongN X(1),A(1);
		for(int i=0;i<n;++i)
		{
			if(sievecopy[i])
			{
				A=A*sieve[i];
				x=x0+index[i];
				X=X*x;
			}	
		}
		A=A.sqrt();
		LongN gsd1(GSD(X+A,data));
		if(gsd1!=1&&gsd1!=data)
		return gsd1;
		return LongN(1);
	}
	}
	return LongN(1);
}
int legandr(LongN data, int p)
{
	data=data%(LongN(p));
	int t=data.convtoint(),res=1;
	for(int i=1;i<p;i+=2)
	{
		res*=t;
		res%=p;
	}
	return res;
}
void testmult(int ok1,int ok2)
{
	LongN a,b,c,d,e,f,g;
	time_t starttime,finishtime,us,cr,tk,sh,st;
	if(ok1){
		a.read_f("input1.txt");
		b.read_f("input2.txt");
	}
	else
	{
		cout<<"Enter Numbers:\n";
		a.read_k();
		b.read_k();
	}
	typemult=0;
	starttime=clock();
	c=a*b;
	finishtime=clock();
	if(ok2)c.write_m();
	us=finishtime-starttime;
	typemult=1;
	starttime=clock();
	d=a*b;
	finishtime=clock();
	if(ok2)d.write_m();
	cr=finishtime-starttime;
	typemult=2;
	starttime=clock();
	e=a*b;
	finishtime=clock();
	if(ok2)e.write_m();
	tk=finishtime-starttime;
	typemult=3;
	starttime=clock();
	g=a*b;
	finishtime=clock();
	if(ok2)g.write_m();
	sh=finishtime-starttime;
	typemult=4;
	starttime=clock();
	f=a*b;
	finishtime=clock();
	if(ok2)f.write_m();
	st=finishtime-starttime;
	cout<<"Method: usual\ttime= "<<us<<" clocks\n";
	cout<<"Method: Caratsubi\t";
	if(c==d) cout<<"time= "<<cr<<" clocks\n";
	else cout<<"Error\n";
	cout<<"Method: ToomCoock\t";
	if(c==e) cout<<"time= "<<tk<<" clocks\n";
	else cout<<"Error\n";
	cout<<"Method: Shenckhage\t";
	if(c==g) cout<<"time= "<<sh<<" clocks\n";
	else cout<<"Error\n";
	cout<<"Method: Shtrassen\t";
	if(c==f) cout<<"time= "<<st<<" clocks\n";
	else cout<<"Error\n";
}
LongN LongN::shenkhage(LongN &data)
{
	LongN res;
	vector<int> fa(num.begin(),num.end()),  fb(data.num.begin(),data.num.end());
	int n = 1;
	while (n < (int)max (num.size(), data.num.size()))  n <<= 1;
	n <<= 1;
	fa.resize(n);
	fb.resize(n);
	modul(fa);
	modul(fb);
	for (int i=0;i<n; ++i)
		fa[i]=int(long long(1)*fa[i]*fb[i]%mods);
	returnmodul(fa);
	res.num.resize(n);
	for (int i=0; i<n; ++i)
		res.num[i] = fa[i];
	int per=0;
	for (int i=0; i<n; ++i)
	{
		while(res.num[i]<0) res.num[i]+=mods;
		res.num[i]+=per;
		per=res.num[i]/base;
		res.num[i]%=base;
	}
	while (res.num.size()>1&&res.num.back()==0) res.num.pop_back();
	res.size=res.num.size();
	return res;
}
void menumult()
{
	char ok2='q';
	while(ok2!='0')
	{
	cout<<"Choose:\n1-test\n2-manual\n0-back\n";
	cin>>ok2;
	switch(ok2)
	{
	case '1':
		{
			char ok3='n',ok4='n';
			cout<<"Numbers from keyboard?(y or n):\n";
			cin>>ok3;
			cout<<"Print numbers on screen?(y or n):\n";
			cin>>ok4;
			testmult((ok3=='Y'||ok3=='y')?0:1,(ok4=='Y'||ok4=='y')?1:0);
			break;
		 }
	case '2':
		{
			LongN num1,num2,res;
			cout<<"Enter Numbers:\n";
			num1.read_k();
			num2.read_k();
			time_t starttime,finishtime;
		    char t;
		    int s=5;
		    while(s<0||s>4)
		    {
				cout<<"Enter method:\n0-usual\n1-Caratsubi\n2-ToomCook\n3-Shenckhage\n4-Shtrassen\n";
				cin>>t;
				s=t-48;
			}
			typemult=s;
			starttime=clock();
			res=num1*num2;
			finishtime=clock();
			res.write_m();
			cout<<"time "<<finishtime-starttime<<" clocks\n";
			typemult=0;
			break;
		}
	case 'c':
		{
			system("cls");
			break;
		}
	case '0':
		{
		break;
			 }
	default:
		{
		cout<<"Wrong choise. Try again:\n";
		break;
			}
	}
	}
}
void menufact()
{
	cout<<"Enter number:\n";
	LongN num,res;
	num.read_k();
	char meth='0';
	while(meth<'1'||meth>'5')
	{
	cout<<"Enter method:\n1-Dicson\n2-Pollard\n3-Quadratic Sieve\n4-Eliptic\n5-Eliptic with parameters\n";
	cin>>meth;
	}
	switch(meth)
	{
	case '1':
		{
			res=dicson(num);
			break;
		}
	case '2':
		{
			res=pollard(num);
			break;
		}
	case '3':
		{
			res=quadraticsieve(num);
			break;
		}
	case '4':
		{
			res=ElCur(num,LongN(50),LongN(50));
			break;
		}
	case '5':
		{
			LongN B,C;
			cout<<"Enter B:\n";
			B.read_k();
			cout<<"Enter C:\n";
			C.read_k();
			res=ElCur(num,B,C);
			break;
		}
	case 'c':
		{
			system("cls");
			break;
		}
	default:
		{
			cout<<"Wrong choise\n";
			break;
		}
	}
	res.write_m();
}
void menu()
{
	srand(unsigned(time(0)));
	char ok1='q';
	while(ok1!='0')
	{
	cout<<"Choose:\n1-multiply\n2-division\n3-factorisation\n0-exit\n";
	cin>>ok1;
	switch(ok1)
	{
	case '1':
		{
		menumult();
		break;
		 }
	case '2':
		{
		menudiv();
		break;
		 }
	case '3':
		{
		menufact();
		break;
		 }
	case 'c':
		{
			system("cls");
			break;
		}
	case '0':
		{
		break;
			 }
	default:
		{
		cout<<"Wrong choise. Try again:\n";
		break;
			}
	}
	}
	cout<<"Press any key\n";
	_getch();
}
void modul(vector<int>&data)
{
	int n = (int) data.size();
	for (int i=1, j=0; i<n; ++i)
	{
		int bit = n >> 1;
		for (; j>=bit; bit>>=1)
			j -= bit;
		j += bit;
		if (i < j)
			swap (data[i], data[j]);
	}
	for (int len=2; len<=n; len<<=1)
	{
		int temp = root;
		for (int i=len; i<root_pw; i<<=1)
			temp =int(temp*long long(1)*temp%mods);
		for (int i=0; i<n; i+=len)
		{
			int w = 1;
			for (int j=0; j<len/2; ++j)
			{
				int u=data[i+j],  v=int(data[i+j+len/2]*long long(1)*w%mods);
				data[i+j]=(u+v<mods)?u+v:u+v-mods;
				data[i+j+len/2]=(u-v>=0)?u-v:u-v+mods;
				w=int(w*long long(1)*temp%mods);
			}
		}
	}
}
void returnmodul(vector<int> & data)
{
	int n=(int)data.size();
	for (int i=1, j=0; i<n; ++i)
	{
		int bit = n >> 1;
		for (; j>=bit; bit>>=1)
			j -= bit;
		j += bit;
		if (i < j)
			swap (data[i], data[j]);
	}
	for (int len=2; len<=n; len<<=1)
	{
		int temp=root_1;
		for (int i=len; i<root_pw; i<<=1)
			temp=int(temp*long long(1)*temp%mods);
		for (int i=0; i<n; i+=len)
		{
			int w=1;
			for (int j=0; j<len/2; ++j)
			{
				int u=data[i+j], v=int(data[i+j+len/2]*long long(1)*w%mods);
				data[i+j]=(u+v<mods)?u+v:u+v-mods;
				data[i+j+len/2]=(u-v>=0)?u-v:u-v+mods;
				w=int(w*long long(1)*temp%mods);
			}
		}
	}
	int nrev=reverse(n,mods);
	for (int i=0;i<n;++i)
		data[i]=int(data[i]*long long(1)*nrev%mods);
}
int reverse(int n,int mod)
{
	int x,y;
	int d=gcdex(n,mod,x,y);
	if(d!=1) return 0;
	return x;
}
int gcdex(int a,int b,int & x,int & y) 
{
	if(a==0) 
	{
		x=0;
		y=1;
		return b;
	}
	int x1, y1;
	int d=gcdex(b%a,a,x1,y1);
	x=y1-(b/a)*x1;
	y=x1;
	return d;
}
LongN int_max(INT_MAX-1);
bool LongN::operator >(int a)
{
	return(*this>LongN(a));
}
bool LongN::operator <(int a)
{
	return(*this<LongN(a));
}
bool LongN::operator >=(int a)
{
	return(*this>=LongN(a));
}
bool LongN::operator <=(int a)
{
	return(*this<=LongN(a));
}
LongN& LongN::operator =(int a)
{
	LongN res(a);
	size=res.size;
	sign=res.sign;
	num.resize(size);
	for(int i=0;i<size;++i)
		num[i]=res.num[i];
	return *(this);
}
LongN LongN::operator /(int a)
{
	return *this/LongN(a);
}
LongN LongN::operator %(int a)
{
	return *this%LongN(a);
}
PointLongN Inv(PointLongN a,int n)
{
	PointLongN b(a),x,two(2),help,help1;
	int power=1,v2=((int)b.size-2>0)?b.num[b.size-2]:0,v3=((int)b.size-3>0)?b.num[b.size-3]:0;
	x=PointLongN(32/(4+2*v2+v3))/PointLongN(4);
	//для збільшення точності, збільшити к-ть ітерацій циклу
		for (int i=0;i<n;i++)
		{
			help=two*x;
			help1=x*x*b;
			x=help-help1;
			x=x.normalise(a,i);
		}
	return x;
}
PointLongN DivInv(PointLongN a,PointLongN b,int n)
{
	PointLongN help;
	help=Inv(b,n+a.size);
	help=a*help;
	while((int)help.pointpos>=n+a.size)
	{
		help.pointpos--;
		help.num.erase(help.num.begin());
		help.size--;
	}
	return help;
}
PointLongN PointLongN::operator-(PointLongN &b)
	{
		LongN x,y(b);
		for (int i=0;i<size;i++)
		{
			x.num.push_back(num[i]);
		}
		x.sign=sign;
		x.size=size;
		int pointposx=pointpos,pointposy=b.pointpos;
		if(pointposx>pointposy)
		{
			y=y.mult10(pointposx-pointposy);
			pointposy=pointposx;
		}
		if(pointposx<pointposy)
		{
			x=x.mult10(pointposy-pointposx);
			pointposx=pointposy;
		}
		while(x.num.back()==0&&x.num.size()>1) x.num.pop_back();
		while(y.num.back()==0&&y.num.size()>1) y.num.pop_back();
		x.size=x.num.size();
		y.size=y.num.size();
		LongN help(x-y);
		PointLongN help1;
		help1.num.clear();
		for (int i=0;i<help.size;i++)
		{
			help1.num.push_back(help.num[i]);
		}
		help1.pointpos=pointposx;
		while(help1.pointpos>=help1.num.size()) help1.num.push_back(0);
		while(help1.num[0]==0&&help1.pointpos>0)
		{
			help1.num.erase(help1.num.begin());
			help1.pointpos--;
		}
		help1.size=help1.num.size();
		help1.sign=help.sign;
		return help1;
	}
PointLongN PointLongN::operator+(PointLongN &b)
	{
		LongN x,y(b);
		for (int i=0;i<size;i++)
		{
			x.num.push_back(num[i]);
		}
		x.sign=sign;
		x.size=size;
		int pointposx=pointpos,pointposy=b.pointpos;
		if(pointposx>pointposy)
		{
			y=y.mult10(pointposx-pointposy);
			pointposy=pointposx;
		}
		if(pointposx<pointposy)
		{
			x=x.mult10(pointposy-pointposx);
			pointposy=pointposx;
		}
		while(x.num.back()==0&&x.num.size()>1) x.num.pop_back();
		while(y.num.back()==0&&y.num.size()>1) y.num.pop_back();
		x.size=x.num.size();
		y.size=y.num.size();
		LongN help(x+y);
		PointLongN help1;
		help1.num.clear();
		for (int i=0;i<help.size;i++)
		{
			help1.num.push_back(help.num[i]);
		}
		while(help1.pointpos>=help1.num.size()) help1.num.push_back(0);
		help1.pointpos=pointposx;
		while(help1.num[0]==0&&help1.pointpos>0)
			help1.num.erase(help1.num.begin());
		help1.sign=help.sign;
		return help1;
	}
PointLongN PointLongN::operator*(PointLongN &b)
	{
		LongN x,y(b);
		for (int i=0;i<size;i++)
		{
			x.num.push_back(num[i]);
		}
		x.sign=sign;
		x.size=size;
		int pointposx=pointpos+b.pointpos;
		while(x.num.back()==0&&x.num.size()>1) x.num.pop_back();
		while(y.num.back()==0&&y.num.size()>1) y.num.pop_back();
		x.size=x.num.size();
		y.size=y.num.size();
		LongN help(x*y);
		//LongN help(x.shtrassen(y));
		PointLongN help1;
		help1.num.clear();
		for (int i=0;i<help.size;i++)
		{
			help1.num.push_back(help.num[i]);
		}
		help1.pointpos=pointposx;
		while(help1.pointpos>=help1.num.size()) help1.num.push_back(0);
		while(help1.num[0]==0&&help1.pointpos>0)
		{
			help1.num.erase(help1.num.begin());
			help1.pointpos--;
		}
		help1.size=help1.num.size();
		help1.sign=help.sign;
		return help1;
	}
PointLongN PointLongN::operator/(PointLongN &b)
	{
		LongN x,y(b);
		for (int i=0;i<size;i++)
		{
			x.num.push_back(num[i]);
		}
		x.sign=sign;
		x.size=size;
		int pointposx=pointpos,pointposy=b.pointpos;
		if(pointposx>pointposy)
		{
			y=y.mult10(pointposx-pointposy);
			pointposy=pointposx;
		}
		if(pointposx<pointposy)
		{
			x=x.mult10(pointposy-pointposx);
			pointposy=pointposx;
		}
		while(x.num.back()==0&&x.num.size()>1) x.num.pop_back();
		while(y.num.back()==0&&y.num.size()>1) y.num.pop_back();
		x.size=x.num.size();
		y.size=y.num.size();
		if ((x==0)||(y==0)) return PointLongN(0);
		LongN help;
		int precision=50;
		int more_precision=((y.size-x.size)>0)?y.size-x.size:0;
			pointposx+=precision+more_precision;
			x=x.mult10(precision+more_precision);
			help=x/y;
		PointLongN help1;
		help1.num.clear();
		for (int i=0;i<help.size;i++)
		{
			help1.num.push_back(help.num[i]);
		}
		help1.pointpos=pointposx;
		while(help1.pointpos>=help1.num.size()) help1.num.push_back(0);
		while(help1.num[0]==0&&help1.pointpos>0)
		{
			help1.num.erase(help1.num.begin());
			help1.pointpos--;
		}
		help1.size=help1.num.size();
		help1.sign=help.sign;
		return help1;
}
void PointLongN::write_m()
{
	if(sign=='-') cout<<sign;
	for(int i=size-1;i>=0;--i)
	{
		cout<<num[i];
		if (pointpos==i) cout<<".";
	}
	cout<<endl;
}
void PointLongN::write_f(char *text)
{
	ofstream f1;
	f1.open(text);
	if(sign=='-') f1<<sign;
	for(int i=size-1;i>=0;--i)
	{
		f1<<num[i];
		if (pointpos==i) cout<<".";
	}
	f1<<endl;
	f1.close();
}
PointLongN::~PointLongN()
{
	size=0;
	num.clear();
	pointpos=0;
}
PointLongN::PointLongN()
{
	pointpos=0;
	size=0;
	sign='+';
}
PointLongN::PointLongN(LongN& a)
{
	sign=a.sign;
	size=a.size;
	for(int i=0;i<size;++i)
		num.push_back(a.num[i]);
	pointpos=0;
}
PointLongN::PointLongN(int b)
{
	LongN a(b);
	sign=a.sign;
	size=a.size;
	for(int i=0;i<size;++i)
		num.push_back(a.num[i]);
	pointpos=0;
}
PointLongN::PointLongN(PointLongN& a)
{
	sign=a.sign;
	size=a.size;
	for(int i=0;i<size;++i)
		num.push_back(a.num[i]);
	pointpos=a.pointpos;
}
PointLongN PointLongN::normalise(PointLongN a, int n)
{
	PointLongN res=PointLongN(1)/a;
	while((int)res.pointpos>=n+a.size)
	{
		res.pointpos--;
		res.num.erase(res.num.begin());
		res.size--;
	}
	return res;
}
LongN ElCur(LongN n, LongN B, LongN C)
{
	LongN a,b;
	LongN k,i,ks,sm;k=1;
	int pr[]={2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59, 61, 67, 71, 73, 79, 83, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149};	
	int q;
	for (q=0; q<16;q++)
	{
		sm=pr[q];
		if (sm>B)
		{
			break;
		}
	}
	int j;
	for (j=0; j<q; j++)
	{
		ks=1;
		sm=pr[j];
		while(sm*ks<C)
		{
			ks=ks*sm;
		}
		k=k*ks;
	}
	if (k>n*n) k=n*n;
	if(B==50&&C==50) k=n;//оскільки все-одно будемо задавати малі тести; для більшої точності і наочності цей рядок можна закоментувати
LongN result;
bool flag;
LongN x(0),y(0);
//LongN t;
//LongN t1;
	while(1)
	{
		chooseP(x, y, a, b, n);
		Point P={x,y};	
		Point Q=P;
		flag=1;
		LongN j;
		for (j=0; j<k;j=j+1)
		{
			a.sign=(a.sign=='+')?'+':'-';
			Q=groupadd(Q,P,a,n,flag,result);
			//t=(Q.x*Q.x*Q.x+Q.x*a+b)%n;
			//t1=(Q.y*Q.y)%n;
			a.sign=(a.sign=='+')?'+':'-';
			if (Q.x==0&&Q.y==0) break;
			if (!flag) {break;}	
		}
		if (!flag) break;
	}
	return result;
}
Point groupadd(Point P, Point Q, LongN p, LongN n, bool & flag, LongN & result)
{
	Point res;
	LongN s;
	LongN iny;
		iny=0;
	LongN h;
	LongN help1;
	help1=0;
	if (P.x!=Q.x)
	{
		s=(P.y-Q.y);
		if(!reverse((P.x-Q.x),n,iny))
		{flag=false; h=(P.x-Q.x); result=GSD(h,n);res.x=0; res.y=0; return res;}
		s=s*iny;
		s=s%n;
		res.x=(s*s-P.x-Q.x);
		res.x=res.x%n;
		if (res.x.sign=='-') res.x=res.x+n;
		res.y=(s*(P.x-res.x)-P.y);
		int check=1;
		if (res.y.sign=='-') check=-1;
		res.y=res.y%n;
		if (check<0) res.y.sign='-';
		if (res.y.sign=='-') res.y=res.y+n;
		return res;
	}
	else
	{
		if (P.y+Q.y==0)
		{
			res.x=0;
			res.y=0;
			return res;
		}
		else
		{
			s=(LongN(3)*P.x*P.x-p);
			LongN help;
			help=LongN(2)*P.y;
			help=help%n;
			if (reverse(help,n,iny))
			{	
				s=s*iny;
				s=s%n;
			}
			else
			{flag=false; result=GSD(n,help);res.x=0; res.y=0; return res;}
			help1=s*s;
			help1=help1%n;
			help=LongN(2)*P.x;
			help=help%n;
			res.x=help1-help;
			res.x=res.x%n;
			if (res.x.sign=='-') res.x=res.x+n;
			res.y=(s*(P.x-res.x)-P.y);
			int check=1;
			if (res.y.sign=='-') check=-1;
			res.y=res.y%n;
			if (check<0) res.y.sign='-';
			if (res.y.sign=='-') res.y=res.y+n;
			return res;
		}
	}
}
bool LongN::operator !=(LongN &data)
{
	return !(*this==data);
}
bool LongN::operator !=(int a)
{
	return !(*this==a);
}
bool reverse(LongN n,LongN mod,LongN &inv)
{
	LongN x,y;
	LongN d=gcdex(n,mod,x,y);
	if(d!=1) return false;
	inv=x;
	return true;
}
LongN gcdex(LongN a,LongN b,LongN& x,LongN& y) 
{
	if(a==0) 
	{
		x=0;
		y=1;
		return b;
	}
	LongN x1, y1;
	LongN d=gcdex(b%a,a,x1,y1);
	x=y1-(b/a)*x1;
	y=x1;
	return d;
}
void chooseP(LongN &x, LongN &y, LongN & a, LongN & b, LongN n)
{
	bool flag=true;
	x=0;
	while(flag)
	{	
		a=1;
		b=1;
		a=a+longrand(n);
		b=b+longrand(n.sqrt());
		b=b*b;
		LongN i;
		for (i=0; i<n; i=i+1)
		{	
			x=i;
			y=x*x*x+a*x+b;
			if (issquare(y))
			{y=y.sqrt();return;}
		}
		x=0;
	}
}
void menudiv()
{
	cout<<"Choose method:\n";
	cout<<"1-Reverse value\n";
	cout<<"2-Division by Cook\n";
	char meth;
	cin>>meth;
	switch(meth)
	{
	case '1':
		{
			cout<<"Enter number:\n";
			LongN num;
			num.read_k();
			PointLongN val(num),res;
			res=Inv(val);
			res.write_m();
			break;
		}
	case '2':
		{
			cout<<"Enter first number:\n";
			LongN num1,num2;
			num1.read_k();
			cout<<"Enter second number:\n";
			num2.read_k();
			PointLongN val1(num1),val2(num2),res;
			res=DivInv(val1,val2);
			res.write_m();
			break;
		}
	case 'c':
		{
			system("cls");
			break;
		}
	default:
		{
			cout<<"Wrong choise\n";
			break;
		}
	}
}
Complex& Complex::operator =(Complex &data)
{
	if(this == &data) return *this;
	Re=data.Re;
	Im=data.Im;
	return *this;
}