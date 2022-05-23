#include "ServiceA.h"
#include "ServiceB.h"

std::mt19937_64 rng1(std::chrono::steady_clock::now().time_since_epoch().count());

ServiceA::ServiceA() {
    id = rng1()%10;
    //std::cout<<"constructor of ServiceA "<<id<<'\n';
}

std::string ServiceA::say() {
    std::string ret = "I am A, and my son say: ";
    ret += "da";//b->say();
    return ret;
}