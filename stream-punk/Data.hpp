# include "StreamPunk.hpp"

// 注意, 禁止关于Base的菱形继承!
struct Test :public Base {

# define Xt_Test(X__) \
X__(std::string, name     ,{}) \
X__(std::string, pwd      ,{}) \
X__(std::string, gateWay  ,{}) \
X__(std::string, mask     ,{}) \
X__(std::string, ip       ,{}) \
X__(std::string, dns1     ,{}) \
X__(std::string, dns2     ,{}) \

    Test() = default;
    UseData(Test);
};

struct MQTT :public Base {
# define Xt_MQTT(X__) \
X__(std::string, host, {}) \
X__(std::string, user, {}) \
X__(std::string, pwd , {}) \

    MQTT() = default;
    UseData(MQTT);
}; // struct MQTT



