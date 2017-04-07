#pragma once
#include <QSharedPointer>

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
    static QString delims = "\t !%&()*+,-./:;<=>?[\\]^{|}~";
    return std::binary_search(delims.constBegin(), delims.constEnd(), ch);
}

}
