#include "di_container.h"

#include <rttr/registration>
using namespace rttr;

#include <cassert>
#include <string>
#include <iostream>
#include <map>

class BarI {
public:
    virtual std::string name() const = 0;
    RTTR_ENABLE()
};

class Bar : public BarI{
public:
    int id;
    Bar() : id(111) {};
    Bar(int id_) : id(id_) {};
    std::string name() const override { return "Bar"+std::to_string(id); };
    RTTR_ENABLE(BarI)
};

class Fita {
public:
    BarI* bar;
    std::string name() const { 
        std::string barik = bar->name();
        barik+= " i Fitusya";
        return barik;
    };
    RTTR_ENABLE()
};

RTTR_REGISTRATION
{
    registration::class_<Fita>("Fita")
        .constructor<>()
        .method("name", &Fita::name)
        .property("bar", &Fita::bar);

    registration::class_<BarI>("BarI")
        .method("name", &BarI::name);

    registration::class_<Bar>("Bar")
        .constructor<>()
        .constructor<int>()
        .method("name", &Bar::name)
        .property("id", &Bar::id);
}

int main() {
    di_container_::di_container di;
    //di.add_singleton<Bar>();
    //di.add_singleton<Fita>();

    //di.field_file_registration("input.txt");
    di.register_instance<BarI,Bar>({1}, di_container_::lifetime_type::Singleton);
    di.register_instance<Bar,Bar>({2}, di_container_::lifetime_type::Singleton);
    di.register_instance<Fita,Fita>({}, di_container_::lifetime_type::Scoped);

    std::cout<<di.get_instance<Bar>()->name()<<"\n\n";
    //std::cout<<di.get_instance<Bar>()->id<<"\n\n";

    di.get_instance<Bar>()->id = 142412;

    std::cout<<di.get_instance<BarI>()->name()<<"\n\n";
    //std::cout<<di.get_instance<Bar>()->id<<"\n\n";
    std::cout<<"==="<<di.get_instance<Fita>()->name()<<"\n\n";
    //std::cout<<"==="<<di.get_instance<Fita>()->bar->id<<"\n\n";

    return 0;
}