#ifndef _SERVICE_B_CLASS_H_INCLUDED
#define _SERVICE_B_CLASS_H_INCLUDED

#include "ServiceA.h"

class ServiceB { 
public:
    int id;
    ServiceB();
    ~ServiceB() {};
    ServiceA* a;

    std::string say();

    RTTR_ENABLE()
};

#endif //_SERVICE_B_CLASS_H_INCLUDED