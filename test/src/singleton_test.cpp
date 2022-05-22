#include <gtest/gtest.h>
#include <rttr/registration>
using namespace rttr;

#include "di_container.h"

/*
    Bar(scoped)--->Fita(singleton)<---Baz(transient)
*/

class SingletonTestFita : public root_class {
public:
    int id;
    SingletonTestFita() : id(di_container_::rng()) {};
    RTTR_ENABLE(root_class)
};

class SingletonTestBar : public root_class {
public:
    SingletonTestFita* bar_fita;
    RTTR_ENABLE(root_class)
};

class SingletonTestBaz : public root_class {
public:
    SingletonTestFita* baz_fita;
    RTTR_ENABLE(root_class)
};


RTTR_REGISTRATION
{
    registration::class_<root_class>("root_class")
        .constructor<>();
    
    registration::class_<SingletonTestFita>("SingletonTestFita")
        .constructor<>()
        .property("id", &SingletonTestFita::id);

    registration::class_<SingletonTestBar>("SingletonTestBar")
        .constructor<>()
        .property("bar_fita", &SingletonTestBar::bar_fita);
    
    registration::class_<SingletonTestBaz>("SingletonTestBaz")
        .constructor<>()
        .property("baz_fita", &SingletonTestBaz::baz_fita);
}

class SingletonTest : public ::testing::Test {
protected:
    // void SetUp() override;
    void SetUp() override {
        di.register_instance_singleton_simple<SingletonTestFita>();
        di.register_instance_scoped_simple<SingletonTestBar>();
        di.register_instance_transient_simple<SingletonTestBaz>();
    }

    // void TearDown() override;

    di_container_::di_container di;   
};

TEST_F(SingletonTest, NoRebuildOccurs) {
    auto Fita_instance1 = di.get_instance<SingletonTestFita>();
    int old_id = Fita_instance1->id;
    auto Fita_instance2 = di.get_instance<SingletonTestFita>();
    int new_id = Fita_instance2->id;

    // std::cout<<"old_id::"<<old_id<<'\n';
    // std::cout<<"new_id::"<<new_id<<'\n';
    EXPECT_EQ(old_id, new_id) << "An object of the class Fita was recreated";
}

TEST_F(SingletonTest, MultipleCallGetInstance) {
    auto Fita_instance1 = di.get_instance<SingletonTestFita>();
    auto Fita_instance2 = di.get_instance<SingletonTestFita>();

    // std::cout<<"Fita_instance1.get()::"<<Fita_instance1.get()<<'\n';
    // std::cout<<"Fita_instance2.get()::"<<Fita_instance2.get()<<'\n';
    EXPECT_EQ(Fita_instance1.get(), Fita_instance2.get()) << "Different objects of class Fita were created";
}

TEST_F(SingletonTest, MultipleCallAddSingleton) {
    di.register_instance_singleton_simple<SingletonTestFita>();

    auto Fita_instance1 = di.get_instance<SingletonTestFita>();
    auto Fita_instance2 = di.get_instance<SingletonTestFita>();

    // std::cout<<"Fita_instance1.get()::"<<Fita_instance1.get()<<'\n';
    // std::cout<<"Fita_instance2.get()::"<<Fita_instance2.get()<<'\n';
    EXPECT_EQ(Fita_instance1.get(), Fita_instance2.get()) << "Different objects of class Fita were created";
}

TEST_F(SingletonTest, SingletonFields) {
    auto Bar_instance1 = di.get_instance<SingletonTestBar>();
    auto Bar_instance2 = di.get_instance<SingletonTestBar>();
    auto Baz_instance1 = di.get_instance<SingletonTestBaz>();
    auto Baz_instance2 = di.get_instance<SingletonTestBaz>();

    // std::cout<<"Bar_instance1->bar_fita::"<<Bar_instance1->bar_fita<<'\n';
    // std::cout<<"Bar_instance2->bar_fita::"<<Bar_instance2->bar_fita<<'\n';
    // std::cout<<"Baz_instance1->baz_fita::"<<Baz_instance1->baz_fita<<'\n';
    // std::cout<<"Baz_instance2->baz_fita::"<<Baz_instance2->baz_fita<<'\n';
    EXPECT_EQ(Bar_instance1->bar_fita, Bar_instance2->bar_fita) << "Different objects of class Fita were created - Bar::bar_fita";
    EXPECT_EQ(Baz_instance1->baz_fita, Baz_instance2->baz_fita) << "Different objects of class Fita were created - Baz::baz_fita";
    EXPECT_EQ(Bar_instance1->bar_fita, Baz_instance1->baz_fita) << "Different objects of class Fita were created - Bar::bar_fita/Baz::baz_fita";
}