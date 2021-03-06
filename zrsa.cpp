// author: dongchangzhang
// time: 2017:4:17:23:01

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define MAX_NU 50000
#define PRIME_INDEX_BASE 1000
#define E_BASE 23

#define ll long long

class ZRSA {
public:
    typedef struct {
        ll e;
        ll n;
    } public_key_type;
    typedef struct {
        ll d;
        ll n;
        ll p;
        ll q;
    } private_key_type;
    ZRSA() = default;
    ll encrypt(public_key_type& puk, ll msg);
    ll decrypt(private_key_type& prk, ll c);
    void generate_key();
    public_key_type get_public_key();
    private_key_type get_private_key();

private:
    ll p, q;
    ll n, e = E_BASE, d;
    std::vector<ll> prime_array;
    public_key_type public_key;
    private_key_type private_key;
    void create_prime_number();
    ll euler_function(ll p, ll q);
    bool euclid_function(ll a, ll b);
    ll cal(ll a, ll m);
    ll e_gcd(ll a, ll b, ll& x, ll& y);
    ll fast_mod(ll a, ll b, ll m);
};
ll ZRSA::encrypt(public_key_type& puk, ll msg)
{

    return fast_mod(msg, puk.e, puk.n);
}
ll ZRSA::decrypt(private_key_type& prk, ll c)
{
    return fast_mod(c, prk.d, prk.n);
}
void ZRSA::generate_key()
{
    create_prime_number();
    ll psize = prime_array.size();
    srand((unsigned)time(0));
    do {
        p = rand() % (psize - PRIME_INDEX_BASE) - 1 + PRIME_INDEX_BASE;
        q = rand() % (psize - PRIME_INDEX_BASE) - 1 + PRIME_INDEX_BASE;
    } while (p == q);
    p = prime_array[p];
    q = prime_array[q];
    n = p * q;
    ll fx = euler_function(p, q);

    while (!euclid_function(e, fx)) {
        ++e;
    }
    d = cal(e, fx);
    // public key
    public_key.e = e;
    public_key.n = n;
    // private key
    private_key.d = d;
    private_key.n = n;
    private_key.p = p;
    private_key.q = q;
}

ZRSA::public_key_type ZRSA::get_public_key()
{
    return public_key;
}

ZRSA::private_key_type ZRSA::get_private_key()
{
    return private_key;
}

void ZRSA::create_prime_number()
{
    ll prime[MAX_NU];
    for (ll i = 0; i < MAX_NU; ++i) {
        prime[i] = i % 2 == 0 ? 0 : 1;
    }
    prime[1] = 0;
    prime[2] = 1;
    for (ll i = 3; i * i < MAX_NU; ++i) {
        for (ll j = i + i; j < MAX_NU; j += i) {
            prime[j] = 0;
        }
    }
    for (ll i = 0; i < MAX_NU; ++i) {
        if (prime[i])
            prime_array.push_back(i);
    }
}

ll ZRSA::euler_function(ll p, ll q)
{
    return (p - 1) * (q - 1);
}

bool ZRSA::euclid_function(ll a, ll b)
{
    ll r1, r2, r3 = 1;
    if (a < b) {
        r1 = a;
        r2 = b;
    } else {
        r1 = b;
        r2 = a;
    }
    while (r3 != 0) {
        r3 = r2 % r1;
        r2 = r1;
        r1 = r3;
    }
    return r2 == 1;
}

ll ZRSA::cal(ll a, ll m)
{
    ll x, y;
    ll gcd = e_gcd(a, m, x, y);
    if (1 % gcd)
        return -1;
    x *= 1 / gcd;
    m = m > 0 ? m : -m;
    ll ans = x % m;
    if (ans <= 0)
        ans += m;
    return ans;
}

ll ZRSA::e_gcd(ll a, ll b, ll& x, ll& y)
{
    if (!b) {
        x = 1;
        y = 0;
        return a;
    }
    ll ans = e_gcd(b, a % b, x, y);
    ll temp = x;
    x = y;
    y = temp - a / b * y;
    return ans;
}

ll ZRSA::fast_mod(ll a, ll b, ll m)
{
    ll ret = 1;
    ll tmp = a;
    while (b) {
        if (b & 0x1)
            ret = ret * tmp % m;
        tmp = tmp * tmp % m;
        b >>= 1;
    }
    return ret;
}

void print_information(ZRSA::public_key_type& pu, ZRSA::private_key_type& pr)
{

    std::cout << "--------------- Infomation ----------------------" << std::endl;
    std::cout << "p: " << pr.p << std::endl;
    std::cout << "q: " << pr.q << std::endl;
    std::cout << "(p-1)(q-1): " << (pr.p - 1) * (pr.q - 1) << std::endl;
    std::cout << std::endl;
    std::cout << "public key:" << std::endl;
    std::cout << "e: " << pu.e << std::endl;
    std::cout << "n: " << pu.n << std::endl;
    std::cout << "private key: " << std::endl;
    std::cout << "d: " << pr.d << std::endl;
    std::cout << "n: " << pr.n << std::endl;
}

void encrypt_and_decrypt(ZRSA& zrsa, ZRSA::public_key_type& pu, ZRSA::private_key_type& pr)
{
    std::cout << "----------------- Encrypt -----------------------" << std::endl;
    std::string input;
    std::cout << "Input Information to Encrypt, End by Enter" << std::endl
              << std::endl;
    std::getline(std::cin, input);
    std::cout << std::endl
              << "Use Public Key to Encrypt Information: " << input << std::endl
              << std::endl;
    std::vector<ll> codes;
    ll code;
    std::cout << "Ciphertext Is:" << std::endl;
    for (auto ch : input) {
        code = zrsa.encrypt(pu, (ll)ch);
        codes.push_back(code);
        std::cout << code << " ";
    }
    std::cout << std::endl
              << std::endl;
    std::cout << "----------------- decrypt -----------------------" << std::endl;
    std::cout << "Use Private Key to Decrypt: " << std::endl
              << std::endl;
    std::vector<ll> dcodes;
    std::cout << "Code After Decrypt: " << std::endl;
    for (auto c : codes) {
        code = zrsa.decrypt(pr, c);
        dcodes.push_back(code);
        std::cout << code << " ";
    }
    std::cout << std::endl
              << std::endl;
    std::cout << "Source Information is: " << std::endl;
    std::string output;
    for (auto dc : dcodes) {
        output += (char)dc;
    }
    std::cout << output << std::endl << std::endl;

    if (input == output)
        std::cout << "Right, Test Passed!" << std::endl;
}

bool confirm()
{
    bool ret = false;
    std::string input;
    std::getline(std::cin, input);
    size_t index = 0;
    while (index < input.size()) {
        if (input[index] == ' ') {
            ++index;
            continue;
        } else if (input[index] == 'Y' || input[index] == 'y') {
            ret = true;
            break;
        } else {
            break;
        }
    }
    return ret;
}

int main()
{
    ZRSA zrsa;
    zrsa.generate_key();

    ZRSA::public_key_type pu = zrsa.get_public_key();
    ZRSA::private_key_type pr = zrsa.get_private_key();

    print_information(pu, pr);

    while (true) {
        std::string input;
        encrypt_and_decrypt(zrsa, pu, pr);

        std::cout << "Continue? (N/y): ";
        if (!confirm())
            break;
        std::cout << "Create A New Key? (N/y): ";
        if (confirm()) {
            zrsa.generate_key();
            pu = zrsa.get_public_key();
            pr = zrsa.get_private_key();
            print_information(pu, pr);
        }
    }
    std::cout << "Bye" << std::endl;
    return 0;
}