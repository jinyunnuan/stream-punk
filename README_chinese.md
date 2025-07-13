# StreamPunk - 现代C++高性能序列化库

## 为什么选择StreamPunk?

在当今数据驱动的应用中，高效序列化解决方案至关重要。StreamPunk专为现代C++设计，提供：

🚀 **极致性能** - 直接二进制操作，避免文本解析开销  
🧩 **零依赖集成** - 纯头文件实现，轻松融入现有项目  
💡 **现代特性支持** - 完美兼容智能指针、时间类型、原子操作等C++17/20特性  
🌐 **跨平台兼容** - 内置机器特性描述，自动处理字节序和架构差异  
🧠 **深度对象支持** - 原生处理多态、循环引用和复杂嵌套结构  

## 方案对比：StreamPunk vs 主流序列化方案

| 特性               | StreamPunk | JSON       | Protocol Buffers | FlatBuffers | Hessian     |
|--------------------|------------|------------|------------------|-------------|-------------|
| **二进制格式**     | ✅          | ❌          | ✅               | ✅           | ✅           |
| **零拷贝访问**     | ❌          | ❌          | ❌               | ✅           | ❌           |
| **头文件实现**     | ✅          | ❌          | ❌               | ❌           | ❌           |
| **C++20特性支持**  | ✅🌟        | ❌          | ❌               | ❌           | ❌           |
| **智能指针支持**   | ✅🌟        | ❌          | ❌               | ❌           | ❌           |
| **多态序列化**     | ✅🌟        | ❌          | ⚠️ 有限         | ❌           | ✅           |
| **跨平台兼容**     | ✅🌟        | ✅          | ✅               | ✅           | ✅           |
| **安装容易度**     | ⭐⭐⭐⭐⭐     | ⭐⭐⭐⭐       | ⭐⭐⭐            | ⭐⭐          | ⭐⭐⭐         |
| **使用自由度**     | ⭐⭐⭐⭐⭐     | ⭐⭐⭐⭐       | ⭐⭐⭐            | ⭐⭐          | ⭐⭐⭐         |
| **性能估计**       | ⭐⭐⭐⭐       | ⭐⭐         | ⭐⭐⭐⭐           | ⭐⭐⭐⭐⭐      | ⭐⭐⭐         |
| **原生指针支持**   | ✅🌟        | ❌          | ❌               | ❌           | ❌           |
| **跨语言支持**     | 🚧 计划中   | ✅          | ✅               | ✅           | ✅           |

### 关键优势解读：
- **安装容易度**：StreamPunk提供最简安装体验，纯头文件无依赖
- **性能估计**：FlatBuffers零拷贝最优，StreamPunk二进制直接操作紧随其后
- **原生指针支持**：唯一原生支持C++原生指针的解决方案
- **跨语言支持**：StreamPunk计划未来支持JS/TS/Python/Kotlin/Java/Go/Rust等语言

## 快速入门指南

### 1. 包含头文件
```cpp
#include "StreamPunk.hpp"
```

### 2. 注册自定义类型（在customData.hpp）
```cpp
// customData.hpp
#define Xt_CustomType(X__) \
X__(Device, Device) \
X__(Sensor, Sensor) \
X__(SmartHomeSystem, SmartHomeSystem)
// 添加更多业务相关类型...
```

### 3. 创建可序列化类
```cpp
struct Device : public Base {
    #define Xt_Device(X__) \
    X__(std::string, deviceId, "unknown") \
    X__(std::string, manufacturer, "") \
    X__(std::chrono::system_clock::time_point, lastSeen, {})
    
    Device() = default;
    UseData(Device); // 关键宏声明
};
```

### 4. 核心API使用
```cpp
int main() {
    INIT_StreamPunk(); // 必须初始化
    
    // 序列化
    SmartHomeSystem homeSystem;
    std::stringstream binStream;
    O output{binStream};
    output << homeSystem;
    
    // 反序列化
    SmartHomeSystem restored;
    I input{binStream};
    input >> restored;
    
    // 深拷贝
    DeepCopier copier;
    SmartHomeSystem copy;
    deepCopy(copier, copy, homeSystem);
    copier.clear(); // 必须清理
}
```

## 功能全景图

### 支持的数据类型
| 类别             | 支持类型                                                                 |
|------------------|--------------------------------------------------------------------------|
| **基础类型**     | 所有整型/浮点型, bool, 字符类型 (char, wchar_t, char8_t等)               |
| **标准容器**     | vector, list, deque, map, set, unordered_map, unordered_set, bitset      |
| **智能指针**     | shared_ptr, unique_ptr, weak_ptr 原生支持                                |
| **时间类型**     | chrono::duration, chrono::time_point                                    |
| **文件系统**     | filesystem::path 完整支持                                                |
| **高级类型**     | variant, optional, atomic, tuple                                   |
| **特殊类型**     | 仅序列化: initializer_list, span , string_view               |

### 核心特性
- 🔄 **多态序列化** - 自动处理继承体系中的对象序列化
- ♻️ **循环引用** - 智能处理相互引用的复杂对象图
- 📊 **深度拷贝** - 完整对象图复制系统
- 📝 **自描述数据** - 内置类型描述系统
- 🔋 **低内存开销** - 高效内存管理策略
- ⏱️ **时间精度** - 支持atto秒级时间精度

## 使用规范与最佳实践

### 关键约束
1. **编译要求**：
   - C++20或更高标准
   - 必须启用RTTI
   - UTF-8无BOM编码源文件

2. **类设计**：
   ```cpp
   struct MyClass : public Base { // 必须继承Base
       // 使用宏声明序列化字段
       #define Xt_MyClass(X__) \
           X__(int, value, 0) \
           X__(std::string, name, "")
       
       UseData(MyClass); // 关键宏
   };
   ```

3. **指针使用**：
   - 可使用裸指针成员, 仅推荐配合std::unique_ptr时使用.
   - 推荐组合：
     - std::shared_ptr + std::weak_ptr
     - std::unique_ptr + raw ptr
   - 使用void\*时请注意:序列化时,遇到void\*,必须在此之前,其指向的对象就已经被序列化过.

### 跨平台注意事项
```cpp
struct StreamPunkMachineInfo {
    u8 endian; // 字节序：0=小端, 1=大端
    // 其他平台特征...
    
    void init() {
        // 自动检测当前平台特性
        if constexpr (std::endian::native == std::endian::little) {
            endian = 0; // 小端
        }
        else if constexpr (std::endian::native == std::endian::big) {
            endian = 1; // 大端
        }
        // ...
    }
};
```

**重要限制**：当前版本仅支持相同端序架构（大端-大端或小端-小端）的机器间数据互通，不支持跨端序数据交换。

### 性能优化
- 对平凡可拷贝类型使用内存直接操作
- 对大数组使用`std::span`避免复制
- 频繁序列化时复用`O`/`I`对象

## 发展路线
- 🚧 v0.1.3：跨平台跨机器数据查询功能
- 🚀 v0.2.0：数据版本管理
- 🌍 v0.3.x：跨语言支持（JS/TS/Python/Kotlin/Java/Go/Rust）
- 🎨 v0.4.0：数据可视化工具

StreamPunk为现代C++应用提供了强大而灵活的序列化解决方案，特别适合需要处理复杂对象关系和高性能数据交换的场景。通过简洁的API设计和深度系统集成，它显著降低了复杂数据处理的开发成本。