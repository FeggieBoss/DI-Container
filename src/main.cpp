#include "di_container.h"

#include <rttr/registration>
#include <rttr/registration_friend>
using namespace rttr;

#include <cassert>
#include <string>
#include <iostream>
#include <map>

// class BarI {
// public:
//     virtual std::string name() const = 0;
//     RTTR_ENABLE()
// };

// class Bar : public BarI{
// public:
//     int id;
//     Bar() : id(111) {};
//     Bar(int id_) : id(id_) {};
//     std::string name() const override { return "Bar"+std::to_string(id); };
//     RTTR_ENABLE(BarI)
// };

// class Fita {
// public:
//     BarI* bar;
//     std::string name() const { 
//         std::string barik = bar->name();
//         barik+= " i Fitusya";
//         return barik;
//     };
//     RTTR_ENABLE()
// };

// RTTR_REGISTRATION
// {
//     registration::class_<Fita>("Fita")
//         .constructor<>()
//         .method("name", &Fita::name)
//         .property("bar", &Fita::bar);

//     registration::class_<BarI>("BarI")
//         .method("name", &BarI::name);

//     registration::class_<Bar>("Bar")
//         .constructor<>()
//         .constructor<int>()
//         .method("name", &Bar::name)
//         .property("id", &Bar::id);
// }


class BazI {
public:
    virtual std::string say() const = 0; 
    RTTR_ENABLE()
};

class Baz : public BazI {
public:
    std::string say() const override {
        return "baz = " + val;
    }

    Baz(){};
    Baz(std::string val_) : val(val_) {};
public:
    std::string val;

    RTTR_ENABLE(BazI)
    RTTR_REGISTRATION_FRIEND
};

class BarI {
public:
    virtual std::string say() const = 0; 
    RTTR_ENABLE()
};

class Bar : public BarI {
public:
    std::string say() const override {
        return "bar + " + baz->say();
    }

    Bar(){};
private:
    BazI* baz;

    RTTR_ENABLE(BarI)
    RTTR_REGISTRATION_FRIEND
};

class BatI {
public:
    virtual std::string say() const = 0; 
    RTTR_ENABLE()
};

class Bat : public BatI {
public:
    std::string say() const override {
        return "bat + " + bar->say();
    }

    Bat(){};
private:
    BarI* bar;

    RTTR_ENABLE(BatI)
    RTTR_REGISTRATION_FRIEND
};

class FitaI {
public:
    virtual std::string say() const = 0; 
    RTTR_ENABLE()
};

class Fita : public FitaI {
public:
    std::string say() const override {
        return "fitochka + " + bat->say();
    }

    Fita(){};
private:
    BatI* bat;

    RTTR_ENABLE(FitaI)
    RTTR_REGISTRATION_FRIEND
};

RTTR_REGISTRATION
{    
    registration::class_<BazI>("BazI")
        .method("say", &BazI::say);

    registration::class_<Baz>("Baz")
        .constructor<>()
        .constructor<std::string>()
        .property("val", &Baz::val)
        .method("say", &Baz::say);
    
    registration::class_<BarI>("BarI")
        .method("say", &BarI::say);

    registration::class_<Bar>("Bar")
        .constructor<>()
        .property("baz", &Bar::baz, registration::private_access)
        .method("say", &Baz::say);
    
    registration::class_<BatI>("BatI")
        .method("say", &BatI::say);

    registration::class_<Bat>("Bat")
        .constructor<>()
        .property("bar", &Bat::bar, registration::private_access)
        .method("say", &Bat::say);
    
    registration::class_<FitaI>("FitaI")
        .method("say", &FitaI::say);

    registration::class_<Fita>("Fita")
        .constructor<>()
        .property("bat", &Fita::bat, registration::private_access)
        .method("say", &Fita::say);
}

int main() {
    di_container_::di_container di;

    di.register_instance<BazI,Baz>({"<3"}); // transient = default life type
    di.register_instance<BarI,Bar>({});
    di.register_instance<BatI,Bat>({});
    di.register_instance<FitaI,Fita>({});
    di.register_instance_singleton_simple<Baz>();

    di.get_instance<FitaI>()->say(); 
    di.get_instance<BatI>()->say(); 
    di.get_instance<BarI>()->say(); 
    di.get_instance<BazI>()->say();  
    
    std::shared_ptr<Baz> it = di.get_instance<Baz>();
    rttr::variant v(it);


    //std::cout<<rttr::type::get_by_name("Baz").get_property_value("val", v).get_value<std::string>()<<"\n";
    
    //di.register_field<Baz>("val","<<<<3");

    rttr::type::get_by_name("Baz").set_property_value("val", v, rttr::variant("kulak: zdorovo => fadin: zdorovey vidali"));
    
    std::cout<<it->val<<'\n';
    std::cout<<rttr::type::get_by_name("Baz").get_property_value("val",*v.get_value<std::shared_ptr<Baz>>().get()).get_value<std::string>()<<"\n";



    // di.register_field<BazI, std::string>("val", "kulak: zdorovo => fadin: zdorovey vidali");
    
    // std::cout<<"we have -> "<<di.get_instance<BazI>()->say()<<'\n';
    // assert(di.get_instance<BazI>()->say() == "baz = kulak: zdorovo => fadin: zdorovey vidali");

    // di_container_::di_container di;
    // //di.add_singleton<Bar>();
    // //di.add_singleton<Fita>();

    // //di.field_file_registration("input.txt");
    // di.register_instance<BarI,Bar>({1}, di_container_::lifetime_type::Singleton);
    // di.register_instance<Bar,Bar>({2}, di_container_::lifetime_type::Singleton);
    // di.register_instance<Fita,Fita>({}, di_container_::lifetime_type::Scoped);

    // std::cout<<di.get_instance<Bar>()->name()<<"\n\n";
    // //std::cout<<di.get_instance<Bar>()->id<<"\n\n";

    // di.get_instance<Bar>()->id = 142412;

    // std::cout<<di.get_instance<BarI>()->name()<<"\n\n";
    // //std::cout<<di.get_instance<Bar>()->id<<"\n\n";
    // std::cout<<"==="<<di.get_instance<Fita>()->name()<<"\n\n";
    // //std::cout<<"==="<<di.get_instance<Fita>()->bar->id<<"\n\n";

    return 0;
}