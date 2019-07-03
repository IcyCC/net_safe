#include <vector>
#include <iostream>


std::vector<std::string> split_string(std::string src, std::string sp) {
    std::string::size_type pos1, pos2 = 0;
    std::vector<std::string> v;
    pos2 = src.find(sp);
    pos1 = 0;
    while (std::string::npos != pos2) {
        v.push_back(src.substr(pos1, pos2 - pos1));

        pos1 = pos2 + sp.size();
        pos2 = src.find(sp, pos1);
    }
    if (pos1 != src.length()) {
        v.push_back(src.substr(pos1));
    }
    return v;
}


int bin_transform(int num, int bin_num[]) {

    int i = 0, mod = 0;

    while (num != 0) {
        mod = num % 2;
        bin_num[i] = mod;
        num = num / 2;
        i++;
    }

    return i;
}


long long mod(long long input, int public_e, int public_n) {
    int bin_num[1000];
    long long data = 1;
    int k = bin_transform(public_e, bin_num) - 1;

    for (int i = k; i >= 0; i--) {
        data = (data * data) % public_n;
        if (bin_num[i] == 1) {
            data = (data * input) % public_n;
        }
    }
    return data;
}

int produce_prime_number(int prime[]) {
    int c = 0, vis[1001];
    memset(vis, 0, sizeof(vis));
    for (int i = 2; i <= 1000; i++)
        if (!vis[i]) {
            prime[c++] = i;
            for (int j = i * i; j <= 1000; j += i)
                vis[j] = 1;
        }

    return c;
}


int get_gcd(int m, int n, int &x) {
    int x1, y1, x0, y0, y;
    x0 = 1;
    y0 = 0;
    x1 = 0;
    y1 = 1;
    x = 0;
    y = 1;
    int r = m % n;
    int q = (m - r) / n;
    while (r) {
        x = x0 - q * x1;
        y = y0 - q * y1;
        x0 = x1;
        y0 = y1;
        x1 = x;
        y1 = y;
        m = n;
        n = r;
        r = m % n;
        q = (m - r) / n;
    }
    return n;
}

void get_public_key(int &key_n,  int &public_e, int &public_d)
{
    int prime[5000];
    int count_Prime = produce_prime_number(prime);

    srand((unsigned)time(NULL));
    int ranNum1 = rand()%count_Prime;
    int ranNum2 = rand()%count_Prime;
    int p = prime[ranNum1], q = prime[ranNum2];

    key_n = p*q;

    int on = (p-1)*(q-1);


    for(int j = 3; j < on; j+=1331)
    {
        int gcd = get_gcd(j, on, public_d);
        if( gcd == 1 && public_d > 0)
        {
            public_e = j;
            break;
        }

    }

}
std::string rsa_encrypt(std::string &input, int public_e, int public_n) {
    std::string result;
    std::vector<long long> middle;
    for (auto &input_part:input) {
        long long data = mod(input_part, public_e, public_n);
        middle.push_back(data);
    }

    for (auto middle_part:middle) {
        result = result + std::to_string(middle_part) + " ";
    }
    return result;
}

std::string rsa_decrypt(std::string &input, int private_e, int private_n) {
    std::vector<std::string> input_parts = split_string(input, " ");
    std::string result;
    for(auto input_part:input_parts) {
        long long data  = std::atoll(input_part.c_str());
        data = mod(data, private_e, private_n);
        result+= data;
    }
    return result;
}
