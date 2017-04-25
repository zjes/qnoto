#pragma once
#include <QSharedPointer>
#include <set>

namespace syntax {

#define FORWARD(X)\
    class X; \
    using X ## Ptr = QSharedPointer<X>;

#define cast(X, CLS) qSharedPointerDynamicCast<X>(CLS)

FORWARD(Base)

class Base: public QEnableSharedFromThis<Base>
{
public:
    virtual ~Base() = default;
};

inline bool isDelimeter(const QChar& ch)
{
    static std::set<QChar> delims{ '\t', ' ', '!', '%', ',', '&', '(', ')', '*', '+', '-', '"',
        '.', '/', ':', ';', '\'', '<', '=', '>', '?', '[', '\\', ']', '^', '{', '|', '}', '~' };
    return delims.find(ch) != delims.end();
}

}
