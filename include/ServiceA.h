#ifndef _SERVICE_A_CLASS_H_INCLUDED
#define _SERVICE_A_CLASS_H_INCLUDED

#include <string>
#include <iostream>
#include "root_class.h"

class ServiceB;
class ServiceA : public root_class { 
public:
    ServiceA() : root_class() {};
    ~ServiceA() {};
    ServiceB* b;

    std::string say();

    RTTR_ENABLE(root_class)
};

#endif //_SERVICE_A_CLASS_H_INCLUDED