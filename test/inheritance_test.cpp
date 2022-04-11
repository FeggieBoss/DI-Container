#include <gtest/gtest.h>
#include <rttr/registration>
using namespace rttr;

#include "di_container.h"

/*
root_class
 ^
 |
Fita
 ^  
 |
Bar
*/

class Fita : public root_class {
public:
    virtual std::string name() const { return "Fita"; };
    RTTR_ENABLE(root_class)
};

class Bar : public Fita {
public:
    std::string name() const override { return "Bar"; };
    RTTR_ENABLE(Fita)
};

RTTR_REGISTRATION
{
    registration::class_<Fita>("root_class")
        .constructor<>();

    registration::class_<Fita>("Fita")
        .constructor<>()
        .method("name", &Fita::name);

    registration::class_<Bar>("Bar")
        .constructor<>()
        .method("name", &Bar::name);
}

class InheritanceTest : public ::testing::Test {
protected:
    // void SetUp() override;
    void SetUp() override {
        di.add_instance<Bar>();
        di.add_instance<root_class>();
        di.add_instance<Fita>();
    }

    // void TearDown() override;

    di_container_::di_container di;
};

TEST_F(InheritanceTest, CorrectIntancesAmount) {
    auto root_class_instances = di.get_prob_instances<root_class>();
    auto Fita_instances = di.get_prob_instances<Fita>();
    auto Bar_instances = di.get_prob_instances<Bar>();
    EXPECT_EQ((int)root_class_instances.size(), 3) << "Must be exactly 3 instances for root_class";
    EXPECT_EQ((int)Fita_instances.size(), 2) << "Must be exactly 2 instances for Fita";
    EXPECT_EQ((int)Bar_instances.size(), 1) << "Must be exactly 1 instances for Bar";
}