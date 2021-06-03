#include "MapReduce.h"
#include "ELGamal.h"

uint32_t prime_;


int mapFunc(string start, string middle, string key){
    uint32_t prime = prime_;
    string value = get_value(start.c_str(),key.c_str());
    fstream file(value, ios::in | ios::out);
    char buffer[1000];
    uint64_t M;
    if(!file.eof()){
        file.getline(buffer,1000*sizeof(char));
        M = atoi(buffer);

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
//        cout << "明文: " << M << endl;
//        cout << "密文: " << C->at(0) << " " << C->at(1) << endl;
//        cout << "公钥: " << y << endl;
//        cout << "私钥: " << d << endl;
//        cout << "解密后明文: " << M_ << "\n" << endl;

        string output_name(middle + "_" + key);
        fstream output(output_name, ios::in | ios::out | ios::trunc);
        output << "明文: " << M << endl;
        output << "密文: " << C->at(0) << " " << C->at(1) << endl;
        output << "公钥: " << y << endl;
        output << "私钥: " << d << endl;
        output << "解密后明文: " << M_ << "\n" << endl;
        output.close();

        return 0;
    }
    file.close();
    return 0;
}

int reduceFunc(string middle, string end, string count){
    uint32_t count_ = atoi(count.c_str());
    char buffer[1000];

    string output_name(end);
    fstream output(output_name, ios::in | ios::out | ios::trunc);

    for(uint32_t key = 0; key < count_; key++){
        string value = string(middle + "_" + to_string(key));
        fstream file(value, ios::in | ios::out);
        while(!file.eof()){
            file.getline(buffer, 1000*sizeof(char));
            output << buffer << endl;
        }
        file.close();
    }
    output.close();
    return 0;
}


int main(int argc, char* argv[]) {
    assert(argc == 3);
    prime_ = atoi(argv[1]);
    string start = string(argv[2]);
    pipeline pipeline_{start,"middle","end"};

    MapReduce* mapReduce = new MapReduce(4,pipeline_,mapFunc,reduceFunc);
    mapReduce->run();

}

