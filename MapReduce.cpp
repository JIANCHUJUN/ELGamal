//
// Created by ME on 2021/6/2.
//
#include "MapReduce.h"
MapReduce::MapReduce(uint32_t num_threads_, pipeline& pipeline_,func_ mapFunc_, func_ reduceFunc_) {
    this->num_threads = num_threads_;
    this->pipeline_ = pipeline_;
    this->map_func = mapFunc_;
    this->reduce_func = reduceFunc_;
}

void MapReduce::run() {
    auto * threads1 = new vector<thread>();
    for(int i = 0; i < this->num_threads; i++){
        threads1->push_back(thread(map_func,this->pipeline_.start,this->pipeline_.middle,to_string(i)));
    }
    for(int i = 0; i < this->num_threads; i++){
        threads1->at(i).join();
    }
    std::cout << "reduce start" << std::endl;
    thread reduce(reduce_func,this->pipeline_.middle, this->pipeline_.end,to_string(num_threads));
    reduce.join();
}

string get_value(const char* filename, const char *key_){
    fstream file(filename, ios::in|ios::out);
    char buffer[1000];
    char key[1000];
    char value[1000];
    while(!file.eof()){
        file.getline(buffer, 1000*sizeof(char));
        sscanf(buffer, "%s %s", key, value);
        if(strcmp(key,key_) == 0){
            return string(value);
        }
    }
    return nullptr;
}