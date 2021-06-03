//
// Created by ME on 2021/6/2.
//

#ifndef MAPREDUCE_MAPREDUCE_H
#define MAPREDUCE_MAPREDUCE_H
#include "thread"
#include "vector"
#include <cstdint>
#include <string>
#include "iostream"
#include "fstream"

using namespace std;

using func_ = int (*)(string, string, string);

struct pipeline{
    string start, middle, end;
};

string get_value(const char* filename, const char *key_);

class MapReduce {
public:
    uint32_t num_threads;
    pipeline pipeline_;
    func_ map_func;
    func_ reduce_func;
public:
    MapReduce(uint32_t num_threads_, pipeline& pipeline_,func_ mapFunc_, func_ reduceFunc_);
    void run();
};


#endif //MAPREDUCE_MAPREDUCE_H
