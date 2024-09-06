#ifndef COMMON_H
#define COMMON_H
/**
 * @brief 该文件是通用的数据接口接口类模板，声明全局变量
 * 
 * @details
 * Author: Tang
 * Date: 2024-08
 * Version: 0.0.1
*/

#ifdef API_LIBRARY
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT __declspec(dllimport)
#endif

#include <QDebug>
#include <QString>
using String = QString;

//integer to string 简写数字转字符
inline String i2s(int num) {
    std::string str = std::to_string(num);
    return QString::number(num);
}

inline String i2s(qint64 num) {
    return QString::number(num);
}

/**定义一个结构体来包含更详细的结果信息**/
struct Result
{
    bool success;
    String message;

    Result(bool s = false, const String& msg =""):success(s),message(msg) {}
    operator bool() const {return success;}//重载了 bool 操作符，使其可以像之前的 bool 返回值一样使用例如：if (result)
};


/**定义一个原子类的结构体用于记录设备等的状态信息**/
#include <QAtomicInteger>
template<typename T>
struct State
{
    QAtomicInteger<T> state;
    String message;

    T getState() const { return state.load(); }
    void setState(T s) { state.store(s); }
    //在构造函数中，可以直接用 T 类型的值初始化它
    State(T s = T(),const QString& msg = QString()) : state(s), message(msg) {}

    // 重载赋值运算符，允许直接赋值
    State& operator=(T value) {
        setState(value);
        return *this;
    }

    // 重载类型转换运算符，允许隐式转换为 T 类型
    operator T() const { return getState(); }

    operator bool() const {return state.load();}
};

State<int> AppStatus(10, "Initial state");

#endif // COMMON_H