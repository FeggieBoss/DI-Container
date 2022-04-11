#include "ServiceA.h"
#include "ServiceB.h"

std::string ServiceB::say() {
    std::string ret = "I am B, and my son say: ";
    ret += a->say();
    return ret;
}