#include <gtest/gtest.h>
#include <rttr/registration>

#include "di_container.h"

using namespace rttr;
using namespace di_container_;

/*
    Bar { int*, string }
    Fita { int*, string }

    Fita(singleton) <-- Bar(singleton)
    Fita constructor Fita(Bar) {}
*/

class Bar {
public:
    int* ptr;
    std::string name = "everlasting";

    Bar(int *ptr_) : ptr(ptr_) {};
    Bar(int *ptr_, std::string name_) : ptr(ptr_), name(name_) {};

    RTTR_ENABLE()
};

class Fita {
public:
    int *ptr;
    std::string name = "summer";
    Bar bar;      // we do it ourselves
    Bar* bar_ptr; // we mark for DI-container in RTTR_REGISTRATION that these are his duties
    
    Fita(int *ptr_, Bar bar_) : ptr(ptr_), bar(bar_)  {};
    Fita(int *ptr_, std::string name_, Bar bar_) : ptr(ptr_), name(name_), bar(bar_) {};

    RTTR_ENABLE()
};

RTTR_REGISTRATION
{
    registration::class_<Bar>("Bar")
        .constructor<int*>()
        .constructor<int*, std::string>()
        .property("ptr", &Bar::ptr)
        .property("name", &Bar::name);

    registration::class_<Fita>("Fita")
        .constructor<int*, Bar>()
        .constructor<int*, std::string, Bar>()
        .property("ptr", &Fita::ptr)
        .property("name", &Fita::name)
        .property("bar", &Fita::bar)
        .property("bar_ptr", &Fita::bar_ptr);
}

class MixedRegistrationTest : public ::testing::Test {
protected:
    int* di_int_ptr;

    // void SetUp() override;
    void SetUp() override {
        di_int_ptr = new int[1];
        *di_int_ptr = 80101;

        di.register_instance<Bar, Bar>({(int*)di_int_ptr}, lifetime_type::Singleton);
        di.register_instance<Fita, Fita>({(int*)di_int_ptr, Bar(di_int_ptr)}, lifetime_type::Singleton);
    }
    
    // void TearDown() override;
    void TearDown() override {
        delete[] di_int_ptr;
    }

    di_container di; 
};

TEST_F(MixedRegistrationTest, NoErrors) {
    EXPECT_NO_THROW({ 
        di.get_instance<Fita>()->ptr; 
        di.get_instance<Fita>()->bar.ptr;
        di.get_instance<Fita>()->bar_ptr->ptr; 
    });
}

TEST_F(MixedRegistrationTest, CorrectValues) {
    EXPECT_EQ(*di.get_instance<Fita>()->ptr, 80101);
    EXPECT_EQ(*di.get_instance<Fita>()->bar.ptr, 80101);
    EXPECT_EQ(*di.get_instance<Fita>()->bar_ptr->ptr, 80101);
}

TEST_F(MixedRegistrationTest, CheckRegistrationThroughInstance) {
    *di.get_instance<Fita>()->ptr = 101;

    EXPECT_EQ(*di.get_instance<Fita>()->ptr, 101)<<"Pointer to the same memory";
    EXPECT_EQ(*di.get_instance<Fita>()->bar.ptr, 101)<<"Pointer to the same memory";
    EXPECT_EQ(*di.get_instance<Fita>()->bar_ptr->ptr, 101)<<"Pointer to the same memory";
}

TEST_F(MixedRegistrationTest, CheckRegistrationThroughInstance2) {
    // re-registration replaces the old one
    di.register_instance<Bar, Bar>({(int*)di_int_ptr}, lifetime_type::Transient);
    
    *di.get_instance<Fita>()->ptr = 101;

    EXPECT_EQ(*di.get_instance<Fita>()->ptr, 101)<<"Pointer to the same memory";
    EXPECT_EQ(*di.get_instance<Fita>()->bar.ptr, 101)<<"Pointer to the same memory";
    EXPECT_EQ(*di.get_instance<Fita>()->bar_ptr->ptr, 101)<<"We copied the pointer, not the memory";
}

TEST_F(MixedRegistrationTest, CheckRegistrationThroughInstance3) {
    int* test_int_ptr = new int[1];
    *test_int_ptr = 5;

    // re-registration replaces the old one
    di.register_instance<Fita, Fita>({(int*)di_int_ptr, Bar(di_int_ptr)}, lifetime_type::Scoped); // any lifetime will do
    di.register_instance<Bar, Bar>({(int*)test_int_ptr}, lifetime_type::Scoped); // any lifetime will do
    *di.get_instance<Fita>()->ptr = 101;

    EXPECT_EQ(*di.get_instance<Fita>()->ptr, 101)<<"We copied the pointer, not the memory";
    // Important difference!!!!!!!!!
    EXPECT_EQ(*di.get_instance<Fita>()->bar.ptr, 101)<<"Memory was changed at this place using a copy of the pointer (this bar is not associated with DI)";
    EXPECT_EQ(*di.get_instance<Fita>()->bar_ptr->ptr, 5)<<"Re-registered pointer";

    di.register_instance<Fita, Fita>({(int*)di_int_ptr, Bar(test_int_ptr)}, lifetime_type::Scoped);
    EXPECT_EQ(*di.get_instance<Fita>()->bar.ptr, 5)<<"Added another pointer";

    delete[] test_int_ptr;
}


