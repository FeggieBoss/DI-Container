#ifndef _DI_CONTAINER_H_INCLUDED
#define _DI_CONTAINER_H_INCLUDED

#include "root_class.h"

#include <rttr/type>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>
#include <vector>

#include <stdio.h>

namespace di_container_ {
    inline int rng() {
        return (std::mt19937_64(std::chrono::steady_clock::now().time_since_epoch().count()))()%1000;
    };

    enum lifetime_type { Singleton, Scoped, Transient };

class di_container {
public:
    ~di_container() {};
    
    // DEPRICATED
    template <class T> 
    void ___add_instance() {
        T* new_instance = new T();

        ___initialize_instances_fields<T>(new_instance);
        ___instances.push_back(new_instance);
    };
    //

    // DEPRICATED
    template <class T> 
    std::vector<T*> ___get_prob_instances() const {
        std::vector<T*> prob_instances;
    
        for(root_class* instance : ___instances) {
            if(rttr::type::get<T>() == rttr::type::get(*instance)) {
                prob_instances.push_back((T*)instance);
                continue;
            }

            rttr::variant _instance = *instance;
            if(_instance.can_convert<T>() || dynamic_cast<T*>(instance)!=nullptr) {
                prob_instances.push_back((T*)instance);
            }
        }

        return prob_instances;
    };
    //

    //////////////////////
    template <class T, typename K> 
    void register_field(std::string field_name, const K& val) {
        registered_fields[std::string(rttr::type::get<T>().get_name())].emplace_back(field_name, val);      
    };

    template <typename K> 
    void register_field(std::string class_name, std::string field_name, const K& val) {
        registered_fields[class_name].emplace_back(field_name, val);      
    };

    template <typename K>
    void register_field(json &j) {
        for(auto &cur_class : j.items()) {
            for(auto &field : cur_class.value().items()) {
                register_field<K>(cur_class.key(), field.key(), field.value().get<K>());
            }
        }
    }

    void file_registration(std::string file_name) {
        json j = json::parse(fopen(file_name.c_str(), "r"));
        register_field<int>(j["int"]);
        register_field<signed int>(j["signed int"]);
        register_field<unsigned int>(j["unsigned int"]);
        register_field<bool>(j["bool"]);
        register_field<double>(j["double"]);
        register_field<std::string>(j["std::string"]);
        register_field<std::vector<bool>>(j["std::vector<bool>"]);
        register_field<std::map<bool, bool>>(j["std::map<bool, bool>"]);
    }

    template <class T> 
    void add_singleton() {
        // DEPRICATED add_constructor<T>();
        lifetime_types[std::string(rttr::type::get<T>().get_name())] = lifetime_type::Singleton;
    };

    template <class T> 
    void add_scoped() {
        // DEPRICATED add_constructor<T>();
        lifetime_types[std::string(rttr::type::get<T>().get_name())] = lifetime_type::Scoped;
    };

    template <class T> 
    void add_transient() {
        // DEPRICATED add_constructor<T>();
        lifetime_types[std::string(rttr::type::get<T>().get_name())] = lifetime_type::Transient;
    };

    template <class T> 
    std::shared_ptr<T> get_instance() {
        rttr::type t = rttr::type::get<T>();
        rttr::variant _instance = get_instance_by_type(t);
        
        std::shared_ptr<T> instance = _instance.get_value<std::shared_ptr<T>>();

        scoped_instances.clear();
        return instance;
    };
    //////////////////////

private:
    // DEPRICATED
    template <class T> 
    void ___initialize_instances_fields(T* new_instance_ptr) {
        for(root_class* old_instance_ptr : ___instances) {
            for(auto &prop : rttr::type::get(*old_instance_ptr).get_properties()) {
                __initialize_prop_with(old_instance_ptr, prop, new_instance_ptr);
            }

            for(auto &prop : rttr::type::get(*new_instance_ptr).get_properties()) {
                __initialize_prop_with(new_instance_ptr, prop, old_instance_ptr);
            }
        }
    };
    //

    // DEPRICATED
    bool __initialize_prop_with(root_class* instance_ptr, rttr::property prop, root_class* obj_ptr) {
        rttr::variant obj = *obj_ptr;
        root_class &obj_link = *obj_ptr;

        if(obj.can_convert(prop.get_type()))
            return prop.set_value(*instance_ptr, *obj_ptr);

        if(is_link_to(prop.get_type(), rttr::type::get(obj_link))) 
            return prop.set_value(*instance_ptr, obj_ptr);

        return false;
    };
    //

    bool is_link_to(rttr::type a, rttr::type b) {
        std::string a_type_name(a.get_name());
        std::string b_type_name(b.get_name());

        a_type_name+="*";
        return (a_type_name == b_type_name);
    };


    //////////////////////
    template <class T> 
    void add_constructor() {
        rttr::type t = rttr::type::get<T>();
        std::string type_name(t.get_name());

        constructors.push_back(t.get_constructor({}));
    };
    

    rttr::variant get_instance_by_type(rttr::type t) {
        std::string type_name(t.get_name());
        //if(type_name=="int") return rttr::variant();

        //std::cout<<"============= ask type - "<<type_name<<"\n";
        if(singleton_instances.find(type_name) != singleton_instances.end()) 
            return singleton_instances[type_name];
        if(scoped_instances.find(type_name) != scoped_instances.end()) 
            return scoped_instances[type_name];
        
        rttr::variant ret_instance;
        rttr::constructor ctor = t.get_constructor({});
        if(ctor) {
            ret_instance = ctor.invoke();
            //std::cout<<"ask type - "<<type_name<<"\n\n";
            //std::cout<<"ctor type - "<< ret_instance.get_type().get_name()<<'\n';
        }

        if(!ret_instance)
            return ret_instance;

        if(registered_fields.find(type_name) != registered_fields.end()) {
            for(auto reg_field : registered_fields[type_name])
                t.set_property_value(reg_field.first, ret_instance, reg_field.second);
        }

        ///////////////// TO DO
        pomoyka.emplace_back(ret_instance);
        ////////////////
        switch (lifetime_types[type_name])
        {
            case Singleton : singleton_instances[type_name] = ret_instance;
            case Scoped    : scoped_instances[type_name] = ret_instance;
            case Transient : /*do nothing*/;
        }

        
        for(auto &prop : t.get_properties()) {
            rttr::variant prop_instance = get_instance_by_type(prop.get_type());
            std::string prop_instance_type_name(prop_instance.get_type().get_name());
            // std::cout<<"prop_instance_type_name_sz is "<<prop_instance_type_name.size()<<'\n';
            if(!prop_instance_type_name.empty())
                initialize_prop_with(ret_instance, prop, prop_instance);
            else {
                std::string prop_type_name(prop.get_type().get_name());

                //std::cout<<"prop_type_name is "<<prop_type_name<<'\n';
                //if(prop_type_name=="int") {
                //    std::cout<<"int:" << (prop.get_value(ret_instance).get_value<int>()) <<'\n';
                //}
                if(prop_type_name.back()=='*') {
                    prop_type_name.erase((--prop_type_name.end()));
                    prop_instance = get_instance_by_type(rttr::type::get_by_name(prop_type_name));
                    
                    initialize_prop_with(ret_instance, prop, prop_instance.extract_wrapped_value());
                }
            }        
        }
        
        return ret_instance;
    };

    bool initialize_prop_with(rttr::variant instance, rttr::property prop, rttr::variant obj) {
        //root_class &obj_link = *obj_ptr;

        
        // std::cout<<"instance type - "<<instance.get_type().get_name()<<'\n';
        // std::cout<<"obj type - "<<obj.get_type().get_name()<<'\n';
        // std::cout<<"prop type - "<<prop.get_type().get_name()<<'\n';
        

        if(obj.can_convert(prop.get_type())) {
            //std::cout<<"we did it\n";
            //obj.convert(prop.get_type());
            return prop.set_value(instance, obj);
        }
        /*if(is_link_to(prop.get_type(), rttr::type::get(obj_link))) 
            return prop.set_value(*instance_ptr, obj_ptr);*/

        return false;
    };

    
    //////////////////////

private:
    // DEPRICATED
    std::vector<root_class*> ___instances;
    //

    ////////////////////// DEPRICATED
    std::vector<rttr::constructor> constructors;
    //////////////////////
    std::map<std::string, rttr::variant> singleton_instances, scoped_instances;
    std::map<std::string, lifetime_type> lifetime_types;
    std::map<std::string, std::vector<std::pair<std::string, rttr::variant>>> registered_fields; 

    ////////////////////// TO DO
    std::vector<rttr::variant> pomoyka;
    //////////////////////
};

}

//#include "di_container_impl.h"

#endif //_DI_CONTAINER_H_INCLUDED