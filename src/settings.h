#pragma once
#include <QAbstractListModel>
#include "includes/settings.h"

class Settings: public qnoto::Settings
{
    Q_OBJECT
    Q_PROPERTY(bool restoreState READ restoreState NOTIFY dummy)
    Q_PROPERTY(QStringList files READ files NOTIFY dummy)
public:
    Settings();
public:
    bool restoreState() const;
    QStringList files() const;
    void openFile(const QString& file);
    void closeFile(const QString& file);
signals:
    void dummy();
};

