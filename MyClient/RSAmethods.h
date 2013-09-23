#include <string>
using namespace std;
long long gcdex (long long, long long, long long&, long long&);
long long ModExp(long long, long long, long long);
long long inverce(long long, long long);
long long gcd(long long, long long);
bool issquare(long long);
bool ferma(long long ,long long&, long long&);
bool is_simple_by_ferma(long long n);
bool solovey (long long);
bool is_simple_by_solovey(long long, long long);
long long Jakobi (long long, long long);
void gen_key_rand(long long &, long long &, long long &);
long long RSA_encode(long long, long long, long long);
long long RSA_decode(long long, long long, long long);
void gen_key_manual(long long &, long long &, long long &, long long &, long long &);
string encode_string(string, long long, long long);
string decode_string(string, long long, long long);