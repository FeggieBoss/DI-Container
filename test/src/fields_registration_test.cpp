#include <gtest/gtest.h>
#include <rttr/registration>
#include <rttr/registration_friend>

#include "di_container.h"

using namespace rttr;
using namespace di_container_;

#include <map>

/*
    Bat {public - int, signed int, unsigned int, bool, double, char }
    Bar {protected - int*, shared_ptr<int>}
    Baz {private - string, pair, vector, map}
    Fita(singleton) <-- Bat(singleton),Bar(scoped),Baz(transient)
*/

class FieldsRegistrationTestBat : public root_class {
public:
    int int_;
    signed int signed_int_;
    unsigned int unsigned_int_;
    bool bool_;
    double double_;
    char char_;
    
    RTTR_ENABLE(root_class)
};

class FieldsRegistrationTestBar : public root_class {
public:
    int* getter_int_ptr() const {return int_ptr;};
    std::shared_ptr<int> getter_int_shared_ptr() const {return int_shared_ptr;}; 
protected:
    int* int_ptr;
    std::shared_ptr<int> int_shared_ptr;

    RTTR_ENABLE(root_class)
    RTTR_REGISTRATION_FRIEND
};

class FieldsRegistrationTestBaz : public root_class {
public:
    std::string getter_string_() const {return string_;}; 
    std::pair<bool, bool> getter_pair_() const {return pair_;}; 
    std::vector<bool> getter_vector_() const {return vector_;}; 
    std::map<bool, bool> getter_map_() const {return map_;}; 
private:
    std::string string_;
    std::pair<bool, bool> pair_;
    std::vector<bool> vector_;
    std::map<bool, bool> map_;

    RTTR_ENABLE(root_class)
    RTTR_REGISTRATION_FRIEND
};

class FieldsRegistrationTestFita : public root_class {
public:    
    FieldsRegistrationTestBat* FieldsRegistrationTestBat_;
    FieldsRegistrationTestBar* FieldsRegistrationTestBar_;
    FieldsRegistrationTestBaz* FieldsRegistrationTestBaz_;

    RTTR_ENABLE(root_class)
};

RTTR_REGISTRATION
{
    registration::class_<root_class>("root_class")
        .constructor<>();
    
    registration::class_<FieldsRegistrationTestBat>("FieldsRegistrationTestBat")
        .constructor<>()
        .property("int_", &FieldsRegistrationTestBat::int_)
        .property("signed_int_", &FieldsRegistrationTestBat::signed_int_)
        .property("unsigned_int_", &FieldsRegistrationTestBat::unsigned_int_)
        .property("bool_", &FieldsRegistrationTestBat::bool_)
        .property("double_", &FieldsRegistrationTestBat::double_)
        .property("char_", &FieldsRegistrationTestBat::char_);
    
    registration::class_<FieldsRegistrationTestBar>("FieldsRegistrationTestBar")
        .constructor<>()
        .property("int_ptr", &FieldsRegistrationTestBar::int_ptr, registration::protected_access)
        .property("int_shared_ptr", &FieldsRegistrationTestBar::int_shared_ptr, registration::protected_access);

    registration::class_<FieldsRegistrationTestBaz>("FieldsRegistrationTestBaz")
        .constructor<>()
        .property("string_", &FieldsRegistrationTestBaz::string_, registration::private_access)
        .property("pair_", &FieldsRegistrationTestBaz::pair_, registration::private_access)
        .property("vector_", &FieldsRegistrationTestBaz::vector_, registration::private_access)
        .property("map_", &FieldsRegistrationTestBaz::map_, registration::private_access);

    registration::class_<FieldsRegistrationTestFita>("FieldsRegistrationTestFita")
        .constructor<>()
        .property("FieldsRegistrationTestBat_", &FieldsRegistrationTestFita::FieldsRegistrationTestBat_)
        .property("FieldsRegistrationTestBar_", &FieldsRegistrationTestFita::FieldsRegistrationTestBar_)
        .property("FieldsRegistrationTestBaz_", &FieldsRegistrationTestFita::FieldsRegistrationTestBaz_);
}

class FieldsRegistrationTest : public ::testing::Test {
protected:
    int* di_int_ptr;
    std::shared_ptr<int> di_int_shared_ptr;
    std::map<bool,bool> di_map;

    // void SetUp() override;
    void SetUp() override {
        di.add_singleton<FieldsRegistrationTestFita>();
        di.add_singleton<FieldsRegistrationTestBat>();
        di.add_scoped<FieldsRegistrationTestBar>();
        di.add_transient<FieldsRegistrationTestBaz>();

        di_int_ptr = new int[1];
        di_int_shared_ptr = std::make_shared<int>(12345);

        di.register_field<FieldsRegistrationTestBat>("int_", 12345);
        di.register_field<FieldsRegistrationTestBat>("signed_int_", -12345);
        di.register_field<FieldsRegistrationTestBat>("unsigned_int_", (unsigned int)12345);
        di.register_field<FieldsRegistrationTestBat>("bool_", true);
        di.register_field<FieldsRegistrationTestBat>("double_", 12345.12345);
        di.register_field<FieldsRegistrationTestBat>("char_", 's');
        di.register_field<FieldsRegistrationTestBar>("int_ptr", di_int_ptr);
        di.register_field<FieldsRegistrationTestBar>("int_shared_ptr", di_int_shared_ptr);
        di.register_field<FieldsRegistrationTestBaz>("string_", "12345");
        di.register_field<FieldsRegistrationTestBaz>("pair_", std::make_pair(true,true));
        di.register_field<FieldsRegistrationTestBaz>("vector_", std::vector<bool>({1,0,1,0,1}));
        
        di_map[1]=1,di_map[0]=1;
        di.register_field<FieldsRegistrationTestBaz>("map_", di_map);
    }

    void TearDown() override {
        delete di_int_ptr;
    }

    di_container di; 
};

TEST_F(FieldsRegistrationTest, Int) {
    EXPECT_EQ(di.get_instance<FieldsRegistrationTestBat>()->int_, 12345);
}

TEST_F(FieldsRegistrationTest, SignedInt) {
    EXPECT_EQ(di.get_instance<FieldsRegistrationTestBat>()->signed_int_, -12345);
}

TEST_F(FieldsRegistrationTest, UnsignedInt) {
    EXPECT_EQ(di.get_instance<FieldsRegistrationTestBat>()->unsigned_int_, 12345);
}

TEST_F(FieldsRegistrationTest, Bool) {
    EXPECT_EQ(di.get_instance<FieldsRegistrationTestBat>()->bool_, true);
}

TEST_F(FieldsRegistrationTest, Double) {
    EXPECT_EQ(di.get_instance<FieldsRegistrationTestBat>()->double_, 12345.12345);
}

TEST_F(FieldsRegistrationTest, Char) {
    EXPECT_EQ(di.get_instance<FieldsRegistrationTestBat>()->char_, 's');
}

TEST_F(FieldsRegistrationTest, Ptr) {
    EXPECT_EQ(di.get_instance<FieldsRegistrationTestBar>()->getter_int_ptr(), di_int_ptr);
}

TEST_F(FieldsRegistrationTest, SharedPtr) {
    EXPECT_EQ(di.get_instance<FieldsRegistrationTestBar>()->getter_int_shared_ptr(), di_int_shared_ptr);
}

TEST_F(FieldsRegistrationTest, String) {
    EXPECT_EQ(di.get_instance<FieldsRegistrationTestBaz>()->getter_string_(), "12345");
}

TEST_F(FieldsRegistrationTest, Pair) {
    EXPECT_EQ(di.get_instance<FieldsRegistrationTestBaz>()->getter_pair_(), std::make_pair(true,true));
}

TEST_F(FieldsRegistrationTest, Vector) {
    EXPECT_EQ(di.get_instance<FieldsRegistrationTestBaz>()->getter_vector_(), std::vector<bool>({1,0,1,0,1}));
}

TEST_F(FieldsRegistrationTest, Map) {
    auto val2 = di.get_instance<FieldsRegistrationTestBaz>()->getter_map_();
    EXPECT_EQ(val2[0], 1);
    EXPECT_EQ(val2[1], 1);
}

TEST_F(FieldsRegistrationTest, RegistrationInFields) {
    auto fita = di.get_instance<FieldsRegistrationTestFita>();
    auto bat = fita->FieldsRegistrationTestBat_;
    auto bar = fita->FieldsRegistrationTestBar_;
    auto baz = fita->FieldsRegistrationTestBaz_;

    EXPECT_EQ(bat->int_, 12345);
    EXPECT_EQ(bat->signed_int_, -12345);
    EXPECT_EQ(bat->unsigned_int_, 12345);
    EXPECT_EQ(bat->bool_, true);
    EXPECT_EQ(bat->double_, 12345.12345);
    EXPECT_EQ(bat->char_, 's');
    EXPECT_EQ(bar->getter_int_ptr(), di_int_ptr);
    EXPECT_EQ(bar->getter_int_shared_ptr(), di_int_shared_ptr);
    EXPECT_EQ(baz->getter_string_(), "12345");
    EXPECT_EQ(baz->getter_pair_(), std::make_pair(true,true));
    EXPECT_EQ(baz->getter_vector_(), std::vector<bool>({1,0,1,0,1}));
    EXPECT_EQ(baz->getter_map_()[0], 1);
    EXPECT_EQ(baz->getter_map_()[1], 1);
}