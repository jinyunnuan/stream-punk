# pragma once
# include "StreamPunk.hpp"
# include <boost/predef.h>

constexpr inline u32 currDataVer = makeVersion(0, 0, 1, 0);

// 机器特性描述
#pragma pack(push, 1)  // 1字节对齐
struct StreamPunkMachineInfo {
    enum E_os {
        e_os_unknow = 0,
        e_os_win    ,
        e_os_mac    ,
        e_os_linux  ,
        e_os_ios    ,
        e_os_android,
        e_os_bsd    ,
    };

    enum E_cpu {
        e_cpu_unknow = 0,
        e_cpu_x86   ,
        e_cpu_X86_64,
        e_cpu_arm   ,
        e_cpu_arm64 ,
        e_cpu_ppc   ,
    };

    enum E_compiler {
        e_compiler_unknow = 0,
        e_compiler_msvc ,
        e_compiler_gcc  ,
        e_compiler_clang,
        e_compiler_intel,
    };

    u8 endian:1         ;   // 字节序：0=小端, 1=大端
    u8 os:7             ;   // 操作系统 (E_os)
    u8 cpu              ;   // CPU架构类型 (E_cpu)
    u8 wordByteCount    ;   // 程序字长的字节数
    u8 compiler         ;   // 编译器类型 (E_compiler)
    
    u32 compilerVer     ;   // 编译器版本
    u32 dataVer         ;   // 数据版本

    void init() {
        std::memset(this, 0, sizeof(StreamPunkMachineInfo));

        if constexpr (std::endian::native == std::endian::little) {
            endian = 0;
        }
        else if constexpr (std::endian::native == std::endian::big) {
            endian = 1;
        }

        wordByteCount = sizeof(void*);

        if      constexpr (BOOST_OS_WINDOWS ) os = e_os_win;
        else if constexpr (BOOST_OS_MACOS   ) os = e_os_mac;
        else if constexpr (BOOST_OS_LINUX   ) os = e_os_linux;
        else if constexpr (BOOST_OS_IOS     ) os = e_os_ios;
# if defined(BOOST_OS_ANDROID)
        else if constexpr (BOOST_OS_ANDROID ) os = e_os_android;
# endif
        else if constexpr (BOOST_OS_BSD     ) os = e_os_bsd;
        else os = e_os_unknow;

        if      constexpr (BOOST_ARCH_X86_64) cpu = e_cpu_X86_64;
        else if constexpr (BOOST_ARCH_X86   ) cpu = e_cpu_x86;
# if defined(BOOST_ARCH_ARM64)
        else if constexpr (BOOST_ARCH_ARM64 ) cpu = e_cpu_arm64;
# endif
        else if constexpr (BOOST_ARCH_ARM   ) cpu = e_cpu_arm;
        else if constexpr (BOOST_ARCH_PPC   ) cpu = e_cpu_ppc;
        else cpu = e_cpu_unknow;

        uint32_t compVerRaw = 0;
        if      constexpr (BOOST_COMP_MSVC  ) {
            compiler = e_compiler_msvc;
            compVerRaw = BOOST_COMP_MSVC;
        }
        else if constexpr (BOOST_COMP_CLANG ) {
            compiler = e_compiler_clang;
            compVerRaw = BOOST_COMP_CLANG;
        }
        else if constexpr (BOOST_COMP_GNUC  ) {
            compiler = e_compiler_gcc;
            compVerRaw = BOOST_COMP_GNUC;
        }
        else if constexpr (BOOST_COMP_INTEL ) {
            compiler = e_compiler_intel;
            compVerRaw = BOOST_COMP_INTEL;
        }
        else {
            compiler = e_compiler_unknow;
        }
        if (compVerRaw != 0) {
            uint32_t major = compVerRaw / 10000000;
            uint32_t minor = (compVerRaw % 10000000) / 100000;
            uint32_t patch = (compVerRaw % 100000) / 1000;
            compilerVer = (major << 24) | (minor << 16) | (patch << 8);
        }
        dataVer = currDataVer;
    }
};
#pragma pack(pop)
inline O& operator<<(O& s, const StreamPunkMachineInfo& v) { s.s.write(reinterpret_cast<char const*>(&v), sizeof(v)); return s; }
inline I& operator>>(I& s, StreamPunkMachineInfo& v) { s.s.read(reinterpret_cast<char*>(&v), sizeof(v)); return s; }


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
    UseDataBase(NetworkDevice,Device);
};

// 传感器设备 - 继承自Device
struct Sensor : public Device {
#define Xt_Sensor(X__) \
    X__(f64, currentValue, 0.0) \
    X__(f64, minValue, 0.0) \
    X__(f64, maxValue, 100.0) \
    X__(std::chrono::milliseconds, samplingInterval, std::chrono::milliseconds(1000))

    Sensor() = default;
    UseDataBase(Sensor,Device);

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
    UseDataBase(TemperatureSensor, Sensor);

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

