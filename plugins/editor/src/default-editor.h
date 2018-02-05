#pragma once

#include <QQuickItem>
#include <QQmlExtensionPlugin>
#include <QPlainTextEdit>
#include "syntax/definition.h"
#include "syntax/theme.h"
#include "syntax/highlighting.h"


class DefaultEditor: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString          fileName    READ fileName   WRITE setFileName   NOTIFY dummy)
    Q_PROPERTY(QQuickItem*      target      READ target     WRITE setTarget     NOTIFY targetChanged)
    Q_PROPERTY(QString          text        READ text       WRITE setText       NOTIFY textChanged)
    Q_PROPERTY(syntax::Theme*   theme       READ theme                          NOTIFY dummy)
    Q_PROPERTY(bool             isModified  READ isModified                     NOTIFY isModifiedChanged)

public:
    DefaultEditor();

signals:
    void dummy();
    void targetChanged();
    void textChanged();
    void isModifiedChanged();

private slots:
    const QString& fileName() const;
    void setFileName(const QString& fname);

    QQuickItem *target();
    void setTarget(QQuickItem *target);

    const QString& text() const;
    void setText(const QString& text);

    syntax::Theme* theme() const;
    bool isModified() const;
    void modificationChanged(bool modification);
private:
    QString m_fileName;
    QQuickItem *m_target = nullptr;
    QTextDocument *m_doc = nullptr;

    syntax::DefinitionPtr                m_definition;
    QScopedPointer<syntax::Highlighting> m_syntax;
    syntax::ThemePtr                     m_theme;
    QString m_text;
};

