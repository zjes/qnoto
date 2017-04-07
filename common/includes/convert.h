#pragma once

#include <QString>
#include <QTextStream>
#include <QDateTime>

namespace qnoto {

template<typename T, typename... Rest>
struct is_same: std::false_type
{};

template<typename T, typename First>
struct is_same<T, First>: std::is_same<std::decay_t<T>, std::decay_t<First>>
{};

template<typename T, typename First, typename... Rest>
struct is_same<T, First, Rest...>: std::integral_constant<bool, is_same<T, First>::value || is_same<T, Rest...>::value>
{};

//template<typename T>
//struct is_string: is_same<T, QString, const char*, char*>
//{};

template<typename T>
using is_string = is_same<T, QString, const char*, char*>;

template<typename T>
struct is_integral: std::is_integral<std::decay_t<T>>
{};

template<typename T1, typename T2>
struct is_cast: std::is_convertible<std::decay_t<T1>, std::decay_t<T2>>
{};

//From string to datetime
template<typename To, typename From>
inline std::decay_t<To> convert(const From& from, std::enable_if_t<is_string<From>::value && is_same<To, QDateTime>::value>* = 0)
{
    return QDateTime::fromString(from, "yyyy-MM-dd hh:mm:ss");
}

//Pass datetime to datetime
template<typename To, typename From>
inline std::decay_t<To> convert(const From& from, std::enable_if_t<is_same<From, QDateTime>::value && is_same<To, QDateTime>::value>* = 0)
{
    return from;
}

//From datetime to string
template<typename To, typename From>
inline To convert(const From& from, std::enable_if_t<is_string<To>::value && is_same<From, QDateTime>::value>* = 0)
{
    return from.toString("yyyy-MM-dd hh:mm:ss");
}

//From timestamp to datetime
template<typename To, typename From>
inline std::decay_t<To> convert(const From& from, std::enable_if_t<is_integral<From>::value && is_same<To, QDateTime>::value>* = 0)
{
    return QDateTime::fromMSecsSinceEpoch(from);
}

//Datetime to timestamp
template<typename To, typename From>
inline std::decay_t<To> convert(const From& from, std::enable_if_t<is_integral<To>::value && is_same<From, QDateTime>::value>* = 0)
{
    return from.toMSecsSinceEpoch();
}

//Cast pass
template<typename To, typename From>
inline std::decay_t<To> convert(const From& from, std::enable_if_t<is_cast<To, From>::value && !is_string<To>::value>* = 0)
{
    return from;
}

//Cast pass
template<typename To, typename From>
inline std::decay_t<To> convert(const From& from, std::enable_if_t<is_string<From>::value && is_same<To, QString>::value>* = 0)
{
    return from;
}


//String to any except bool and datetime
template<typename To, typename From>
inline std::decay_t<To> convert(const From& from, std::enable_if_t<is_string<From>::value && !is_string<To>::value && !is_same<To, bool, QDateTime>::value>* = 0)
{
    std::decay_t<To> val;
    QString valStr = from;
    QTextStream st(&valStr);
    st >> val;
    return val;
}

//String to bool
template<typename To, typename From>
inline std::decay_t<To> convert(const From& from, std::enable_if_t<is_string<From>::value && is_same<To, bool>::value>* = 0)
{
    return from == QString("true") || from == QString("1");
}

//Any type except datetime to string
template<typename To, typename From>
inline std::decay_t<To> convert(const From& from, std::enable_if_t<!is_string<From>::value && !is_same<From, QDateTime>::value && is_same<To, QString>::value>* = 0)
{
    QString out;
    QTextStream ss(&out);
    if(std::is_same<bool, std::decay_t<From>>()){
        ss << (from ? "true" : "false");
    } else {
        ss << from;
    }
    return out;
}

}

