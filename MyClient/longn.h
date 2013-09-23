#include <vector>
//#include <complex>
using namespace std;
struct Point;
class PointLongN;
class LongN
{
private:
	vector <int> num;
	int size;//�����
	char sign;//����
public:
	LongN();
	LongN(int);
	LongN(LongN&);
	~LongN();
	LongN add(LongN&);//��������� �� �������
	LongN substr(LongN&);//�������� �� �������
	LongN mult10(int);//�������� �� 10 � ������(����)
	LongN div10(int);//������ �� 10 � ������(����)
	void read_k();//���������� � ���������
	void write_m();//���� �� �����
	void read_f(char*);//���������� � �����
	void write_f(char*);//����� � ����
	void conv10_to2(LongN);//����������� � ������� �������
	void dividecar(LongN&,LongN&,LongN&,LongN&,LongN&,int&);//���� ����� �� ������� ��� �������� ���������
	void dividetom(LongN&,LongN&,LongN&,LongN&,LongN&,LongN&,LongN&,int&);//���� ����� �� ������� ��� �������� ����� ����
	LongN usmult(LongN&);//�������� ��������
	LongN caratsubi(LongN&);//�������� ���������
	LongN toomcook(LongN&);//�������� ����� ����
	LongN shenkhage(LongN&);//�������� �������
	LongN shtrassen(LongN&);//�������� ���������
	LongN div(int,int&);//������ �� ������� �����
	LongN div(int);//������ �� ������� �����
	int mod(int);//������ �� ������ �� ������� �����
	LongN div(LongN&);//������ �� ����� �����
	LongN mod(LongN&);//������ �� ������ �� ����� �����
	LongN sqrt();//��������� �����
	void convtolong(int);//������������� � ��� LongN
	int convtoint();//����������� � ��� int
	bool operator ==(int);
	bool operator ==(LongN&);
	bool operator !=(int);
	bool operator !=(LongN&);
	bool operator >(LongN&);
	bool operator <(LongN&);
	bool operator <=(LongN&);
	bool operator >=(LongN&);
	LongN& operator =(LongN&);
	bool operator >(int);
	bool operator <(int);
	bool operator <=(int);
	bool operator >=(int);
	LongN& operator =(int);
	LongN operator *(LongN&);
	LongN operator +(LongN&);
	LongN operator -(LongN&);
	LongN operator /(LongN&);
	LongN operator %(LongN&);
	LongN operator *(int);
	LongN operator +(int);
	LongN operator -(int);
	LongN operator /(int);
	LongN operator %(int);
	friend LongN GSD(LongN&,LongN&);//��� ���� �����
	friend LongN dicson(LongN&);//������������ ĳ�����
	friend LongN longrand(LongN &);//��������������� ����� � �������� �� �����
	friend LongN pollard(LongN&);//������������ ��������
	friend LongN quadraticsieve(LongN&);//������������ ������������ �������
	friend PointLongN Inv(PointLongN,int n=15);//�������� ���� ����������� ���������� �� �����
	friend PointLongN DivInv(PointLongN,PointLongN,int n=15);//�������� ������ ����
	friend LongN ElCur(LongN, LongN, LongN);//������� ����
	friend Point groupadd(Point,Point,LongN,LongN,bool&,LongN&);//��������� ���� ����� �� ��������� ������
	friend class PointLongN;
};
class Complex
{
	double Re;
	double Im;
public:
	Complex();
	Complex(double x);
	Complex(double x,double y);
	Complex(Complex&);
	~Complex();
	Complex operator +(Complex&);
	Complex operator -(Complex&);
	Complex operator *(Complex&);
	Complex operator /(int);
	Complex operator *=(Complex&);
	Complex operator /=(int);
	Complex& operator =(Complex&);
	double real();
};

class PointLongN:public LongN
{
private:
	unsigned int pointpos;//������� ����
public:
	PointLongN();
	PointLongN(int);
	PointLongN(LongN&);
	PointLongN(PointLongN&);
	~PointLongN();
	PointLongN operator-(PointLongN &);
	PointLongN operator+(PointLongN &);
	PointLongN operator*(PointLongN &);
	PointLongN operator/(PointLongN &);
	PointLongN normalise(PointLongN, int n=15);//��������� ������ �� ��� �������� �� �������
	void write_m();//���� �� �����
	void write_f(char*);//����� � ����
	friend PointLongN Inv(PointLongN,int n);//�������� ���� ����������� ���������� �� �����
	friend PointLongN DivInv(PointLongN,PointLongN,int n);//�������� ������ ����
};
struct Point
{
	LongN x;
	LongN y;
};
//typedef complex<double> mytype;
typedef Complex mytype;
typedef vector<int> decay;//������� �� �������� ���
extern int typemult;//��� ��������
extern int base;//������ ��������
extern vector<int> factbase;//�������� ����
extern LongN int_max;//�������� ����
vector<int> erat(int);//�������� "������ ����������"
LongN func(LongN&,LongN&,int);//��� ������� ��� ��������� ��������
bool issquare(LongN n);//�������� �� �������
int pw(int,int);//������
int dsqrt(int,int&);//���������� �����
void fft(mytype*,bool,int);//������������ ���'�
int legandr(LongN,int);//������ ��������
void swap(int&,int&);
void swap(Complex&,Complex&);
void testmult(int,int);//���������
void menumult();//���������
void menufact();//���������
void menu();//���������
void modul(vector<int>&);//������ ������
void returnmodul(vector<int>&);//���������� ����� �� ������
int reverse(int,int);//�������� �����
int gcdex(int,int,int&,int&);//���������� �������� ������
bool reverse(LongN,LongN,LongN&);//�������� �����
LongN gcdex(LongN,LongN,LongN&,LongN&);//���������� �������� ������
void chooseP(LongN &x, LongN &y, LongN & a, LongN & b, LongN n);//����� ����� � ������ ������������ �� �������� �����
void menudiv();//���������