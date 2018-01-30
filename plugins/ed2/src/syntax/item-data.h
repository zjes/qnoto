#pragma once

#include "common.h"

namespace syntax {

FORWARD(ItemData)

class ItemData: public Base
{
public:
    ItemData(const QString& name, const QString& styleNum);

    void setDefColor(const QString& color);
    void setDefSelColor(const QString& color);
    void setDefBold(bool isBold);
    void setDefItalic(bool isItalic);

public:
    const QString& name() const;
    const QString& defColor() const;
    const QString& style() const;
    bool isDefBold() const;
    bool isDefItalic() const;

private:
    QString m_name;
    QString m_styleNum;

    QString m_defColor;
    QString m_defSelColor;
    bool    m_defBold = false;
    bool    m_defItalic = false;
};

}
