#pragma once
#include <QTextBlockUserData>

namespace syntax {

struct State;

struct Folding
{
    enum Type {
        None,
        Begin,
        End
    };

    operator bool() const
    {
        return type != None;
    }

    bool checkEndBlock(const QTextBlock& blk);

    Type type   = None;
    int  offset = 0;
    bool closed = false;
    int  id     = 0;
};

struct UserData: public QTextBlockUserData
{
    virtual ~UserData() = default;

    QSharedPointer<State>  state;
    std::map<int, QString> markFound;
    Folding                folding;
};

inline UserData* userData(const QTextBlock& blk)
{
    return dynamic_cast<syntax::UserData*>(blk.userData());
}

inline Folding* folding(const QTextBlock& blk, bool all = false)
{
    UserData* data = userData(blk);
    if (data && (data->folding.type == Folding::Begin || (all && data->folding.type == Folding::End)))
        return &data->folding;
    return nullptr;
}

inline bool Folding::checkEndBlock(const QTextBlock& blk)
{
    auto* data = userData(blk);
    return data && data->folding.type == Folding::End && id == data->folding.id;
}

}
