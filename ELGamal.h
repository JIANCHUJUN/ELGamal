//
// Created by Jiacheng on 2021/6/3.
//

#ifndef ELGAMAL_ELGAMAL_H
#define ELGAMAL_ELGAMAL_H

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>

using namespace std;

uint64_t big_prime(uint16_t upper, char* filename = nullptr);
uint64_t modulo(uint64_t a, uint64_t b, uint64_t m);
bool is_origin(uint64_t a, uint64_t m);
vector<uint64_t>* encrypt(uint64_t M, uint64_t p, uint64_t alpha, uint64_t y);
uint64_t decrypt(vector<uint64_t>* C, uint64_t d, uint64_t p);
uint64_t reverse(uint64_t V, uint64_t prime);



#endif //ELGAMAL_ELGAMAL_H
