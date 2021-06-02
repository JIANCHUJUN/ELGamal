#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>

using namespace std;

uint64_t big_prime(uint16_t power, char* filename = nullptr);
uint64_t modulo(uint64_t a, uint64_t b, uint64_t m);
bool is_origin(uint64_t a, uint64_t m);
vector<uint64_t>* encrypt(uint64_t M, uint64_t p, uint64_t alpha, uint64_t y);
uint64_t decrypt(vector<uint64_t>* C, uint64_t d, uint64_t p);
uint64_t reverse(uint64_t V, uint64_t prime);


int main(int argc, char* argv[]) {
    assert(argc == 3);
    uint64_t prime = atoi(argv[1]);
    uint64_t M = atoi(argv[2]);
    uint64_t alpha;
    uint64_t d;
    uint64_t y;

    for(uint64_t i = 2; i < prime; i++){
        if(is_origin(i,prime)){
            alpha = i;
            break;
        }
    }

    std::srand(std::time(nullptr));
    do{
        d = std::rand() % prime;
    }while(d == 1);
    y = modulo(alpha, d, prime);

    auto* C = encrypt(M,prime,alpha,y);
    uint64_t M_ = decrypt(C, d, prime);
    assert(M_ == M);
    return 0;
}

/*
 * 模逆运算
 */
uint64_t reverse(uint64_t V, uint64_t prime){
    V = V%prime;
    for(uint64_t i = 2; i < prime; i++){
        if((i*V)%prime == 1){
            return i;
        }
    }
    return 0;
}

/*
 * 加密
 */
vector<uint64_t>* encrypt(uint64_t M, uint64_t p, uint64_t alpha, uint64_t y){
    auto* C = new vector<uint64_t>;
    uint64_t k,U;
    do{
        do{
            std::srand(std::time(nullptr));
            k = std::rand() % (p - 1);
        } while (k == 1);
        U = modulo(y,k,p);
    } while (U == 1);
    C->push_back(modulo(alpha,k,p));
    C->push_back((U*M)%p);
    return C;
}

/*
 * 解密
 */
uint64_t decrypt(vector<uint64_t>* C, uint64_t d, uint64_t p){
    uint64_t V = modulo(C->at(0),d,p);
    uint64_t rV = reverse(V,p);

    assert((rV*V)%p == 1);
    uint64_t M = 0;
    if(rV){
        M = (C->at(1)*rV)%p;
    }
    return M;
}

/*
 * 模幂乘
 */
uint64_t modulo(uint64_t a, uint64_t b, uint64_t m){
    uint64_t x=1, y=a;
    while (b > 0) {
        if (b%2 == 1) {
            x = (x*y) % m; // multiplying with base
        }
        y = (y*y) % m; // squaring the base
        b /= 2;
    }
    return x % m;
}

/*
 * 判断原根
 */
bool is_origin(uint64_t a, uint64_t m){
    if(std::gcd(a,m) != 1){
        return false;
    }
    for(uint64_t i = 2; i < m; i++){
        if(m%i == 0){
            if(modulo(a,i,m) == 1){
                return false;
            }
            while(m % i == 0){
                m = m/i;
            }
        }
    }
    return true;
}

/*
 * 打印指数表
 */
uint64_t big_prime(char* filename){
    uint64_t prime = PRIME_UPPER_BOUND;
    auto* table = new vector<pair<uint64_t,bool>*>;
    table->push_back(new pair<uint64_t,bool>(0, true));
    table->push_back(new pair<uint64_t,bool>(1, true));
    for(uint64_t i = 2; i < prime; i++){
        table->push_back(new pair<uint64_t,bool>(i, false));
    }
    for(uint64_t i = 0; i < prime; i++){
        if(table->at(i)->second){
            continue;
        }else{
            for(uint64_t j = i + 1; j < prime; j++){
                if(table->at(j)->second){
                    continue;
                }else if(table->at(j)->first % i == 0){
                    table->at(j)->second = true;
                }else {
                    continue;
                }
            }
        }
    }

    uint64_t result;
    fstream file;
    if(filename){
        file.open(filename,ios::in | ios::out | ios::binary | ios::trunc);
    }
    for(uint64_t i = 2; i < prime; i++){
        if(!table->at(i)->second){
            result = table->at(i)->first;
            if(filename){
                file << to_string(result) << endl;
            }
        }
    }
    if(filename) {
        file.close();
    }
    delete table;
    return result;
}