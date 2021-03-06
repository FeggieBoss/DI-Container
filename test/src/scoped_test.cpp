#include <gtest/gtest.h>
#include <rttr/registration>
using namespace rttr;

#include "di_container.h"

/*
    Fita(singleton)--->Bar(scoped)
                   |-->Baz(singleton)-->Bar(scoped)
      
*/

class ScopedTestBar {
public:
    int id;
    ScopedTestBar() : id(di_container_::rng()) {};
    RTTR_ENABLE()
};

class ScopedTestBaz {
public:
    ScopedTestBar* bar;
    RTTR_ENABLE()
};

class ScopedTestFita {
public:
    ScopedTestBar* bar;
    ScopedTestBaz* baz;
    RTTR_ENABLE()
};


RTTR_REGISTRATION
{
    registration::class_<ScopedTestFita>("ScopedTestFita")
        .constructor<>()
        .property("bar", &ScopedTestFita::bar)
        .property("baz", &ScopedTestFita::baz);

    registration::class_<ScopedTestBaz>("ScopedTestBaz")
        .constructor<>()
        .property("bar", &ScopedTestBaz::bar);

    registration::class_<ScopedTestBar>("ScopedTestBar")
        .constructor<>()
        .property("id", &ScopedTestBar::id);
}

class ScopedTest : public ::testing::Test {
protected:
    // void SetUp() override;
    void SetUp() override {
        di.register_instance_singleton_simple<ScopedTestFita>();
        di.register_instance_singleton_simple<ScopedTestBaz>();
        di.register_instance_scoped_simple<ScopedTestBar>();
    }   

    // void TearDown() override;

    di_container_::di_container di;
};

TEST_F(ScopedTest, SingleGetInstance) {
    auto Bar_instance1 = di.get_instance<ScopedTestBar>();
    auto Bar_instance2 = di.get_instance<ScopedTestBar>();

    // std::cout<<"Bar_instance1->id::"<<Bar_instance1->id<<'\n';
    // std::cout<<"Bar_instance2->id::"<<Bar_instance2->id<<'\n';
    EXPECT_NE(Bar_instance1->id, Bar_instance2->id) << "No rebuild happened in DIFFERENT get instance calls";
}

TEST_F(ScopedTest, ScopedFieldsSingleGetInstance) {
    auto Fita_instance = di.get_instance<ScopedTestFita>();

    // std::cout<<"Fita_instance->bar->id::"<<Fita_instance->bar->id<<'\n';
    // std::cout<<"Fita_instance->baz->bar->id::"<<Fita_instance->baz->bar->id<<'\n';
    EXPECT_EQ(Fita_instance->bar->id, Fita_instance->baz->bar->id) << "Rebuild happened in SAME get instance call";
}

TEST_F(ScopedTest, ScopedFieldsOfSingletonClassMultipleGetInstance) {
    auto Fita_instance1 = di.get_instance<ScopedTestFita>();
    auto Fita_instance2 = di.get_instance<ScopedTestFita>();

    // std::cout<<"Fita_instance1->bar->id::"<<Fita_instance1->bar->id<<'\n';
    // std::cout<<"Fita_instance1->baz->bar->id::"<<Fita_instance1->baz->bar->id<<'\n';
    // std::cout<<"Fita_instance2->bar->id::"<<Fita_instance2->bar->id<<'\n';
    // std::cout<<"Fita_instance2->baz->bar->id::"<<Fita_instance2->baz->bar->id<<'\n';
    EXPECT_EQ(Fita_instance1->bar->id, Fita_instance1->baz->bar->id) << "Rebuild happened in SAME get instance call";
    EXPECT_EQ(Fita_instance2->bar->id, Fita_instance2->baz->bar->id) << "Rebuild happened in SAME get instance call";
    EXPECT_EQ(Fita_instance1->bar->id, Fita_instance2->bar->id) << "Rebuild happened in scoped field of SINGLETON Fita::bar in get instance calls";
}

TEST_F(ScopedTest, ScopedFieldsOfTransientClassMultipleGetInstance) {
/*
    Fita(transient)--->Bar(scoped)
                   |-->Baz(transient)-->Bar(scoped)    
                    
    want to check rebuilding in different get_instance calls, and no rebuilding in same 
*/
    di.register_instance_scoped_simple<ScopedTestFita>();
    di.register_instance_transient_simple<ScopedTestBaz>();

    auto Fita_instance1 = di.get_instance<ScopedTestFita>();
    auto Fita_instance2 = di.get_instance<ScopedTestFita>();

    // std::cout<<"Fita_instance1->bar->id::"<<Fita_instance1->bar->id<<'\n';
    // std::cout<<"Fita_instance1->baz->bar->id::"<<Fita_instance1->baz->bar->id<<'\n';
    // std::cout<<"Fita_instance2->bar->id::"<<Fita_instance2->bar->id<<'\n';
    // std::cout<<"Fita_instance2->baz->bar->id::"<<Fita_instance2->baz->bar->id<<'\n';
    EXPECT_EQ(Fita_instance1->bar->id, Fita_instance1->baz->bar->id) << "Rebuild happened in SAME get instance call";
    EXPECT_EQ(Fita_instance2->bar->id, Fita_instance2->baz->bar->id) << "Rebuild happened in SAME get instance call";
    EXPECT_NE(Fita_instance1->bar->id, Fita_instance2->bar->id) << "No rebuild happened in DIFFERENT get instance calls";
}

TEST_F(ScopedTest, MultipleCallAddScoped) {
    di_container_::di_container di;
    di.register_instance_scoped_simple<ScopedTestBar>();
    di.register_instance_scoped_simple<ScopedTestBar>();

    auto Bar_instance1 = di.get_instance<ScopedTestBar>();
    auto Bar_instance2 = di.get_instance<ScopedTestBar>();

    // std::cout<<"Bar_instance1->id::"<<Bar_instance1->id<<'\n';
    // std::cout<<"Bar_instance2->id::"<<Bar_instance2->id<<'\n';
    EXPECT_NE(Bar_instance1->id, Bar_instance2->id) << "No rebuild happened in DIFFERENT get instance calls";
}