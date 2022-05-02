#include <gtest/gtest.h>
#include <rttr/registration>
using namespace rttr;

#include "di_container.h"

#include "ServiceA.h"
#include "ServiceB.h"

/*
A <---> B
- A::b is instance of B
- B::a is instance of A
- B::say() using A::say() inside
*/

RTTR_REGISTRATION
{
    registration::class_<ServiceA>("ServiceA")
        .constructor<>()
        .property("id", &ServiceA::id)
        .property("b", &ServiceA::b)
        .method("say", &ServiceA::say);

    registration::class_<ServiceB>("ServiceB")
        .constructor<>()
        .property("id", &ServiceB::id)
        .property("a", &ServiceB::a)
        .method("say", &ServiceB::say);
}

class CyclicDepenciesTest : public ::testing::Test {
protected:
    // void SetUp() override;
    void SetUp() override {
        di.add_singleton<ServiceA>();
        di.add_singleton<ServiceB>();
    }   

    // void TearDown() override;

    di_container_::di_container di;
};

TEST_F(CyclicDepenciesTest, ReferToEachOther) {
    auto A_instance = di.get_instance<ServiceA>();
    auto B_instance = di.get_instance<ServiceB>();
    EXPECT_EQ(A_instance.get(), B_instance->a) << "B refers to another object of class ServiceA";
    EXPECT_EQ(B_instance.get(), A_instance->b) << "A refers to another object of class ServiceB";
    EXPECT_EQ(A_instance->id, B_instance->a->id) << "B refers to another object of class ServiceA";
    EXPECT_EQ(B_instance->id, A_instance->b->id) << "A refers to another object of class ServiceB";
}

TEST_F(CyclicDepenciesTest, NoSegmentFault) {
    auto A_instance = di.get_instance<ServiceA>();
    auto B_instance = di.get_instance<ServiceB>();
    EXPECT_NO_THROW({A_instance->say();}) << "Error in A::say()";
    EXPECT_NO_THROW({B_instance->say();}) << "Error in B::say()";
}

TEST_F(CyclicDepenciesTest, CorrectInstancesMethods) {
    auto A_instance = di.get_instance<ServiceA>();
    auto B_instance = di.get_instance<ServiceB>();
    EXPECT_EQ(A_instance->say(), "I am A, and my son say: da") << "Wrong method A::say()";
    EXPECT_EQ(B_instance->say(), "I am B, and my son say: I am A, and my son say: da") << "Wrong method B::say()";
}

TEST(CyclicDepenciesTestMixed, AllInOnce) {
    di_container_::di_container di;
    di.add_scoped<ServiceA>();
    di.add_singleton<ServiceB>();

    auto A_instance = di.get_instance<ServiceA>();
    auto B_instance = di.get_instance<ServiceB>();

    EXPECT_EQ(A_instance.get(), B_instance->a) << "B refers to another object of class ServiceA";
    EXPECT_EQ(B_instance.get(), A_instance->b) << "A refers to another object of class ServiceB";
    
    EXPECT_NO_THROW({A_instance->say();}) << "Error in A::say()";
    EXPECT_NO_THROW({B_instance->say();}) << "Error in B::say()";

    EXPECT_EQ(A_instance->say(), "I am A, and my son say: da") << "Wrong method A::say()";
    EXPECT_EQ(B_instance->say(), "I am B, and my son say: I am A, and my son say: da") << "Wrong method B::say()";
}