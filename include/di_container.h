#ifndef _DI_CONTAINER_H_INCLUDED
#define _DI_CONTAINER_H_INCLUDED

#include "root_class.h"

#include <rttr/type>
#include <vector>

namespace di_container_ {

class di_container {
public:
    ~di_container();
    
    template <class T> void add_instance();
    template <class T> std::vector<T*> get_prob_instances() const;
private:
    template <class T> void initialize_instances_fields(T* new_instance_ptr);
    bool initialize_prop_with(root_class* instance_ptr, rttr::property prop, root_class* obj_ptr);

private:
    std::vector<root_class*> instances;
};

}

#include "di_container_impl.h"

#endif //_DI_CONTAINER_H_INCLUDED