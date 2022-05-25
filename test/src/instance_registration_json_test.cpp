#include <gtest/gtest.h>
#include <rttr/registration>
#include <rttr/registration_friend>

#include "di_container.h"

using namespace rttr;
using namespace di_container_;

#include <map>

/*
    Fita { string, 
           Bat { string }, 
           Bar { int, 
                signed int, 
                unsigned int, 
                bool, 
                double, 
                Baz { string, 
                    vector, 
                    map
                } 
           } 
    }
*/

class InstanceRegistrationJsonBaz {
public:
    std::string getter_string_() const {return string_;}; 
    std::vector<bool> getter_vector_() const {return vector_;}; 
    std::map<bool, bool> getter_map_() const {return map_;}; 

    InstanceRegistrationJsonBaz(){};
    InstanceRegistrationJsonBaz(std::string string__) : string_(string__) {};
    InstanceRegistrationJsonBaz(std::string string__, 
                               std::vector<bool> vector__,
                               std::map<bool, bool> map__) {
        string_ = string__;
        vector_ = vector__;
        map_ = map__;
    }
private:
    std::string string_;
    std::vector<bool> vector_;
    std::map<bool, bool> map_;

    RTTR_ENABLE()
    RTTR_REGISTRATION_FRIEND
};

class InstanceRegistrationJsonBar {
public:
    int int_;
    signed int signed_int_;
    unsigned int unsigned_int_;
    bool bool_;
    double double_;

    std::shared_ptr<InstanceRegistrationJsonBaz> baz;

    InstanceRegistrationJsonBar( int int__, 
                               signed int signed_int__,
                               unsigned int unsigned_int__,
                               bool bool__,
                               double double__,
                               std::shared_ptr<InstanceRegistrationJsonBaz> baz_) {
        int_ = int__;
        signed_int_ = signed_int__;
        unsigned_int_ = unsigned_int__;
        bool_ = bool__;
        double_ = double__;
        baz = baz_;
    };

    RTTR_ENABLE()
};

class InstanceRegistrationJsonBat {
public:
    std::string string_;
    InstanceRegistrationJsonBat(std::string string__) : string_(string__) {};
    RTTR_ENABLE()
};

class InstanceRegistrationJsonFita {
public:
    std::string string_;

    std::shared_ptr<InstanceRegistrationJsonBat> bat;

    std::shared_ptr<InstanceRegistrationJsonBar> bar;

    InstanceRegistrationJsonFita(std::string string__, std::shared_ptr<InstanceRegistrationJsonBat> bat_, std::shared_ptr<InstanceRegistrationJsonBar> bar_) 
        : string_(string__),
          bat(bat_),
          bar(bar_) {};
    RTTR_ENABLE()
};

RTTR_REGISTRATION
{
    registration::class_<InstanceRegistrationJsonBaz>("InstanceRegistrationJsonBaz")
        .constructor<>()
        .constructor<std::string>()
        .constructor<std::string, std::vector<bool>, std::map<bool, bool>>()
        .property("string_", &InstanceRegistrationJsonBaz::string_, registration::private_access)
        .property("vector_", &InstanceRegistrationJsonBaz::vector_, registration::private_access)
        .property("map_", &InstanceRegistrationJsonBaz::map_, registration::private_access);

    registration::class_<InstanceRegistrationJsonBar>("InstanceRegistrationJsonBar")
        .constructor<int, signed int, unsigned int, bool, double, std::shared_ptr<InstanceRegistrationJsonBaz>>()
        .property("int_", &InstanceRegistrationJsonBar::int_)
        .property("signed_int_", &InstanceRegistrationJsonBar::signed_int_)
        .property("unsigned_int_", &InstanceRegistrationJsonBar::unsigned_int_)
        .property("bool_", &InstanceRegistrationJsonBar::bool_)
        .property("double_", &InstanceRegistrationJsonBar::double_)
        .property("baz", &InstanceRegistrationJsonBar::baz);

    registration::class_<InstanceRegistrationJsonBat>("InstanceRegistrationJsonBat")
        .constructor<std::string>()
        .property("string_", &InstanceRegistrationJsonBat::string_);

    registration::class_<InstanceRegistrationJsonFita>("InstanceRegistrationJsonFita")
        .constructor<std::string, std::shared_ptr<InstanceRegistrationJsonBat>, std::shared_ptr<InstanceRegistrationJsonBar>>()
        .property("string_", &InstanceRegistrationJsonFita::string_)
        .property("bat", &InstanceRegistrationJsonFita::bat)
        .property("bar", &InstanceRegistrationJsonFita::bar);
}

class InstanceRegistrationJsonTest : public ::testing::Test {
protected:
    void SetUp() override {
        di.instance_file_registration("../samples/instance_registration_json_test.txt");
    }

    //void TearDown() override {};

    di_container di; 
};

TEST_F(InstanceRegistrationJsonTest, Baz) {
    auto baz = di.get_instance<InstanceRegistrationJsonBaz>();

    EXPECT_EQ(baz->getter_string_(),"Second constructor!");
}

TEST_F(InstanceRegistrationJsonTest, Bar) {
    auto bar = di.get_instance<InstanceRegistrationJsonBar>();

    EXPECT_EQ(bar->int_,54321);
    EXPECT_EQ(bar->signed_int_,-54321);
    EXPECT_EQ(bar->unsigned_int_,54321);
    EXPECT_EQ(bar->bool_,true);
    EXPECT_EQ(bar->double_,54321.54321);
    EXPECT_EQ(bar->baz->getter_string_(),"54321");
    EXPECT_EQ(bar->baz->getter_vector_(),std::vector<bool>({0,1,0,1}));
    EXPECT_EQ(bar->baz->getter_map_()[0], 1);
    EXPECT_EQ(bar->baz->getter_map_()[1], 0);
}

TEST_F(InstanceRegistrationJsonTest, Fita) {
    auto fita = di.get_instance<InstanceRegistrationJsonFita>();

    EXPECT_EQ(fita->string_, "Slovo mobil'naya razrabotka i slovo smert' dlya vas oznachayut odno i to zhe");
    EXPECT_EQ(fita->bat->string_, "da tebe kulak");
    EXPECT_EQ(fita->bar->int_,12345);
    EXPECT_EQ(fita->bar->signed_int_,-12345);
    EXPECT_EQ(fita->bar->unsigned_int_,12345);
    EXPECT_EQ(fita->bar->bool_,true);
    EXPECT_EQ(fita->bar->double_,12345.12345);
    EXPECT_EQ(fita->bar->baz->getter_string_(),"12345");
    EXPECT_EQ(fita->bar->baz->getter_vector_(),std::vector<bool>({1,0,1,0,1}));
    EXPECT_EQ(fita->bar->baz->getter_map_()[0], 1);
    EXPECT_EQ(fita->bar->baz->getter_map_()[1], 1);
}