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

class InheritanceTestFita : public root_class {
public:
    virtual std::string name() const { return "Fita"; };
    RTTR_ENABLE(root_class)
};

class InheritanceTestBar : public InheritanceTestFita {
public:
    std::string name() const override { return "Bar"; };
    RTTR_ENABLE(InheritanceTestFita)
};

RTTR_REGISTRATION
{
    registration::class_<root_class>("root_class")
        .constructor<>();

    registration::class_<InheritanceTestFita>("InheritanceTestFita")
        .constructor<>()
        .method("name", &InheritanceTestFita::name);

    registration::class_<InheritanceTestBar>("InheritanceTestBar")
        .constructor<>()
        .method("name", &InheritanceTestBar::name);
}

class InheritanceTest : public ::testing::Test {
protected:
    // void SetUp() override;
    void SetUp() override {
        di.___add_instance<InheritanceTestBar>();
        di.___add_instance<root_class>();
        di.___add_instance<InheritanceTestFita>();
    }

    // void TearDown() override;

    di_container_::di_container di;
};

TEST_F(InheritanceTest, CorrectIntancesAmount) {
    auto root_class_instances = di.___get_prob_instances<root_class>();
    auto Fita_instances = di.___get_prob_instances<InheritanceTestFita>();
    auto Bar_instances = di.___get_prob_instances<InheritanceTestBar>();
    EXPECT_EQ((int)root_class_instances.size(), 3) << "Must be exactly 3 instances for root_class";
    EXPECT_EQ((int)Fita_instances.size(), 2) << "Must be exactly 2 instances for Fita";
    EXPECT_EQ((int)Bar_instances.size(), 1) << "Must be exactly 1 instances for Bar";
}