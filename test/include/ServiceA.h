#ifndef _SERVICE_A_CLASS_H_INCLUDED
#define _SERVICE_A_CLASS_H_INCLUDED

#include <string>
#include <iostream>
#include <chrono>
#include <random>
#include "root_class.h"

class ServiceB;
class ServiceA { 
public:
    int id;
    ServiceA();
    ~ServiceA() {};
    ServiceB* b;

    std::string say();

    RTTR_ENABLE()
};

#endif //_SERVICE_A_CLASS_H_INCLUDED