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
    template <class I, class C>
    bool register_instance_singleton(const std::vector<rttr::variant> params) {
        bool no_error = create_instance<I,C>(params);
        if(!no_error) {
            return false;
        }  
        add_singleton<I>();
        return true;
    }

    template <class C>
    bool register_instance_singleton_simple(const std::vector<rttr::variant> params) {
        return register_instance_singleton<C,C>(params);
    }
    template <class C>
    bool register_instance_singleton_simple() {
        return register_instance_singleton<C,C>({});
    }

    template <class I, class C>
    bool register_instance_scoped(const std::vector<rttr::variant> params) {
        bool no_error = create_instance<I,C>(params);
        if(!no_error) {
            return false;
        }  
        add_scoped<I>();
        return true;
    }

    template <class C>
    bool register_instance_scoped_simple(const std::vector<rttr::variant> params) {
        return register_instance_scoped<C,C>(params);
    }
    template <class C>
    bool register_instance_scoped_simple() {
        return register_instance_scoped<C,C>({});
    }

    template <class I, class C>
    bool register_instance_transient(const std::vector<rttr::variant> params) {
        bool no_error = create_instance<I,C>(params);
        if(!no_error) {
            return false;
        }  
        add_transient<I>();
        return true;
    }

    template <class C>
    bool register_instance_transient_simple(const std::vector<rttr::variant> params) {
        return register_instance_transient<C,C>(params);
    }
    template <class C>
    bool register_instance_transient_simple() {
        return register_instance_transient<C,C>({});
    }

    template <class C>
    bool register_instance_simple(const std::vector<rttr::variant> params) {
        return register_instance_transient<C,C>(params);
    }
    template <class C>
    bool register_instance_simple() {
        return register_instance_transient<C,C>({});
    }

    template <class I, class C>
    bool register_instance(const std::vector<rttr::variant> params, const lifetime_type& t) {
        bool no_error = create_instance<I,C>(params);
        if(!no_error) {
            return false;
        }  
        
        switch (t)
        {
            case Singleton : add_singleton<I>(); break;
            case Scoped    : add_scoped<I>(); break;
            case Transient : add_transient<I>();
        }
        return true;
    }

    template <class T, typename K> 
    void register_field(const std::string& field_name, const K& val) {
        registered_fields[std::string(rttr::type::get<T>().get_name())].emplace_back(field_name, val);      
    };

    template <typename K> 
    void register_field(const std::string& class_name, const std::string& field_name, const K& val) {
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

    void field_file_registration(const std::string& file_name) {
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

    template <class C> 
    std::shared_ptr<C> get_instance() {
        rttr::type t = rttr::type::get<C>();
        rttr::variant _instance = get_instance_by_type(t);
        
        std::shared_ptr<C> instance = _instance.get_value<std::shared_ptr<C>>();

        scoped_instances.clear();
        return instance;
    };

    // template <class I, class C> 
    // std::shared_ptr<I> get_instance() {
    //     rttr::type t = rttr::type::get<I>();
    //     rttr::variant _instance = get_instance_by_type(t);
        
    //     std::shared_ptr<C> instance = _instance.get_value<std::shared_ptr<C>>();

    //     scoped_instances.clear();
    //     return instance;
    // };

    //std::cout<<"otladka\n";
    ~di_container() {};
private:
    template <class I, class C>
    bool create_instance(const std::vector<rttr::variant> params) {
        rttr::type typeI = rttr::type::get<I>(), typeC = rttr::type::get<C>();
        std::string typeI_name(typeI.get_name());
        
        std::vector<rttr::type> params_types;
        for(auto &el : params) 
            params_types.emplace_back(el.get_type());

        std::vector<rttr::argument> args;
        for(const rttr::variant& el : params)
            args.emplace_back(rttr::argument(el));

        rttr::constructor ctor = typeC.get_constructor(params_types);
        rttr::variant instance = ctor.invoke_variadic(args);
        if(instance.is_valid()) {
            reg_instances[typeI_name] = instance;
            reg_ctor_params[typeI_name] = std::make_pair(std::string(typeC.get_name()), params);
            return true;
        }
        return false;
    }; 

    template <class T> 
    void add_singleton() {
        lifetime_types[std::string(rttr::type::get<T>().get_name())] = lifetime_type::Singleton;
    };

    template <class T> 
    void add_scoped() {
        lifetime_types[std::string(rttr::type::get<T>().get_name())] = lifetime_type::Scoped;
    };

    template <class T> 
    void add_transient() {
        lifetime_types[std::string(rttr::type::get<T>().get_name())] = lifetime_type::Transient;
    };

    template <class I>
    std::shared_ptr<I> get_reg_instance() {
        rttr::type typeI = rttr::type::get<I>();

        rttr::variant instance = reg_instances[std::string(typeI.get_name())];
        std::shared_ptr<I> instance_ptr = instance.get_value<std::shared_ptr<I>>();
        return instance_ptr;
    }
    
    template <class T>
    std::shared_ptr<T> get_copy_instance() {
        std::shared_ptr<T> instance = get_reg_instance<T>();
        return std::make_shared<T>(*instance.get());
    }

    rttr::variant get_copy_instance_by_name(const std::string& type_name) {
        if(reg_ctor_params.find(type_name) == reg_ctor_params.end())
            return nullptr;

        auto ctor_params = reg_ctor_params[type_name];

        std::vector<rttr::type> params_types;
        for(auto &el : ctor_params.second) 
            params_types.emplace_back(el.get_type());

        std::vector<rttr::argument> args;
        for(const rttr::variant& el : ctor_params.second)
            args.emplace_back(rttr::argument(el));

        return rttr::type::get_by_name(ctor_params.first).get_constructor(params_types).invoke_variadic(args);

        return nullptr;
    }

    rttr::variant get_instance_by_type(rttr::type t) {
        std::string type_name(t.get_name());

        if(singleton_instances.find(type_name) != singleton_instances.end())
            return singleton_instances[type_name];
        if(scoped_instances.find(type_name) != scoped_instances.end()) 
            return scoped_instances[type_name];
        
        rttr::variant ret_instance(get_copy_instance_by_name(type_name));
        if(!ret_instance.is_valid())
            return nullptr;

        if(registered_fields.find(type_name) != registered_fields.end()) {
            for(auto reg_field : registered_fields[type_name])
                t.set_property_value(reg_field.first, ret_instance, reg_field.second);
        }

        shared_ptrs.emplace_back(ret_instance);

        switch (lifetime_types[type_name])
        {
            case Singleton : singleton_instances[type_name] = ret_instance;
            case Scoped    : scoped_instances[type_name] = ret_instance;
            case Transient : /*do nothing*/;
        }

        for(auto &prop : t.get_properties()) {
            std::string prop_type_name(prop.get_type().get_name());

            if(prop_type_name.back()=='*') {
                prop_type_name.erase((--prop_type_name.end()));
                rttr::variant prop_instance = get_instance_by_type(rttr::type::get_by_name(prop_type_name));
                
                initialize_prop_with(ret_instance, prop, prop_instance.extract_wrapped_value());
            }    
        }
        
        return ret_instance;
    };

    bool initialize_prop_with(rttr::variant instance, rttr::property prop, rttr::variant obj) {
        if(obj.can_convert(prop.get_type())) {
            obj.convert(prop.get_type());
            return prop.set_value(instance, obj);
        }
        return false;
    };

private:
    std::map<std::string, rttr::variant> reg_instances, singleton_instances, scoped_instances;
    std::map<std::string, lifetime_type> lifetime_types;
    std::map<std::string, std::pair<std::string, std::vector<rttr::variant>>> reg_ctor_params;
    std::map<std::string, std::vector<std::pair<std::string, rttr::variant>>> registered_fields;
    std::vector<rttr::variant> shared_ptrs;    
};

// class di_container_second_version_depricated {
// public:
//     template <class T, typename K> 
//     void register_field(const std::string& field_name, const K& val) {
//         registered_fields[std::string(rttr::type::get<T>().get_name())].emplace_back(field_name, val);      
//     };

//     template <typename K> 
//     void register_field(const std::string& class_name, const std::string& field_name, const K& val) {
//         registered_fields[class_name].emplace_back(field_name, val);      
//     };

//     template <typename K>
//     void register_field(json &j) {
//         for(auto &cur_class : j.items()) {
//             for(auto &field : cur_class.value().items()) {
//                 register_field<K>(cur_class.key(), field.key(), field.value().get<K>());
//             }
//         }
//     }

//     void field_file_registration(const std::string& file_name) {
//         json j = json::parse(fopen(file_name.c_str(), "r"));
//         register_field<int>(j["int"]);
//         register_field<signed int>(j["signed int"]);
//         register_field<unsigned int>(j["unsigned int"]);
//         register_field<bool>(j["bool"]);
//         register_field<double>(j["double"]);
//         register_field<std::string>(j["std::string"]);
//         register_field<std::vector<bool>>(j["std::vector<bool>"]);
//         register_field<std::map<bool, bool>>(j["std::map<bool, bool>"]);
//     }

//     template <class T> 
//     std::shared_ptr<T> get_instance() {
//         rttr::type t = rttr::type::get<T>();
//         rttr::variant _instance = get_instance_by_type(t);
        
//         std::shared_ptr<T> instance = _instance.get_value<std::shared_ptr<T>>();

//         scoped_instances.clear();
//         return instance;
//     };

// private:
//     rttr::variant get_instance_by_type(rttr::type t) {
//         std::string type_name(t.get_name());

//         if(singleton_instances.find(type_name) != singleton_instances.end()) 
//             return singleton_instances[type_name];
//         if(scoped_instances.find(type_name) != scoped_instances.end()) 
//             return scoped_instances[type_name];
        
//         rttr::variant ret_instance;
//         rttr::constructor ctor = t.get_constructor({});
//         if(ctor) {
//             ret_instance = ctor.invoke();
//         }

//         if(!ret_instance)
//             return ret_instance;

//         if(registered_fields.find(type_name) != registered_fields.end()) {
//             for(auto reg_field : registered_fields[type_name])
//                 t.set_property_value(reg_field.first, ret_instance, reg_field.second);
//         }

//         shared_ptrs.emplace_back(ret_instance);

//         switch (lifetime_types[type_name])
//         {
//             case Singleton : singleton_instances[type_name] = ret_instance;
//             case Scoped    : scoped_instances[type_name] = ret_instance;
//             case Transient : /*do nothing*/;
//         }

        
//         for(auto &prop : t.get_properties()) {
//             rttr::variant prop_instance = get_instance_by_type(prop.get_type());
//             std::string prop_instance_type_name(prop_instance.get_type().get_name());
            
//             if(!prop_instance_type_name.empty())
//                 initialize_prop_with(ret_instance, prop, prop_instance);
//             else {
//                 std::string prop_type_name(prop.get_type().get_name());
//                 if(prop_type_name.back()=='*') {
//                     prop_type_name.erase((--prop_type_name.end()));
//                     prop_instance = get_instance_by_type(rttr::type::get_by_name(prop_type_name));
                    
//                     initialize_prop_with(ret_instance, prop, prop_instance.extract_wrapped_value());
//                 }
//             }        
//         }
        
//         return ret_instance;
//     };

//     bool initialize_prop_with(rttr::variant instance, rttr::property prop, rttr::variant obj) {
//         if(obj.can_convert(prop.get_type())) {
//             return prop.set_value(instance, obj);
//         }
//         return false;
//     };

// private:
//     std::map<std::string, rttr::variant> singleton_instances, scoped_instances;
//     std::map<std::string, lifetime_type> lifetime_types;
//     std::map<std::string, std::vector<std::pair<std::string, rttr::variant>>> registered_fields; 
//     std::vector<rttr::variant> shared_ptrs;
// };

}

#endif //_DI_CONTAINER_H_INCLUDED