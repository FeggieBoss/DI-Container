#include <gtest/gtest.h>
#include <rttr/registration>
#include <rttr/registration_friend>

#include "di_container.h"

using namespace rttr;
using namespace di_container_;

#include <map>

/*
    Bat {public - int, signed int, unsigned int, bool, double, char }
    Baz {private - string, vector, map}
    Fita(singleton) <-- Bat(singleton),Baz(transient)
*/

/*
    char, int*, shared_ptr<int>, pair 
    not supported
*/

class FieldsRegistrationJsonTestBat {
public:
    int int_;
    signed int signed_int_;
    unsigned int unsigned_int_;
    bool bool_;
    double double_;
};

class FieldsRegistrationJsonTestBaz {
public:
    std::string getter_string_() const {return string_;}; 
    std::vector<bool> getter_vector_() const {return vector_;}; 
    std::map<bool, bool> getter_map_() const {return map_;}; 
private:
    std::string string_;
    std::vector<bool> vector_;
    std::map<bool, bool> map_;

    RTTR_REGISTRATION_FRIEND
};

class FieldsRegistrationJsonTestFita {
public:    
    FieldsRegistrationJsonTestBat* FieldsRegistrationJsonTestBat_;
    FieldsRegistrationJsonTestBaz* FieldsRegistrationJsonTestBaz_;
};

RTTR_REGISTRATION
{
    registration::class_<FieldsRegistrationJsonTestBat>("FieldsRegistrationJsonTestBat")
        .constructor<>()
        .property("int_", &FieldsRegistrationJsonTestBat::int_)
        .property("signed_int_", &FieldsRegistrationJsonTestBat::signed_int_)
        .property("unsigned_int_", &FieldsRegistrationJsonTestBat::unsigned_int_)
        .property("bool_", &FieldsRegistrationJsonTestBat::bool_)
        .property("double_", &FieldsRegistrationJsonTestBat::double_);
    
    registration::class_<FieldsRegistrationJsonTestBaz>("FieldsRegistrationJsonTestBaz")
        .constructor<>()
        .property("string_", &FieldsRegistrationJsonTestBaz::string_, registration::private_access)
        .property("vector_", &FieldsRegistrationJsonTestBaz::vector_, registration::private_access)
        .property("map_", &FieldsRegistrationJsonTestBaz::map_, registration::private_access);

    registration::class_<FieldsRegistrationJsonTestFita>("FieldsRegistrationJsonTestFita")
        .constructor<>()
        .property("FieldsRegistrationJsonTestBat_", &FieldsRegistrationJsonTestFita::FieldsRegistrationJsonTestBat_)
        .property("FieldsRegistrationJsonTestBaz_", &FieldsRegistrationJsonTestFita::FieldsRegistrationJsonTestBaz_);
}

class FieldsRegistrationJsonTest : public ::testing::Test {
protected:
    void SetUp() override {
        di.add_singleton<FieldsRegistrationJsonTestFita>();
        di.add_singleton<FieldsRegistrationJsonTestBat>();
        di.add_transient<FieldsRegistrationJsonTestBaz>();

        di.file_registration("input.txt");
    }

    //void TearDown() override {};

    di_container di; 
};

TEST_F(FieldsRegistrationJsonTest, Int) {
    EXPECT_EQ(di.get_instance<FieldsRegistrationJsonTestBat>()->int_, 12345);
}

TEST_F(FieldsRegistrationJsonTest, SignedInt) {
    EXPECT_EQ(di.get_instance<FieldsRegistrationJsonTestBat>()->signed_int_, -12345);
}

TEST_F(FieldsRegistrationJsonTest, UnsignedInt) {
    EXPECT_EQ(di.get_instance<FieldsRegistrationJsonTestBat>()->unsigned_int_, 12345);
}

TEST_F(FieldsRegistrationJsonTest, Bool) {
    EXPECT_EQ(di.get_instance<FieldsRegistrationJsonTestBat>()->bool_, true);
}

TEST_F(FieldsRegistrationJsonTest, Double) {
    EXPECT_EQ(di.get_instance<FieldsRegistrationJsonTestBat>()->double_, 12345.12345);
}


TEST_F(FieldsRegistrationJsonTest, String) {
    EXPECT_EQ(di.get_instance<FieldsRegistrationJsonTestBaz>()->getter_string_(), "12345");
}

TEST_F(FieldsRegistrationJsonTest, Vector) {
    EXPECT_EQ(di.get_instance<FieldsRegistrationJsonTestBaz>()->getter_vector_(), std::vector<bool>({1,0,1,0,1}));
}

TEST_F(FieldsRegistrationJsonTest, Map) {
    auto val2 = di.get_instance<FieldsRegistrationJsonTestBaz>()->getter_map_();
    EXPECT_EQ(val2[0], 1);
    EXPECT_EQ(val2[1], 1);
}

TEST_F(FieldsRegistrationJsonTest, RegistrationInFields) {
    auto fita = di.get_instance<FieldsRegistrationJsonTestFita>();
    auto bat = fita->FieldsRegistrationJsonTestBat_;
    auto baz = fita->FieldsRegistrationJsonTestBaz_;

    EXPECT_EQ(bat->int_, 12345);
    EXPECT_EQ(bat->signed_int_, -12345);
    EXPECT_EQ(bat->unsigned_int_, 12345);
    EXPECT_EQ(bat->bool_, true);
    EXPECT_EQ(bat->double_, 12345.12345);
    EXPECT_EQ(baz->getter_string_(), "12345");
    EXPECT_EQ(baz->getter_vector_(), std::vector<bool>({1,0,1,0,1}));
    EXPECT_EQ(baz->getter_map_()[0], 1);
    EXPECT_EQ(baz->getter_map_()[1], 1);
}