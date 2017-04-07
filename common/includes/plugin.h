#pragma once
#include <QWidget>
#include <QSharedPointer>
#include "common-export.h"

class QSettings;

namespace qnoto {

#define FORWARD(X)\
    class X; \
    using X ## Ptr = QSharedPointer<X>;

FORWARD(Plugin)

class PreferencesPage: public QWidget
{
public:
    virtual bool save() = 0;
};

class COMMON_EXPORT Plugin: public QWidget
{
    Q_OBJECT
public:
    Plugin();
    virtual ~Plugin();
    virtual void saveState(QSettings& sets) const;
    virtual void restoreState(QSettings& sets);
    virtual PreferencesPage* preferences() const;
    virtual QString name() const = 0;
    virtual QString title() const = 0;
};

}
