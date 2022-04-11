#ifndef _DI_CONTAINER_H_INCLUDED
#define _DI_CONTAINER_H_INCLUDED

#include "root_class.h"

#include <rttr/type>
#include <vector>

namespace di_container_ {

class di_container {
public:
    ~di_container() {};
    
    template <class T> 
    void add_instance() {
        T* new_instance = new T();

        initialize_instances_fields<T>(new_instance);
        instances.push_back(new_instance);
    };

    template <class T> 
    std::vector<T*> get_prob_instances() const {
        std::vector<T*> prob_instances;
    
        for(root_class* instance : instances) {
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

private:
    template <class T> 
    void initialize_instances_fields(T* new_instance_ptr) {
        for(root_class* old_instance_ptr : instances) {
            for(auto &prop : rttr::type::get(*old_instance_ptr).get_properties()) {
                initialize_prop_with(old_instance_ptr, prop, new_instance_ptr);
            }

            for(auto &prop : rttr::type::get(*new_instance_ptr).get_properties()) {
                initialize_prop_with(new_instance_ptr, prop, old_instance_ptr);
            }
        }

    //rttr::variant new_instance = *new_instance_ptr;
    //root_class &new_instance_link = *new_instance_ptr;

    //for(root_class* old_instance_ptr : instances) {  
        /*rttr::variant old_instance_ = *old_instance_ptr;
        root_class &old_instance_link = *old_instance_ptr;

        for(auto &prop : rttr::type::get(*old_instance_ptr).get_properties()) {
            if(new_instance.can_convert(prop.get_type())) {
                prop.set_value(*old_instance_ptr, *new_instance_ptr);
                break;
            }
            else if(is_link_to(prop.get_type(), rttr::type::get(new_instance_link))) {
                prop.set_value(*old_instance_ptr, new_instance_ptr);
                break;
            }
        }

        for(auto &prop : rttr::type::get(*new_instance_ptr).get_properties()) { 
            if(old_instance_.can_convert(prop.get_type())) {
                prop.set_value(*new_instance_ptr, *old_instance_ptr);
                break;
            }
            else if(is_link_to(prop.get_type(), rttr::type::get(old_instance_link))) {
                prop.set_value(*new_instance_ptr, old_instance_ptr);
                break;
            }
        }*/
    //}
    };

    bool initialize_prop_with(root_class* instance_ptr, rttr::property prop, root_class* obj_ptr) {
        rttr::variant obj = *obj_ptr;
        root_class &obj_link = *obj_ptr;

        if(obj.can_convert(prop.get_type()))
            return prop.set_value(*instance_ptr, *obj_ptr);

        if(is_link_to(prop.get_type(), rttr::type::get(obj_link))) 
            return prop.set_value(*instance_ptr, obj_ptr);

        return false;
    };

    bool is_link_to(rttr::type a, rttr::type b) {
        std::string a_type_name(a.get_name());
        std::string b_type_name(b.get_name());

        a_type_name+="*";
        return (a_type_name == b_type_name);
    };

private:
    std::vector<root_class*> instances;
};

}

//#include "di_container_impl.h"

#endif //_DI_CONTAINER_H_INCLUDED