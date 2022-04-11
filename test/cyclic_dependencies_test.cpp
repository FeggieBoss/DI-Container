#include <gtest/gtest.h>
#include <rttr/registration>
using namespace rttr;

#include "di_container.h"

#include "ServiceA.h"
#include "ServiceB.h"

/*
A <-> B
- A::b is instance of B
- B::a is instance of A
- B::say() using A::say() inside
*/

RTTR_REGISTRATION
{
    registration::class_<ServiceA>("ServiceA")
        .constructor<>()
        .property("b", &ServiceA::b)
        .method("say", &ServiceA::say);

    registration::class_<ServiceB>("ServiceB")
        .constructor<>()
        .property("a", &ServiceB::a)
        .method("say", &ServiceB::say);
}

class CyclicDepenciesTest : public ::testing::Test {
protected:
    // void SetUp() override;
    void SetUp() override {
        di.add_instance<ServiceA>();
        di.add_instance<ServiceB>();
    }

    // void TearDown() override;

    di_container_::di_container di;
};

TEST_F(CyclicDepenciesTest, ProbInstancesNotEmpty) {
    auto A_instances = di.get_prob_instances<ServiceA>();
    auto B_instances = di.get_prob_instances<ServiceB>();
    EXPECT_GT((int)A_instances.size(), 0) << "No instance found for A";
    EXPECT_GT((int)B_instances.size(), 0) << "No instance found for B";
}

TEST_F(CyclicDepenciesTest, NoSegmentFault) {
    auto A_instances = di.get_prob_instances<ServiceA>();
    auto B_instances = di.get_prob_instances<ServiceB>();
    EXPECT_NO_THROW({A_instances.back()->say();}) << "Error in A::say()";
    EXPECT_NO_THROW({B_instances.back()->say();}) << "Error in B::say()";
}

TEST_F(CyclicDepenciesTest, CorrectInstancesMethods) {
    auto A_instances = di.get_prob_instances<ServiceA>();
    auto B_instances = di.get_prob_instances<ServiceB>();
    EXPECT_EQ(A_instances.back()->say(), "I am A, and my son say: da") << "Wrong method A::say()";
    EXPECT_EQ(B_instances.back()->say(), "I am B, and my son say: I am A, and my son say: da") << "Wrong method B::say()";
}