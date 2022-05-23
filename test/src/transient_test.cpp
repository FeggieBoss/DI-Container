#include <gtest/gtest.h>
#include <rttr/registration>
using namespace rttr;

#include "di_container.h"

/*
    Fita(singleton)--->Bar(transient)
                   |-->Baz(singleton)-->Bar(transient)
*/

class TransientTestBar {
public:
    int id;
    TransientTestBar() : id(di_container_::rng()) {};
    RTTR_ENABLE()
};

class TransientTestBaz {
public:
    TransientTestBar* bar;
    RTTR_ENABLE()
};

class TransientTestFita {
public:
    TransientTestBar* bar;
    TransientTestBaz* baz;
    RTTR_ENABLE()
};


RTTR_REGISTRATION
{
    registration::class_<TransientTestFita>("TransientTestFita")
        .constructor<>()
        .property("bar", &TransientTestFita::bar)
        .property("baz", &TransientTestFita::baz);

    registration::class_<TransientTestBaz>("TransientTestBaz")
        .constructor<>()
        .property("bar", &TransientTestBaz::bar);

    registration::class_<TransientTestBar>("TransientTestBar")
        .constructor<>()
        .property("id", &TransientTestBar::id);
}

class TransientTest : public ::testing::Test {
protected:
    // void SetUp() override;
    void SetUp() override {
        di.register_instance_singleton_simple<TransientTestFita>();
        di.register_instance_singleton_simple<TransientTestBaz>();
        di.register_instance_transient_simple<TransientTestBar>();
    }   

    // void TearDown() override;

    di_container_::di_container di;
};

TEST_F(TransientTest, SingleGetInstance) {
    auto Bar_instance1 = di.get_instance<TransientTestBar>();
    auto Bar_instance2 = di.get_instance<TransientTestBar>();

    // std::cout<<"Bar_instance1->id::"<<Bar_instance1->id<<'\n';
    // std::cout<<"Bar_instance2->id::"<<Bar_instance2->id<<'\n';
    EXPECT_NE(Bar_instance1->id, Bar_instance2->id) << "No rebuild happened in different get instance calls";
}

TEST_F(TransientTest, TransientFieldsSingleGetInstance) {
    auto Fita_instance = di.get_instance<TransientTestFita>();

    // std::cout<<"Fita_instance->bar->id::"<<Fita_instance->bar->id<<'\n';
    // std::cout<<"Fita_instance->baz->bar->id::"<<Fita_instance->baz->bar->id<<'\n';
    EXPECT_NE(Fita_instance->bar->id, Fita_instance->baz->bar->id) << "No rebuild happened in same get instance call";
}

TEST_F(TransientTest, TransientFieldsOfSingletonClassMultipleGetInstance) {
    auto Fita_instance1 = di.get_instance<TransientTestFita>();
    auto Fita_instance2 = di.get_instance<TransientTestFita>();

    // std::cout<<"Fita_instance1->bar->id::"<<Fita_instance1->bar->id<<'\n';
    // std::cout<<"Fita_instance1->baz->bar->id::"<<Fita_instance1->baz->bar->id<<'\n';
    // std::cout<<"Fita_instance2->bar->id::"<<Fita_instance2->bar->id<<'\n';
    // std::cout<<"Fita_instance2->baz->bar->id::"<<Fita_instance2->baz->bar->id<<'\n';
    EXPECT_NE(Fita_instance1->bar->id, Fita_instance1->baz->bar->id) << "No rebuild happened in same get instance call";
    EXPECT_NE(Fita_instance2->bar->id, Fita_instance2->baz->bar->id) << "No rebuild happened in same get instance call";
    EXPECT_EQ(Fita_instance1->bar->id, Fita_instance2->bar->id) << "Rebuild happened in Transient field of SINGLETON Fita::bar in get instance calls";
}

TEST_F(TransientTest, TransientFieldsOfTransientClassMultipleGetInstance) {
/*
    Fita(transient)--->Bar(scoped)
                   |-->Baz(transient)-->Bar(scoped)    
                    
    want to check rebuilding in get_instance calls
*/
    di.register_instance_transient_simple<TransientTestFita>();
    di.register_instance_transient_simple<TransientTestBaz>();

    auto Fita_instance1 = di.get_instance<TransientTestFita>();
    auto Fita_instance2 = di.get_instance<TransientTestFita>();

    // std::cout<<"Fita_instance1->bar->id::"<<Fita_instance1->bar->id<<'\n';
    // std::cout<<"Fita_instance1->baz->bar->id::"<<Fita_instance1->baz->bar->id<<'\n';
    // std::cout<<"Fita_instance2->bar->id::"<<Fita_instance2->bar->id<<'\n';
    // std::cout<<"Fita_instance2->baz->bar->id::"<<Fita_instance2->baz->bar->id<<'\n';
    EXPECT_NE(Fita_instance1->bar->id, Fita_instance1->baz->bar->id) << "No rebuild happened in same get instance call";
    EXPECT_NE(Fita_instance2->bar->id, Fita_instance2->baz->bar->id) << "No rebuild happened in same get instance call";
    EXPECT_NE(Fita_instance1->bar->id, Fita_instance2->bar->id) << "No rebuild happened in different get instance calls";
}

TEST_F(TransientTest, MultipleCallAddTransient) {
    di_container_::di_container di;
    di.register_instance_transient_simple<TransientTestBar>();
    di.register_instance_transient_simple<TransientTestBar>();

    auto Bar_instance1 = di.get_instance<TransientTestBar>();
    auto Bar_instance2 = di.get_instance<TransientTestBar>();

    // std::cout<<"Bar_instance1->id::"<<Bar_instance1->id<<'\n';
    // std::cout<<"Bar_instance2->id::"<<Bar_instance2->id<<'\n';
    EXPECT_NE(Bar_instance1->id, Bar_instance2->id) << "No rebuild happened in different get instance calls";
}