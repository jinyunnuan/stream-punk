[汉语说明](./README_chinese.md)

# StreamPunk - Modern C++ High-Performance Serialization Library

## Why Choose StreamPunk?

In today's data-driven applications, efficient serialization solutions are critical. StreamPunk is designed for modern C++ and offers:

🚀 **Extreme Performance** - Direct binary operations eliminate text parsing overhead  
🧩 **Zero-Dependency Integration** - Pure header-only implementation for seamless project integration  
💡 **Modern Feature Support** - Full compatibility with C++17/20 features including smart pointers, time types, and atomic operations  
🌐 **Cross-Platform Compatibility** - Built-in machine feature detection automatically handles endianness and architecture differences  
🧠 **Deep Object Support** - Native handling of polymorphism, circular references, and complex nested structures  

## Solution Comparison: StreamPunk vs Mainstream Serialization Solutions

| Feature                | StreamPunk | JSON       | Protocol Buffers | FlatBuffers | Hessian     |
|------------------------|------------|------------|------------------|-------------|-------------|
| **Binary Format**      | ✅          | ❌          | ✅               | ✅           | ✅           |
| **Zero-Copy Access**   | ❌          | ❌          | ❌               | ✅           | ❌           |
| **Header-Only**        | ✅          | ❌          | ❌               | ❌           | ❌           |
| **C++20 Support**      | ✅🌟        | ❌          | ❌               | ❌           | ❌           |
| **Smart Pointers**     | ✅🌟        | ❌          | ❌               | ❌           | ❌           |
| **Polymorphic Support**| ✅🌟        | ❌          | ⚠️ Limited       | ❌           | ✅           |
| **Cross-Platform**     | ✅🌟        | ✅          | ✅               | ✅           | ✅           |
| **Ease of Installation** | ⭐⭐⭐⭐⭐     | ⭐⭐⭐⭐       | ⭐⭐⭐            | ⭐⭐          | ⭐⭐⭐         |
| **Usage Flexibility**  | ⭐⭐⭐⭐⭐     | ⭐⭐⭐⭐       | ⭐⭐⭐            | ⭐⭐          | ⭐⭐⭐         |
| **Performance Estimate** | ⭐⭐⭐⭐       | ⭐⭐         | ⭐⭐⭐⭐           | ⭐⭐⭐⭐⭐      | ⭐⭐⭐         |
| **Raw Pointer Support**| ✅🌟        | ❌          | ❌               | ❌           | ❌           |
| **Multi-Language**     | 🚧 Planned  | ✅          | ✅               | ✅           | ✅           |

### Key Advantages:
- **Installation Simplicity**: Pure header-only design with zero dependencies
- **Performance**: StreamPunk's direct binary operations rank second only to FlatBuffers' zero-copy
- **Raw Pointer Support**: Unique native handling of C++ raw pointers
- **Multi-Language Roadmap**: Future support for JS/TS/Python/Kotlin/Java/Go/Rust

## Quick Start Guide

### 1. Include Header File
```cpp
#include "StreamPunk.hpp"
```

### 2. Register Custom Types (in customData.hpp)
```cpp
// customData.hpp
#define Xt_CustomType(X__) \
X__(Device, Device) \
X__(Sensor, Sensor) \
X__(SmartHomeSystem, SmartHomeSystem)
// Add more business-specific types...
```

### 3. Create Serializable Class
```cpp
struct Device : public Base {
    #define Xt_Device(X__) \
    X__(std::string, deviceId, "unknown") \
    X__(std::string, manufacturer, "") \
    X__(std::chrono::system_clock::time_point, lastSeen, {})
    
    Device() = default;
    UseData(Device); // Critical macro declaration
};
```

### 4. Core API Usage
```cpp
int main() {
    INIT_StreamPunk(); // Mandatory initialization
    
    // Serialization
    SmartHomeSystem homeSystem;
    std::stringstream binStream;
    O output{binStream};
    output << homeSystem;
    
    // Deserialization
    SmartHomeSystem restored;
    I input{binStream};
    input >> restored;
    
    // Deep Copy
    DeepCopier copier;
    SmartHomeSystem copy;
    deepCopy(copier, copy, homeSystem);
    copier.clear(); // Mandatory cleanup
}
```

## Feature Overview

### Supported Data Types
| Category            | Supported Types                                                                 |
|---------------------|---------------------------------------------------------------------------------|
| **Primitives**      | All integers/floats, bool, character types (char, wchar_t, char8_t, etc.)       |
| **Containers**      | vector, list, deque, map, set, unordered_map, unordered_set, bitset            |
| **Smart Pointers**  | shared_ptr, unique_ptr, weak_ptr (native support)                               |
| **Time Types**      | chrono::duration, chrono::time_point                                           |
| **Filesystem**      | filesystem::path (full support)                                                |
| **Advanced Types**  | variant, optional, atomic, tuple, any                                         |
| **Special Types**   | Serialization only: initializer_list, span, string_view            |

### Core Features
- 🔄 **Polymorphic Serialization** - Automatic handling of inheritance hierarchies
- ♻️ **Circular References** - Intelligent handling of object graphs with mutual references
- 📊 **Deep Copy** - Complete object graph duplication system
- 📝 **Self-Describing Data** - Built-in type description system
- 🔋 **Low Memory Overhead** - Efficient memory management strategies
- ⏱️ **Time Precision** - Attosecond-level time resolution

## Usage Guidelines & Best Practices

### Critical Constraints
1. **Compilation Requirements**:
   - C++20 or higher standard
   - RTTI must be enabled
   - UTF-8 encoding without BOM

2. **Class Design**:
   ```cpp
   struct MyClass : public Base { // Must inherit from Base
       // Declare serialized fields using macro
       #define Xt_MyClass(X__) \
           X__(int, value, 0) \
           X__(std::string, name, "")
       
       UseData(MyClass); // Critical macro
   };
   ```

3. **Pointer Usage**:
   - Raw pointer members are permitted but recommended only when used with std::unique_ptr
   - Recommended combinations:
     - std::shared_ptr + std::weak_ptr
     - std::unique_ptr + raw pointer
   - Important: When using void*, ensure pointed objects are serialized BEFORE the void* pointer

### Cross-Platform Considerations
```cpp
struct StreamPunkMachineInfo {
    u8 endian; // Endianness: 0=Little, 1=Big
    // Other platform features...
    
    void init() {
        // Auto-detect current platform features
        if constexpr (std::endian::native == std::endian::little) {
            endian = 0; // Little-endian
        }
        else if constexpr (std::endian::native == std::endian::big) {
            endian = 1; // Big-endian
        }
        // ...
    }
};
```

**Important Limitation**: Current version only supports data exchange between same-endian architectures (big-big or little-little), not cross-endian transfers.

### Performance Optimization
- Use direct memory operations for trivially copyable types
- Utilize `std::span` for large arrays to avoid copying
- Reuse `O`/`I` objects during frequent serialization operations

## Development Roadmap
- 🚧 v0.1.3: Cross-platform data query capabilities
- 🚀 v0.2.0: Data version management
- 🌍 v0.3.x: Multi-language support (JS/TS/Python/Kotlin/Java/Go/Rust)
- 🎨 v0.4.0: Data visualization tools

StreamPunk provides a powerful and flexible serialization solution for modern C++ applications, especially suitable for scenarios requiring complex object relationships and high-performance data exchange. With its clean API design and deep system integration, it significantly reduces development costs for complex data processing.