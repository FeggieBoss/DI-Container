#include <gtest/gtest.h>
#include <rttr/registration>
#include <rttr/registration_friend>

#include "di_container.h"

using namespace rttr;
using namespace di_container_;

/*
    Fita.say() = "fita 
               + Fita.Bat.say() 
               + Fita.Bat.Bar.say() 
               + Fita.Bat.Bar.Baz.say
               ";
    Baz { val }
*/

class BazI {
public:
    virtual std::string say() const = 0; 
    RTTR_ENABLE()
};

class Baz : public BazI {
public:
    std::string say() const override {
        return "baz = " + val;
    }

    Baz(std::string val_) : val(val_) {};
private:
    std::string val;

    RTTR_ENABLE(BazI)
    RTTR_REGISTRATION_FRIEND
};

class BarI {
public:
    virtual std::string say() const = 0; 
    RTTR_ENABLE()
};

class Bar : public BarI {
public:
    std::string say() const override {
        return "bar + " + baz->say();
    }
private:
    BazI* baz;

    RTTR_ENABLE(BarI)
    RTTR_REGISTRATION_FRIEND
};

class BatI {
public:
    virtual std::string say() const = 0; 
    RTTR_ENABLE()
};

class Bat : public BatI {
public:
    std::string say() const override {
        return "bat + " + bar->say();
    }
private:
    BarI* bar;

    RTTR_ENABLE(BatI)
    RTTR_REGISTRATION_FRIEND
};

class FitaI {
public:
    virtual std::string say() const = 0; 
    RTTR_ENABLE()
};

class Fita : public FitaI {
public:
    std::string say() const override {
        return "fitochka + " + bat->say();
    }
private:
    BatI* bat;

    RTTR_ENABLE(FitaI)
    RTTR_REGISTRATION_FRIEND
};

RTTR_REGISTRATION
{    
    registration::class_<BazI>("BazI")
        .method("say", &BazI::say);

    registration::class_<Baz>("Baz")
        .constructor<std::string>()
        .property("val", &Baz::val, registration::private_access)
        .method("say", &Baz::say);
    
    registration::class_<BarI>("BarI")
        .method("say", &BarI::say);

    registration::class_<Bar>("Bar")
        .constructor<>()
        .property("baz", &Bar::baz, registration::private_access)
        .method("say", &Baz::say);
    
    registration::class_<BatI>("BatI")
        .method("say", &BatI::say);

    registration::class_<Bat>("Bat")
        .constructor<>()
        .property("bar", &Bat::bar, registration::private_access)
        .method("say", &Bat::say);
    
    registration::class_<FitaI>("FitaI")
        .method("say", &FitaI::say);

    registration::class_<Fita>("Fita")
        .constructor<>()
        .property("bat", &Fita::bat, registration::private_access)
        .method("say", &Fita::say);
}

class InterfaceRegistrationTest : public ::testing::Test {
protected:
    // void SetUp() override;
    void SetUp() override {
        di.register_instance<BazI,Baz>({"<3"}); // transient = default life type
        di.register_instance<BarI,Bar>({});
        di.register_instance<BatI,Bat>({});
        di.register_instance<FitaI,Fita>({});
    }

    // void TearDown() override;

    di_container di;
};

TEST_F(InterfaceRegistrationTest, NoErrors) {
    EXPECT_NO_THROW({ 
        di.get_instance<FitaI>()->say(); 
        di.get_instance<BatI>()->say(); 
        di.get_instance<BarI>()->say(); 
        di.get_instance<BazI>()->say();  
    });
}

TEST_F(InterfaceRegistrationTest, CorrectMethodSay) {
    EXPECT_EQ(di.get_instance<FitaI>()->say(),"fitochka + bat + bar + baz = <3");
    EXPECT_EQ(di.get_instance<BatI>()->say(), "bat + bar + baz = <3");
    EXPECT_EQ(di.get_instance<BarI>()->say(), "bar + baz = <3");
    EXPECT_EQ(di.get_instance<BazI>()->say(), "baz = <3");
}

TEST_F(InterfaceRegistrationTest, ReRegistrationViaInstance) {
    di.register_instance<BazI, Baz>({"cho po zdorov'yu kulak?"}, lifetime_type::Singleton); // any life type will do

    EXPECT_EQ(di.get_instance<FitaI>()->say(),"fitochka + bat + bar + baz = cho po zdorov'yu kulak?");
    EXPECT_EQ(di.get_instance<BatI>()->say(), "bat + bar + baz = cho po zdorov'yu kulak?");
    EXPECT_EQ(di.get_instance<BarI>()->say(), "bar + baz = cho po zdorov'yu kulak?");
    EXPECT_EQ(di.get_instance<BazI>()->say(), "baz = cho po zdorov'yu kulak?");
}

TEST_F(InterfaceRegistrationTest, ReRegistrationViaFieldRegistration) {
    di.register_field<BazI, std::string>("val", "kulak: zdorovo => fadin: zdorovey vidali");

    EXPECT_EQ(di.get_instance<FitaI>()->say(),"fitochka + bat + bar + baz = kulak: zdorovo => fadin: zdorovey vidali");
    EXPECT_EQ(di.get_instance<BatI>()->say(), "bat + bar + baz = kulak: zdorovo => fadin: zdorovey vidali");
    EXPECT_EQ(di.get_instance<BarI>()->say(), "bar + baz = kulak: zdorovo => fadin: zdorovey vidali");
    EXPECT_EQ(di.get_instance<BazI>()->say(), "baz = kulak: zdorovo => fadin: zdorovey vidali");
}