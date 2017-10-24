#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QIcon>
#include <QAction>

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

inline QIcon icon(const QString& name)
{
    if (QIcon::hasThemeIcon(name))
        return QIcon::fromTheme(name);
    return QIcon::fromTheme(":/icons/"+QIcon::themeName()+"/"+name);
}

template<typename Parent>
class Action
{
public:
    Action(Parent* parent, const QString& name):
        m_action(new QAction(parent))
    {
        m_action->setProperty("name", name);
    }
    Action& title(const QString& title)
    {
        m_action->setText(title);
        m_action->setProperty("orig-text", title);
        return *this;
    }
    Action& enabled(bool enabled)
    {
        m_action->setEnabled(enabled);
        return *this;
    }
    template<typename Func>
    Action& enabled(Func&& /*func*/)
    {
        //m_action->connect(m_action, &QAction::)
        return *this;
    }
    Action& shortcut(const QKeySequence& seq)
    {
        m_action->setShortcut(seq);
        return *this;
    }
    Action& icon(const QIcon& icon)
    {
        m_action->setIcon(icon);
        return *this;
    }
    Action& icon(const QString& icon)
    {
        m_action->setIcon(qnoto::icon(icon));
        return *this;
    }
    template<typename Func>
    Action& action(Func&& func)
    {
        m_action->connect(m_action, &QAction::triggered, qobject_cast<Parent*>(m_action->parent()), std::move(func));
        return *this;
    }
    operator QAction*() const
    {
        return m_action;
    }
private:
    QAction* m_action = nullptr;
};


}
