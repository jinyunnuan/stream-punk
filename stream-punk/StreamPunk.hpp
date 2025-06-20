# pragma once

# include <iostream>

# include <type_traits>
# include <cstdint>
# include <vector>
# include <array>
# include <string>
# include <string_view>

# include <deque>
# include <list>
# include <forward_list>
# include <map>
# include <unordered_map>
# include <set>
# include <unordered_set>
# include <bitset>
# include <memory>
# include <chrono>

# include <algorithm>

/*
这个库叫流水账
英文名:StreamPunk
基础功能:
    支持数据的序列化和反序列化
基础功能对数据的态度是:
    不管是什么类型的数据,你这个程序应当知道它是什么结构,只要你拥有数据定义的头文件你就应当知道.
*/

# define Xt_BasicType(X__) \
X__(::std::uint8_t , u8 ) \
X__(::std::uint16_t, u16) \
X__(::std::uint32_t, u32) \
X__(::std::uint64_t, u64) \
X__(::std::int8_t  , i8 ) \
X__(::std::int16_t , i16) \
X__(::std::int32_t , i32) \
X__(::std::int64_t , i64) \
X__(float          , f32) \
X__(double         , f64) \
X__(char           , ch ) \
X__(bool           , bl ) \

# define Xt_Type(X__) \
Xt_BasicType(X__)

# define X_using(oldName, newName) using newName = oldName;
Xt_BasicType(X_using);
# undef X_using

namespace detail {
    using Ostr = ::std::ostream;
    using Sz = u32;
    using PtrValue = uint64_t;

    template<typename T> using Sptr = std::shared_ptr<T>;
    template<typename T> using Wptr = std::weak_ptr  <T>;
    template<typename T> using Uptr = std::unique_ptr<T>;

    template <typename T> constexpr bool trivially_copyable =
        std::is_trivially_copyable_v<std::remove_cvref_t<T>>
        &&
        !std::is_pointer_v<std::remove_cvref_t<T>>
    ;
} // namespace detail


/*
    这个流对象会在序列化和反序列化时,存一些上下文数据
    主要是为了避免重复序列化同一个指针,
    以及在反序列化时,避免重复创建对象
*/
struct O {
    std::ostream& s;
    std::unordered_set<detail::PtrValue> ptrSet;

    void clear() {
        ptrSet.clear();
    }
}; // struct S

struct I {
    std::istream& s;
    std::unordered_map<detail::PtrValue, detail::Sptr<void>> sptrSet;
    std::unordered_map<detail::PtrValue, void* > ptrSet;

    void clear() {
        sptrSet.clear();
        ptrSet.clear();
    }
}; // struct I

/* =======================  实现各类数据二进制输入输出 ===================== */

    /* ======================= 基本类型 ===================== */
inline O& operator<<(O& s, const u8 & v) { s.s.write(reinterpret_cast<char const*>(&v), sizeof(v)); return s; }
inline O& operator<<(O& s, const u16& v) { s.s.write(reinterpret_cast<char const*>(&v), sizeof(v)); return s; }
inline O& operator<<(O& s, const u32& v) { s.s.write(reinterpret_cast<char const*>(&v), sizeof(v)); return s; }
inline O& operator<<(O& s, const u64& v) { s.s.write(reinterpret_cast<char const*>(&v), sizeof(v)); return s; }
inline O& operator<<(O& s, const i8 & v) { s.s.write(reinterpret_cast<char const*>(&v), sizeof(v)); return s; }
inline O& operator<<(O& s, const i16& v) { s.s.write(reinterpret_cast<char const*>(&v), sizeof(v)); return s; }
inline O& operator<<(O& s, const i32& v) { s.s.write(reinterpret_cast<char const*>(&v), sizeof(v)); return s; }
inline O& operator<<(O& s, const i64& v) { s.s.write(reinterpret_cast<char const*>(&v), sizeof(v)); return s; }
inline O& operator<<(O& s, const f32& v) { s.s.write(reinterpret_cast<char const*>(&v), sizeof(v)); return s; }
inline O& operator<<(O& s, const f64& v) { s.s.write(reinterpret_cast<char const*>(&v), sizeof(v)); return s; }
inline O& operator<<(O& s, const ch & v) { s.s.write(reinterpret_cast<char const*>(&v), sizeof(v)); return s; }
inline O& operator<<(O& s, const bl & v) { s.s.write(reinterpret_cast<char const*>(&v), sizeof(v)); return s; }

inline I& operator>>(I& s,       u8 & v) { s.s.read(reinterpret_cast<char*>(&v), sizeof(v)); return s; }
inline I& operator>>(I& s,       u16& v) { s.s.read(reinterpret_cast<char*>(&v), sizeof(v)); return s; }
inline I& operator>>(I& s,       u32& v) { s.s.read(reinterpret_cast<char*>(&v), sizeof(v)); return s; }
inline I& operator>>(I& s,       u64& v) { s.s.read(reinterpret_cast<char*>(&v), sizeof(v)); return s; }
inline I& operator>>(I& s,       i8 & v) { s.s.read(reinterpret_cast<char*>(&v), sizeof(v)); return s; }
inline I& operator>>(I& s,       i16& v) { s.s.read(reinterpret_cast<char*>(&v), sizeof(v)); return s; }
inline I& operator>>(I& s,       i32& v) { s.s.read(reinterpret_cast<char*>(&v), sizeof(v)); return s; }
inline I& operator>>(I& s,       i64& v) { s.s.read(reinterpret_cast<char*>(&v), sizeof(v)); return s; }
inline I& operator>>(I& s,       f32& v) { s.s.read(reinterpret_cast<char*>(&v), sizeof(v)); return s; }
inline I& operator>>(I& s,       f64& v) { s.s.read(reinterpret_cast<char*>(&v), sizeof(v)); return s; }
inline I& operator>>(I& s,       ch & v) { s.s.read(reinterpret_cast<char*>(&v), sizeof(v)); return s; }
inline I& operator>>(I& s,       bl & v) { s.s.read(reinterpret_cast<char*>(&v), sizeof(v)); return s; }

    /* ======================= 连续空间输入输出 ===================== */
namespace detail {
    template <typename T> struct is_std_array : std::false_type {};
    template <typename T, size_t N> struct is_std_array<std::array<T, N>> : std::true_type {};
    template <typename T> constexpr bool is_std_array_v = is_std_array<T>::value;

    template<typename ValueType, typename T> inline void writeSpan(O& o, T const& v) {
        o << static_cast<Sz>(v.size());
        if constexpr (trivially_copyable<ValueType>) {
            /*
                如果 ValueType 是平凡可拷贝的类型，直接使用二进制写入
                然而会引来一个问题:如果ValueType是std::array,尺寸是固定已知的,
            */
            o.s.write(reinterpret_cast<char const*>(v.data()), sizeof(ValueType) * v.size());
        }
        else {
            for (auto&& x : v) {
                o << x;
            }
        }
    }
    template <typename ValueType, typename T> inline void readSpan(I& i, T& v) {
        Sz size;
        i >> size;
        if constexpr (!is_std_array_v<T>) {
            v.resize(size);
        }
        if constexpr (trivially_copyable<ValueType>) {
            i.s.read(reinterpret_cast<char*>(v.data()), sizeof(ValueType) * size);
        }
        else {
            for (auto& x : v) {
                i >> x;
            }
        }
    }
} // namespace detail

template<typename T, typename...Args> inline O& operator<<(O& o, std::vector<T, Args...>const& v) {
    detail::writeSpan<T>(o, v); return o;
}
template<typename T, size_t N       > inline O& operator<<(O& o, std::array <T, N      >const& v) {
    detail::writeSpan<T>(o, v); return o;
}
template<typename T, typename...Args> inline O& operator<<(O& o, std::basic_string <T, Args...>const& v) {
    detail::writeSpan<T>(o, v); return o;
}
template<typename T> inline O& operator<<(O& o, std::basic_string_view<T> const& v) {
    detail::writeSpan<T>(o, v); return o;
}

template<typename T, typename...Args> inline I& operator>>(I& i, std::vector<T, Args...>& v) {
    detail::readSpan<T>(i, v); return i;
}
template<typename T, size_t N       > inline I& operator>>(I& i, std::array <T, N      >& v) {
    detail::readSpan<T>(i, v); return i;
}
template<typename T, typename...Args> inline I& operator>>(I& i, std::basic_string <T, Args...>& v) {
    detail::readSpan<T>(i, v); return i;
}
template<typename T                 > inline I& operator>>(I& i, std::basic_string_view<T>& v) {
    detail::readSpan<T>(i, v); return i;
}

    /* ======================= 不连续的容器类型 ===================== */
namespace detail {
    template<typename ValueType, typename T> inline void write(O& o, T const& v) {
        Sz sz = v.size();
        o << sz;
        for (auto&& x : v) {
            o << x;
        }
    }
    template<typename T> inline void writeMap(O& o, T const& v) {
        Sz sz = v.size();
        o << sz;
        for (auto&& x : v) {
            o << x.first << x.second;
        }
    }
}   // namespace detail

template<typename T, typename... Args>             inline O& operator<<(O& o, std::deque        <T,     Args...>const& v) {
    detail::write<T>(o, v); return o;
}
template<typename T, typename... Args>             inline O& operator<<(O& o, std::list         <T,     Args...>const& v) {
    detail::write<T>(o, v); return o;
}
template<typename T, typename... Args>             inline O& operator<<(O& o, std::set          <T,     Args...>const& v) {
    detail::write<T>(o, v); return o;
}
template<typename T, typename... Args>             inline O& operator<<(O& o, std::unordered_set<T,     Args...>const& v) {
    detail::write<T>(o, v); return o;
}
template<typename T, typename... Args>             inline O& operator<<(O& o, std::forward_list <T,     Args...>const& v) {
    detail::Sz sz = 0;
    for (auto&& x : v) {
        ++sz;
    }
    o << sz;
    for (auto&& x : v) {
        o << x;
    }
    return o;
}
template<typename K, typename V, typename... Args> inline O& operator<<(O& o, std::map          <K, V,  Args...>const& v) {
    detail::writeMap(o, v); return o;
}
template<typename K, typename V, typename... Args> inline O& operator<<(O& o, std::unordered_map<K, V,  Args...>const& v) {
    detail::writeMap(o, v); return o;
}
template<size_t N> inline O& operator<<(O& o, std::bitset<N>const& v) {
    detail::Sz sz = N;
    o << sz;
    if (sz <= 64) {
        u64 n = v.to_ullong();
        o.s.write(reinterpret_cast<char const*>(&n), sizeof(n));
    }
    else {
        // 超过64位的bitset，使用字符串表示
        std::string n = v.to_string();
        o.s.write(n.data(), n.size());
    }
    return o;
}

namespace detail {
    template<typename T> inline void read(I& i, T& v) {
        Sz sz;
        i >> sz;
        for (size_t j = 0; j < sz; ++j) {
            v.emplace_back();
            i >> v.back();
        }
    }
    template<typename T> inline void readSet(I& i, T& v) {
        Sz sz;
        i >> sz;
        for (size_t j = 0; j < sz; ++j) {
            typename T::value_type temp{};
            i >> temp;
            v.emplace(std::move(temp));
        }
    }
    template<typename T> inline void readMap(I& i, T& v) {
        Sz sz;
        i >> sz;
        for (size_t j = 0; j < sz; ++j) {
            typename T::key_type k{};
            i >> k;
            i >> v[k];
        }
    }
}   // namespace detail

template<typename T, typename... Args> inline I& operator>>(I& i, std::deque        <T, Args...>& v) {
    detail::read(i, v); return i;
}
template<typename T, typename... Args> inline I& operator>>(I& i, std::list         <T, Args...>& v) {
    detail::read(i, v); return i;
}
template<typename T, typename... Args> inline I& operator>>(I& i, std::set          <T, Args...>& v) {
    detail::readSet(i, v); return i;
}
template<typename T, typename... Args> inline I& operator>>(I& i, std::unordered_set<T, Args...>& v) {
    detail::readSet(i, v); return i;
}
template<typename T, typename... Args> inline I& operator>>(I& i, std::forward_list <T, Args...>& v) {
    detail::Sz sz;
    i >> sz;
    for (size_t j = 0; j < sz; ++j) {
        T temp{};
        i >> temp;
        v.emplace_front(std::move(temp));
    }
    v.reverse();
    return i;
}
template<typename K, typename V, typename... Args> inline I& operator>>(I& i, std::map          <K, V, Args...>& v) {
    detail::readMap(i, v); return i;
}
template<typename K, typename V, typename... Args> inline I& operator>>(I& i, std::unordered_map<K, V, Args...>& v) {
    detail::readMap(i, v); return i;
}
template<size_t   N> inline I& operator>>(I& i, std::bitset<N>& v) {
    detail::Sz sz;
    i >> sz;
    if (sz <= 64) {
        u64 n;
        i.s.read(reinterpret_cast<char*>(&n), sizeof(n));
        v = std::bitset<N>(n);
    }
    else {
        // 超过64位的bitset，使用字符串表示
        std::string str(sz, '0');
        i.s.read(str.data(), sz);
        v = std::bitset<N>(str);
    }
    return i;
}

    /* ======================= 指针类型 ===================== */
template<typename T> inline O& operator<<(O& o, T const* const v) {
    auto const p = reinterpret_cast<detail::PtrValue>(v);
    o << p;
    if (v == nullptr) {
        return o;
    }
    if (o.ptrSet.find(p) == o.ptrSet.end()) {
        o.ptrSet.emplace(p);
        o << (*v);
        # if _DEBUG
        std::cout << "output Pointer: " << std::showbase << std::uppercase << std::hex << p << std::endl;
        # endif
    }
    return o;
}
template<typename T> inline O& operator<<(O& o, detail::Sptr<T>const& v) { o << v.get(); return o; }
template<typename T> inline O& operator<<(O& o, detail::Wptr<T>const& v) { o << v.lock().get(); return o; }
template<typename T> inline O& operator<<(O& o, detail::Uptr<T>const& v) { o << v.get(); return o; }

template<typename T> inline I& operator>>(I& i, T*& v) {
    detail::PtrValue p = 0;
    i >> p;
    if ( (void*)p == nullptr) {
        v = nullptr;
        return i;
    }
    auto ptrIter = i.ptrSet.find(p);
    auto sptrIter = i.sptrSet.find(p);
    if (ptrIter != i.ptrSet.end()) {
        v = reinterpret_cast<T*>(ptrIter->second);
        return i;
    }
    if (sptrIter != i.sptrSet.end()) {
        v = reinterpret_cast<T*>(sptrIter->second.get());
        return i;
    }
    v = new T{};
    i >> (*v);
    i.ptrSet.emplace(p, v);
    return i;
}
template<typename T> inline I& operator>>(I& i, detail::Sptr<T>& v) {
    detail::PtrValue p = 0;
    i >> p;
    if (p == 0) {
        v.reset();
        return i;
    }
    # if _DEBUG
    std::cout << "input  Pointer: " << std::showbase << std::uppercase << std::hex << p << std::endl;
    # endif
    auto iter = i.sptrSet.find(p);
    if (iter != i.sptrSet.end()) {
        v = std::static_pointer_cast<T>(iter->second);
        return i;
    }
    auto ptrIter = i.ptrSet.find(p);
    if (ptrIter != i.ptrSet.end()) {
        v.reset((T*)(ptrIter->second));
        return i;
    }
    auto ptr = new T{};
    v.reset(ptr);
    i.sptrSet.emplace(p, v);
    i >> *ptr;
    return i;
}

template<typename T> inline I& operator>>(I& i, detail::Wptr<T>& v) {
    detail::PtrValue p;
    i >> p;
    v.reset();
    if (p == 0) {
        return i;
    }
    auto sIter = i.sptrSet.find(p);
    if (sIter != i.sptrSet.end()) {
        v = std::static_pointer_cast<T>(sIter->second);
        return i;
    }
    auto ptrIter = i.ptrSet.find(p);
    if (ptrIter != i.ptrSet.end()) {
        /*
            父类指针指向子类的对象, 先放入到流当中,
            反序列化时,用父类指针去取出,对象就会被新建成父类对象. 从而出错.
        */
        detail::Sptr<T> sptr(reinterpret_cast<T*>(ptrIter->second));
        i.sptrSet.emplace(p, sptr);
        v = sptr;
        return i;
    }
    detail::Sptr<T> sptr(new T{});
    i >> *sptr;
    v = sptr;
    i.sptrSet.emplace(p, std::move(sptr));
    return i;
}
template<typename T> inline I& operator>>(I& i, detail::Uptr<T>& v) { T* p{}; i >> p; v.reset(p); return i; }


