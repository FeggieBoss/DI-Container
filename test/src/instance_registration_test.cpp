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

class InstanceRegistrationTestBat {
public:
    int int_;
    signed int signed_int_;
    unsigned int unsigned_int_;
    bool bool_;
    double double_;
    char char_;
    InstanceRegistrationTestBat( int int__, 
                               signed int signed_int__,
                               unsigned int unsigned_int__,
                               bool bool__,
                               double double__,
                               char char__) {
        int_ = int__;
        signed_int_ = signed_int__;
        unsigned_int_ = unsigned_int__;
        bool_ = bool__;
        double_ = double__;
        char_ = char__;
    };

    RTTR_ENABLE()
};

class InstanceRegistrationTestBar {
public:
    int* getter_int_ptr() const {return int_ptr;};
    std::shared_ptr<int> getter_int_shared_ptr() const {return int_shared_ptr;}; 

    InstanceRegistrationTestBar(int* int_ptr_, std::shared_ptr<int> int_shared_ptr_) {
        int_ptr = int_ptr_;
        int_shared_ptr = int_shared_ptr_;
    }
protected:
    int* int_ptr;
    std::shared_ptr<int> int_shared_ptr;

    RTTR_ENABLE()
    RTTR_REGISTRATION_FRIEND
};

class InstanceRegistrationTestBaz {
public:
    std::string getter_string_() const {return string_;}; 
    std::pair<bool, bool> getter_pair_() const {return pair_;}; 
    std::vector<bool> getter_vector_() const {return vector_;}; 
    std::map<bool, bool> getter_map_() const {return map_;}; 

    InstanceRegistrationTestBaz( std::string string__, 
                               std::pair<bool, bool> pair__,
                               std::vector<bool> vector__,
                               std::map<bool, bool> map__) {
        string_ = string__;
        pair_ = pair__;
        vector_ = vector__;
        map_ = map__;
    }
private:
    std::string string_;
    std::pair<bool, bool> pair_;
    std::vector<bool> vector_;
    std::map<bool, bool> map_;

    RTTR_ENABLE()
    RTTR_REGISTRATION_FRIEND
};

class InstanceRegistrationTestFita {
public:    
    InstanceRegistrationTestBat* InstanceRegistrationTestBat_;
    InstanceRegistrationTestBar* InstanceRegistrationTestBar_;
    InstanceRegistrationTestBaz* InstanceRegistrationTestBaz_;
    
    RTTR_ENABLE()
};

RTTR_REGISTRATION
{
    registration::class_<InstanceRegistrationTestBat>("InstanceRegistrationTestBat")
        .constructor<int, signed int, unsigned int, bool, double, char>()
        .property("int_", &InstanceRegistrationTestBat::int_)
        .property("signed_int_", &InstanceRegistrationTestBat::signed_int_)
        .property("unsigned_int_", &InstanceRegistrationTestBat::unsigned_int_)
        .property("bool_", &InstanceRegistrationTestBat::bool_)
        .property("double_", &InstanceRegistrationTestBat::double_)
        .property("char_", &InstanceRegistrationTestBat::char_);
    
    registration::class_<InstanceRegistrationTestBar>("InstanceRegistrationTestBar")
        .constructor<int*, std::shared_ptr<int>>()
        .property("int_ptr", &InstanceRegistrationTestBar::int_ptr, registration::protected_access)
        .property("int_shared_ptr", &InstanceRegistrationTestBar::int_shared_ptr, registration::protected_access);

    registration::class_<InstanceRegistrationTestBaz>("InstanceRegistrationTestBaz")
        .constructor<std::string, std::pair<bool, bool>, std::vector<bool>, std::map<bool, bool>>()
        .property("string_", &InstanceRegistrationTestBaz::string_, registration::private_access)
        .property("pair_", &InstanceRegistrationTestBaz::pair_, registration::private_access)
        .property("vector_", &InstanceRegistrationTestBaz::vector_, registration::private_access)
        .property("map_", &InstanceRegistrationTestBaz::map_, registration::private_access);

    registration::class_<InstanceRegistrationTestFita>("InstanceRegistrationTestFita")
        .constructor<>()
        .property("InstanceRegistrationTestBat_", &InstanceRegistrationTestFita::InstanceRegistrationTestBat_)
        .property("InstanceRegistrationTestBar_", &InstanceRegistrationTestFita::InstanceRegistrationTestBar_)
        .property("InstanceRegistrationTestBaz_", &InstanceRegistrationTestFita::InstanceRegistrationTestBaz_);
}

class InstanceRegistrationTest : public ::testing::Test {
protected:
    int* di_int_ptr;
    std::shared_ptr<int> di_int_shared_ptr;
    std::map<bool,bool> di_map;

    // void SetUp() override;
    void SetUp() override {
        di.register_instance_singleton_simple<InstanceRegistrationTestFita>();

        di.register_instance_singleton_simple<InstanceRegistrationTestBat>({
            12345,
            -12345,
            (unsigned int)12345,
            true,
            12345.12345,
            's'
        });
        
        di_int_ptr = new int[1];
        di_int_shared_ptr = std::make_shared<int>(12345);
        di.register_instance_scoped_simple<InstanceRegistrationTestBar>({
            (int*)di_int_ptr,
            (std::shared_ptr<int>)di_int_shared_ptr
        });

        di_map[1]=1,di_map[0]=1;
        di.register_instance_transient_simple<InstanceRegistrationTestBaz>({
            "12345",
            std::make_pair(true,true),
            std::vector<bool>({1,0,1,0,1}),
            di_map
        });
    }

    // void TearDown() override;
    void TearDown() override {
        delete[] di_int_ptr;
    }

    di_container di; 
};

TEST_F(InstanceRegistrationTest, Int) {
    EXPECT_EQ(di.get_instance<InstanceRegistrationTestBat>()->int_, 12345);
}

TEST_F(InstanceRegistrationTest, SignedInt) {
    EXPECT_EQ(di.get_instance<InstanceRegistrationTestBat>()->signed_int_, -12345);
}

TEST_F(InstanceRegistrationTest, UnsignedInt) {
    EXPECT_EQ(di.get_instance<InstanceRegistrationTestBat>()->unsigned_int_, 12345);
}

TEST_F(InstanceRegistrationTest, Bool) {
    EXPECT_EQ(di.get_instance<InstanceRegistrationTestBat>()->bool_, true);
}

TEST_F(InstanceRegistrationTest, Double) {
    EXPECT_EQ(di.get_instance<InstanceRegistrationTestBat>()->double_, 12345.12345);
}

TEST_F(InstanceRegistrationTest, Char) {
    EXPECT_EQ(di.get_instance<InstanceRegistrationTestBat>()->char_, 's');
}

TEST_F(InstanceRegistrationTest, Ptr) {
    EXPECT_EQ(di.get_instance<InstanceRegistrationTestBar>()->getter_int_ptr(), di_int_ptr);
}

TEST_F(InstanceRegistrationTest, SharedPtr) {
    EXPECT_EQ(di.get_instance<InstanceRegistrationTestBar>()->getter_int_shared_ptr(), di_int_shared_ptr);
}

TEST_F(InstanceRegistrationTest, String) {
    EXPECT_EQ(di.get_instance<InstanceRegistrationTestBaz>()->getter_string_(), "12345");
}

TEST_F(InstanceRegistrationTest, Pair) {
    EXPECT_EQ(di.get_instance<InstanceRegistrationTestBaz>()->getter_pair_(), std::make_pair(true,true));
}

TEST_F(InstanceRegistrationTest, Vector) {
    EXPECT_EQ(di.get_instance<InstanceRegistrationTestBaz>()->getter_vector_(), std::vector<bool>({1,0,1,0,1}));
}

TEST_F(InstanceRegistrationTest, Map) {
    auto val2 = di.get_instance<InstanceRegistrationTestBaz>()->getter_map_();
    EXPECT_EQ(val2[0], 1);
    EXPECT_EQ(val2[1], 1);
}

TEST_F(InstanceRegistrationTest, RegistrationInFields) {
    auto fita = di.get_instance<InstanceRegistrationTestFita>();
    auto bat = fita->InstanceRegistrationTestBat_;
    auto bar = fita->InstanceRegistrationTestBar_;
    auto baz = fita->InstanceRegistrationTestBaz_;

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