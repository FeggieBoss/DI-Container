#ifndef _SERVICE_B_CLASS_H_INCLUDED
#define _SERVICE_B_CLASS_H_INCLUDED

#include "ServiceA.h"

class ServiceB : public root_class { 
public:
    ServiceB() : root_class() {};
    ~ServiceB() {};
    ServiceA* a;

    std::string say();

    RTTR_ENABLE(root_class)
};

#endif //_SERVICE_B_CLASS_H_INCLUDED