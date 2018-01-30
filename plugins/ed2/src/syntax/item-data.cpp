#include "item-data.h"

namespace syntax {

ItemData::ItemData(const QString& name, const QString& styleNum):
    m_name(name),
    m_styleNum(styleNum)
{}

void ItemData::setDefColor(const QString& color)
{
    m_defColor = color;
}

void ItemData::setDefSelColor(const QString& color)
{
    m_defSelColor = color;
}

void ItemData::setDefBold(bool isBold)
{
    m_defBold = isBold;
}

void ItemData::setDefItalic(bool isItalic)
{
    m_defItalic = isItalic;
}

const QString& ItemData::name() const
{
    return m_name;
}

const QString& ItemData::defColor() const
{
    return m_defColor;
}

bool ItemData::isDefBold() const
{
    return m_defBold;
}

bool ItemData::isDefItalic() const
{
    return m_defItalic;
}

const QString& ItemData::style() const
{
    return m_styleNum;
}


}
