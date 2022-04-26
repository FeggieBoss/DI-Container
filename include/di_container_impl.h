#ifndef _DI_CONTAINER_IMPL_H_INCLUDED
#define _DI_CONTAINER_IMPL_H_INCLUDED

//#include "di_container.h"

using namespace di_container_;

template <class T>
void di_container::add_instance() {
    T* new_instance = new T();

    initialize_instances_fields<T>(new_instance);
    instances.push_back(new_instance);
}

template <class T>
std::vector<T*> di_container::get_prob_instances() const {
    std::vector<T*> prob_instances;
    
    for(root_class* instance : instances) {
        if(rttr::type::get<T>() == rttr::type::get(*instance)) {
            prob_instances.push_back((T*)instance);
            continue;
        }

        rttr::variant _instance = *instance;
        if(_instance.can_convert<T>()) 
            prob_instances.push_back((T*)instance);
    }

    return prob_instances;
}

template <class T>
void di_container::initialize_instances_fields(T* new_instance_ptr) {
    for(root_class* old_instance_ptr : instances) {
        for(auto &prop : rttr::type::get(*old_instance_ptr).get_properties()) {
            initialize_prop_with(old_instance_ptr, prop, new_instance_ptr);
        }

        for(auto &prop : rttr::type::get(*new_instance_ptr).get_properties()) {
            initialize_prop_with(new_instance_ptr, prop, old_instance_ptr);
        }
    }
}

bool di_container::initialize_prop_with(root_class* instance_ptr, rttr::property prop, root_class* obj_ptr) {
    rttr::variant obj = *obj_ptr;
    root_class &obj_link = *obj_ptr;

    if(obj.can_convert(prop.get_type()))
        return prop.set_value(*instance_ptr, *obj_ptr);
    
    if(is_link_to(prop.get_type(), rttr::type::get(obj_link))) 
        return prop.set_value(*instance_ptr, obj_ptr);

    return false;
}

di_container::~di_container() {
    for(auto instance : instances)
        delete instance;
    
    instances.clear();
}

#endif //_DI_CONTAINER_IMPL_H_INCLUDED