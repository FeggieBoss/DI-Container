#include "ServiceA.h"
#include "ServiceB.h"

std::mt19937_64 rng2(std::chrono::steady_clock::now().time_since_epoch().count());

ServiceB::ServiceB() : root_class() {
    id = rng2()%10;
    //std::cout<<"constructor of ServiceB "<<id<<'\n';
}

std::string ServiceB::say() {
    std::string ret = "I am B, and my son say: ";
    ret += a->say();
    return ret;
}