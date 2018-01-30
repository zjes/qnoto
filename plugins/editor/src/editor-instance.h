#pragma once
#include <QObject>

class EditorInstance : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fileName   READ _fileName   NOTIFY fileNameChanged)

signals:
    void fileNameChanged();

public:
    EditorInstance();

public slots:
    void load(const QString& fileName);

private slots:
    const QString& _fileName() const;

private:
    QString m_fileName;
};
