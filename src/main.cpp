#include <cassert>
#include <string>
#include <iostream>

#include "di_container.h"


#include "ServiceA.h"
#include "ServiceB.h"


#include <rttr/registration>
using namespace rttr;

struct MyStruct { MyStruct() {}; void func(double) {}; int data; };

class Fita : public root_class {
public:
    virtual std::string name() const { return "Fita"; };
    RTTR_ENABLE(root_class)
};

class Bar : public Fita {
public:
    std::string name() const override { return "Bar"; };
    RTTR_ENABLE(Fita)
};

RTTR_REGISTRATION
{
    registration::class_<MyStruct>("MyStruct")
        .constructor<>()
        .property("data", &MyStruct::data)
        .method("func", &MyStruct::func);

    
    /*registration::class_<ServiceA>("ServiceA")
        .constructor<>()
        .property("b", &ServiceA::b)
        .method("say", &ServiceA::say);

    registration::class_<ServiceB>("ServiceB")
        .constructor<>()
        .property("a", &ServiceB::a)
        .method("say", &ServiceB::say);*/
    registration::class_<Fita>("root_class")
        .constructor<>();

    registration::class_<Fita>("Fita")
        .constructor<>()
        .method("name", &Fita::name);

    registration::class_<Bar>("Bar")
        .constructor<>()
        .method("name", &Bar::name);
}

int main() {
    type t = type::get<MyStruct>();
    for (auto& prop : t.get_properties())
        std::cout << "name: " << prop.get_name() << '\n';

    for (auto& meth : t.get_methods())
        std::cout << "name: " << meth.get_name() << '\n';

    /*di_container di;
    di.add_instance<ServiceA>();
    di.add_instance<ServiceB>();
    
    auto supa = di.get_prob_instances<ServiceB>();
    std::cout<<supa.size()<<'\n';
    std::cout<<supa.back()->say();
    */

    di_container_::di_container di;

    di.add_instance<Bar>();
    di.add_instance<root_class>();
    di.add_instance<Fita>();

    auto Fita_instances = di.get_prob_instances<Fita>();
    std::cout<<Fita_instances.size();

    return 0;
}