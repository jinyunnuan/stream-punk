# include "StreamPunk.hpp"

/*
    这是单元测试的一部分 也是一个使用示例
*/

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


// 包含各种指针类型的类
struct PointerDemo : public Base {
#define Xt_PointerDemo(X__) \
    X__(Test*,         rawPtr,       nullptr) \
    X__(Sptr<MQTT>,    sharedPtr,    {}) \
    X__(Uptr<Test>,    uniquePtr,    {}) \
    X__(Wptr<PointerDemo>, weakSelf, {})  // 自引用weak_ptr 

    PointerDemo() = default;
    PointerDemo& operator=(PointerDemo const& v_) = default;
    PointerDemo& operator=(PointerDemo&& v_) = default;
    PointerDemo(PointerDemo const& v_) = default;
    PointerDemo(PointerDemo&& v_) = default;

    UseData(PointerDemo);
};

// 包含容器和指针的类
struct ContainerDemo : public Base {
    /*
        X__(std::map<std::string, Uptr<MQTT>>, mqttConfigs, {}) 会报错
        原因是宏的逗号识别问题.
        解决方法:
            1.使用using 给带逗号的类型起一个别名
            2.使用DH 代替 逗号','
    */
    using mqttUptrMap = std::map<std::string, Uptr<MQTT>>;

#define Xt_ContainerDemo(X__) \
    X__(std::vector<Sptr<Test>>     , testPtrs, {}) \
    X__(Sptr<ContainerDemo>         , selfContainer, {}) \
    X__(std::unordered_set<Base*>   , allObjects, {}) \
    X__(mqttUptrMap                 , mqttConfigs, {}) \
    X__(std::map<std::string DH Uptr<MQTT>> , mqttConfigs2, {}) \
    
    ContainerDemo() = default;
    UseData(ContainerDemo);
};



// 更复杂的嵌套结构
struct NetworkSystem : public Base {
#define Xt_NetworkSystem(X__) \
    X__(Sptr<ContainerDemo>,  mainContainer,   {}) \
    X__(std::vector<Wptr<Test>>, activeTests,   {}) \
    X__(std::list<Uptr<MQTT>>,  mqttInstances,  {}) \
    X__(std::deque<Sptr<PointerDemo>>, demos,  {})

    NetworkSystem() = default;
    UseData(NetworkSystem);
};


// 基础设备类
struct Device : public Base {
#define Xt_Device(X__) \
    X__(std::string, deviceId, "unknown") \
    X__(std::string, manufacturer, "") \
    X__(std::chrono::system_clock::time_point, lastSeen, {})

    Device() = default;
    UseData(Device);
};

// 网络设备 - 继承自Device
struct NetworkDevice : public Device {
#define Xt_NetworkDevice(X__) \
    X__(std::string, ipAddress, "0.0.0.0") \
    X__(std::string, macAddress, "00:00:00:00:00:00") \
    X__(u16, port, 0)

    NetworkDevice() = default;
    UseData(NetworkDevice);
};

// 传感器设备 - 继承自Device
struct Sensor : public Device {
#define Xt_Sensor(X__) \
    X__(f64, currentValue, 0.0) \
    X__(f64, minValue, 0.0) \
    X__(f64, maxValue, 100.0) \
    X__(std::chrono::milliseconds, samplingInterval, std::chrono::milliseconds(1000))

    Sensor() = default;
    UseData(Sensor);

    // 虚函数示例
    virtual f64 getNormalizedValue() const {
        return (currentValue - minValue) / (maxValue - minValue);
    }
};

// 温度传感器 - 继承自Sensor
struct TemperatureSensor : public Sensor {
#define Xt_TemperatureSensor(X__) \
    X__(bool, isCelsius, true) \
    X__(f64, calibrationOffset, 0.0)

    TemperatureSensor() = default;
    UseData(TemperatureSensor);

    // 重写虚函数
    f64 getNormalizedValue() const override {
        f64 value = currentValue;
        if (!isCelsius) {
            // 转换为摄氏温度
            value = (value - 32) * 5 / 9;
        }
        return (value - minValue) / (maxValue - minValue);
    }
};

// 智能设备集合类
struct SmartHomeSystem : public Base {
#define Xt_SmartHomeSystem(X__) \
    X__(std::vector<Sptr<Device>>, allDevices, {}) \
    X__(std::map<std::string DH Sptr<Sensor>>, sensors, {}) \
    X__(Sptr<TemperatureSensor>, mainThermostat, nullptr) \
    X__(Sptr<NetworkSystem>, network, nullptr)

    SmartHomeSystem() = default;
    UseData(SmartHomeSystem);
};

// 复杂的多态容器类
struct MultiLevelContainer : public Base {
#define Xt_MultiLevelContainer(X__) \
    X__(Sptr<Base>, baseObj, nullptr) \
    X__(std::vector<Sptr<Device>>, deviceList, {}) \
    X__(std::map<std::string DH Sptr<Sensor>>, sensorMap, {}) \
    X__(Sptr<MultiLevelContainer>, selfRef, nullptr)

    MultiLevelContainer() = default;
    UseData(MultiLevelContainer);
};