#include <vector>
//#include <complex>
using namespace std;
struct Point;
class PointLongN;
class LongN
{
private:
	vector <int> num;
	int size;//розмір
	char sign;//знак
public:
	LongN();
	LongN(int);
	LongN(LongN&);
	~LongN();
	LongN add(LongN&);//додавання за модулем
	LongN substr(LongN&);//віднімання за модулем
	LongN mult10(int);//множення на 10 в степені(зсув)
	LongN div10(int);//ділення на 10 в степені(зсув)
	void read_k();//зчитування з клавіатури
	void write_m();//вивід на екран
	void read_f(char*);//зчитування з файлу
	void write_f(char*);//запис в файл
	void conv10_to2(LongN);//конвертація в двійкову систему
	void dividecar(LongN&,LongN&,LongN&,LongN&,LongN&,int&);//поділ числа на частини для множення Карацубою
	void dividetom(LongN&,LongN&,LongN&,LongN&,LongN&,LongN&,LongN&,int&);//поділ числа на частини для множення Тоома Кука
	LongN usmult(LongN&);//звичайне множення
	LongN caratsubi(LongN&);//множення Карацубою
	LongN toomcook(LongN&);//множення Тоома Кука
	LongN shenkhage(LongN&);//множення Шенхаге
	LongN shtrassen(LongN&);//множення Штрассена
	LongN div(int,int&);//ділення на коротке число
	LongN div(int);//ділення на коротке число
	int mod(int);//остача від ділення на коротке число
	LongN div(LongN&);//ділення на довге число
	LongN mod(LongN&);//остача від ділення на довге число
	LongN sqrt();//квадратній корінь
	void convtolong(int);//конвертування в тип LongN
	int convtoint();//конвертація в тип int
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
	friend LongN GSD(LongN&,LongN&);//НСД двох чисел
	friend LongN dicson(LongN&);//факторизація Діксона
	friend LongN longrand(LongN &);//псевдовипадкове число в інтервалі до числа
	friend LongN pollard(LongN&);//факторизація Полларда
	friend LongN quadraticsieve(LongN&);//факторизація квадратичним решетом
	friend PointLongN Inv(PointLongN,int n=15);//алгоритм Кука знаходження оберненого до числа
	friend PointLongN DivInv(PointLongN,PointLongN,int n=15);//алгоритм ділення Кука
	friend LongN ElCur(LongN, LongN, LongN);//еліптичні криві
	friend Point groupadd(Point,Point,LongN,LongN,bool&,LongN&);//Додавання двох точок по груповому закону
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
	unsigned int pointpos;//позиція коми
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
	PointLongN normalise(PointLongN, int n=15);//видалення зайвих біт при ітерації по Ньютону
	void write_m();//вивід на екран
	void write_f(char*);//запис в файл
	friend PointLongN Inv(PointLongN,int n);//алгоритм Кука знаходження оберненого до числа
	friend PointLongN DivInv(PointLongN,PointLongN,int n);//алгоритм ділення Кука
};
struct Point
{
	LongN x;
	LongN y;
};
//typedef complex<double> mytype;
typedef Complex mytype;
typedef vector<int> decay;//розклад по факторній базі
extern int typemult;//тип множення
extern int base;//основа числення
extern vector<int> factbase;//факторна база
extern LongN int_max;//максимум інтів
vector<int> erat(int);//алгоритм "Решета Ератосфена"
LongN func(LongN&,LongN&,int);//доп функція для алгоритма Полларда
bool issquare(LongN n);//перевірка на квадрат
int pw(int,int);//степінь
int dsqrt(int,int&);//дискретний корінь
void fft(mytype*,bool,int);//перетворення Фур'є
int legandr(LongN,int);//символ Лежандра
void swap(int&,int&);
void swap(Complex&,Complex&);
void testmult(int,int);//інтерфейс
void menumult();//інтерфейс
void menufact();//інтерфейс
void menu();//інтерфейс
void modul(vector<int>&);//взяття модуля
void returnmodul(vector<int>&);//відновлення числа по лишкам
int reverse(int,int);//обернене число
int gcdex(int,int,int&,int&);//розширений алгоритм Евкліда
bool reverse(LongN,LongN,LongN&);//обернене число
LongN gcdex(LongN,LongN,LongN&,LongN&);//розширений алгоритм евкліда
void chooseP(LongN &x, LongN &y, LongN & a, LongN & b, LongN n);//обирає точку з цілими координатами на еліптичній кривій
void menudiv();//інтерфейс