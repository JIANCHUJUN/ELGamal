cmake_minimum_required(VERSION 3.17)
project(ELGamal)

set(CMAKE_CXX_STANDARD 20)

add_executable(ELGamal main.cpp ELGamal.cpp ELGamal.h MapReduce.cpp MapReduce.h)