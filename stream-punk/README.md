# StreamPunk 库 (v0.3.1)

StreamPunk (中文名：流水账) 是一个用于C++对象序列化和反序列化的轻量级库，提供了对多种数据类型的基本支持和智能指针处理功能。

## 核心特性

1. **多平台支持**：跨平台设计
2. **高性能序列化**：对POD类型使用直接内存拷贝优化
3. **完整类型支持**：
   - 基本数据类型
   - STL容器 (vector, list, deque, map, set等)
   - 智能指针 (shared_ptr, weak_ptr, unique_ptr)
   - 自定义数据结构

## 支持的数据类型

| 类别            | 支持的类型                                                                 |
|----------------|---------------------------------------------------------------------------|
| **基本类型**    | u8, u16, u32, u64, i8, i16, i32, i64, f32, f64, ch, bl                    |
| **连续容器**     | vector, array, basic_string, basic_string_view                            |
| **链表容器**     | deque, list, forward_list                                                 |
| **关联容器**     | map, unordered_map, set, unordered_set                                    |
| **其他容器**     | bitset                                                                  |
| **智能指针**     | shared_ptr, weak_ptr, unique_ptr                                        |
| **时间类型**     | chrono (通过customData.hpp扩展)                                        |

## 集成方法

1. 包含头文件：
   ```cpp
   #include "StreamPunk.hpp"
   ```

2. 在项目中添加自定义数据类型定义：
   ```cpp
   #include "customData.hpp"
   ```

## 基本用法

### 序列化对象
```cpp
#include "StreamPunk.hpp"
#include <fstream>

struct MyData {
    int id;
    std::vector<float> values;
    
    // 使用宏定义序列化接口
    UseData(MyData)
};

int main() {
    MyData data{42, {1.1f, 2.2f, 3.3f}};
    std::ofstream file("data.bin", std::ios::binary);
    
    O o{file}; // 创建输出上下文
    o << data; // 序列化对象
    
    return 0;
}
```

### 反序列化对象
```cpp
#include "StreamPunk.hpp"
#include <fstream>

int main() {
    MyData data;
    std::ifstream file("data.bin", std::ios::binary);
    
    I i{file}; // 创建输入上下文
    i >> data; // 反序列化对象
    
    std::cout << "ID: " << data.id << "\n";
    for (auto v : data.values) {
        std::cout << v << " ";
    }
    
    return 0;
}
```

## 自定义数据类型支持

所有自定义类型必须继承自`Base`基类：

```cpp
#include "StreamPunk.hpp"

struct CustomType : public Base {
    std::string name;
    int value;
    
    // 使用宏简化类型定义
    UseData(CustomType)
};

// 在customData.hpp中注册
#define Xt_CustomType(X__) X__(CustomType, ct)
```

## 指针处理机制

1. **智能指针支持**：
   ```cpp
   auto ptr = std::make_shared<MyObject>();
   o << ptr;
   
   std::shared_ptr<MyObject> newPtr;
   i >> newPtr;
   ```

2. **循环引用处理**：
   - 输出上下文维护指针地址集合
   - 输入上下文维护指针地址映射表

3. **多态限制**：
   ```cpp
   Base* base = new Derived(); // ❌ 序列化为Base类型
   ```
   解决方案：
   - 所有可序列化类型需继承自`Base`
   - 通过虚函数实现正确类型分发

## 性能优化

1. POD类型的内存直接复制
2. 避免大型对象的重复序列化
3. 最小化上下文管理开销

## 版本历史

- v0.0.1：基础类型支持
- v0.0.2：STL容器支持
- v0.0.3：原始指针和智能指针支持
- v0.0.4：指针处理优化和问题修复

## 注意事项

1. 所有可序列化的自定义类型必须直接或间接继承`Base`基类
2. 跨平台使用时注意：StreamPunk目前只支持相同端序的机器之间进行数据传递

## 应用场景

- 网络数据传输
- 持久化存储
- 进程间通信
- 游戏存档系统
- 分布式计算

StreamPunk为C++开发者提供了简单高效的序列化解决方案，特别适合需要轻量级序列化功能的场景。