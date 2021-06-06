#include <map>
#include "ELGamal.h"
#include "thread"

const uint32_t prime = 10037;

void Encrypt(uint32_t M, uint32_t idx, crptxt* cryps, uint32_t* ds){
    uint64_t alpha;
    uint64_t d;
    uint64_t y;

    std::srand(std::time(nullptr));

    do{
        alpha = std::rand() % prime;
    }while(!is_origin(alpha,prime));
    std::srand(std::time(nullptr));
    do{
        d = std::rand() % prime;
    }while(d == 1);
    y = modulo(alpha, d, prime);

    vector<uint64_t>* C = encrypt(M,prime,alpha,y);
    cryps[idx].c1 = C->at(0);
    cryps[idx].c2 = C->at(1);
    ds[idx] = d;
    delete C;
}

int main(int argc, char* argv[]){
    char buffer[1000];
    fstream alphabet("Data/alphabet.txt", ios::in);
//    uint32_t* alpha = new uint32_t[26];
    auto* alpha = new map<char,uint32_t>;
    auto* beta = new map<uint32_t,char>;
    while(!alphabet.eof()){
        alphabet.getline(buffer, 1000*sizeof(char));
        char c;
        uint32_t u;
        sscanf(buffer,"%c %u",&c, &u);
        alpha->insert(pair<char,uint32_t>(c,u));
        beta->insert(pair<uint32_t,char>(u,c));
    }
    alpha->insert(pair<char,uint32_t>(' ',1234));
    beta->insert(pair<uint32_t,char>(1234,' '));

    alpha->insert(pair<char,uint32_t>('\n',1235));
    beta->insert(pair<uint32_t,char>(1235,'\n'));

    alphabet.close();

    fstream raw_file("Data/raw_file.txt", ios::in);
    auto * chars = new vector<uint32_t>();
    while(!raw_file.eof()){
        char c;
        raw_file.read(&c, sizeof(char));
        c = tolower(c);
        if(alpha->contains(c)){
            chars->push_back(alpha->at(c));
        }
    }
    raw_file.close();

    auto* threads = new vector<thread>();
    crptxt* crptxts = new crptxt[chars->size()];
    uint32_t* ds = new uint32_t [chars->size()];
    for(uint32_t i = 0; i < chars->size(); i++){
        threads->push_back(thread(Encrypt,chars->at(i),i,crptxts,ds));
    }

    fstream result("Tmp/result.txt",ios::out | ios::trunc);
    fstream crp("Tmp/crptxts.txt",ios::out | ios::trunc);
    fstream dd("Tmp/ds.txt",ios::out | ios::trunc);
    for(uint32_t i = 0; i < chars->size(); i++){
        threads->at(i).join();
        crp << crptxts[i].c1 << ',' << crptxts[i].c2 << endl;
        dd << ds[i] << endl;
        uint32_t M = decrypt(crptxts[i],ds[i],prime);
        char c = beta->at(M);
        result << c;
    }
    result.close();
    crp.close();
    dd.close();
    delete threads;
    delete crptxts;
    delete ds;
    delete alpha;
    delete beta;
}

