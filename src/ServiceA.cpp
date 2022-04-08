#include "ServiceA.h"
#include "ServiceB.h"

std::string ServiceA::say() {
    std::string ret = "I am A, and my son say: ";
    ret += "da";//b->say();
    return ret;
}