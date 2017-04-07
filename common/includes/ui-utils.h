#pragma once
#include <QWidget>
#include <QVBoxLayout>

namespace qnoto {

namespace detail {

    inline void addToLayout(QBoxLayout* lay, QWidget* wid)
    {
        lay->addWidget(wid);
    }

    inline void addToLayout(QBoxLayout* lay, QLayout* item)
    {
        lay->addLayout(item);
    }

    template<typename... Others>
    inline void addToLayout(QBoxLayout* lay, QWidget* wid, Others&... others)
    {
        lay->addWidget(wid);
        addToLayout(lay, others...);
    }

    template<typename... Others>
    inline void addToLayout(QBoxLayout* lay, QLayout* item, Others&... others)
    {
        lay->addLayout(item);
        addToLayout(lay, others...);
    }
}

template<typename LayoutT, typename... Args>
inline LayoutT* createLayout(const Args&... args)
{
    LayoutT* lay = new LayoutT;
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);

    detail::addToLayout(lay, args...);

    return lay;
}

template<typename LayoutT, typename... Args>
inline LayoutT* setLayout(QWidget* widget, const Args&... args)
{
    LayoutT* lay = createLayout<LayoutT>(args...);
    widget->setLayout(lay);
    return lay;
}

}
