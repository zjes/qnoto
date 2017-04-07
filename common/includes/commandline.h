#pragma once
#include <functional>
#include <QString>
#include <QVector>

#include "includes/convert.h"

namespace qnoto {

class CommandLine
{
public:
    class Format
    {
    public:
        enum Match
        {
            None,
            Long,
            Short,
        };

    public:
        Format(const QString& frm, bool isBool):
            m_isBool(isBool)
        {
            QStringList lst = frm.split("|");
            if (lst.size() == 2){
                m_long = "--" + lst[0];
                m_short = "-" + lst[1];
            } else {
                m_long = "--" + lst[0];
            }
        }

        Match match(const QString& option, QString& value) const
        {
            if (option.startsWith(m_long)){
                if (m_isBool)
                    return Long;

                if (option[m_long.length()] != '=')
                    return None;

                value = option.mid(m_long.length()+1);
                return Long;
            } else if (option.startsWith(m_short)) {
                if (m_isBool)
                    return Short;

                value = option.mid(m_long.length()+1).trimmed();
                return Short;
            }
            return None;
        }

        QString usage() const
        {
            if (m_isBool){
                return m_long + (!m_short.isEmpty() ? " or " + m_short : "");
            } else {
                return m_long + "=[value]" + (!m_short.isEmpty() ? " or " + m_short + " [value]": "");
            }
        }
    private:
        bool m_isBool;
        QString m_long;
        QString m_short;
    };

    class CommandOption
    {
        friend class CommandLine;
    public:
        template<typename T>
        CommandOption(T& variable, const QString& format, const QString& description, bool required = false):
            getter([&variable](CommandOption& opt, const QString& val){
                if (opt.m_isBool){
                    variable = true;
                    opt.m_isSetted = true;
                } else {
                    variable = convert<decltype(variable)>(val);
                    opt.m_isSetted = true;
                }
            }),
            m_isBool(is_same<T, bool>::value),
            m_format(format, m_isBool),
            m_description(description),
            m_required(required),
            m_default(convert<QString>(variable))
        {}

        bool parse(int* arg, char** argv)
        {
            QString value;
            Format::Match match = m_format.match(argv[*arg], value);
            if (match == Format::None)
                return false;

            if (match == Format::Long){
                getter(*this, value);
            } else {
                getter(*this, argv[++(*arg)]);
            }
            return true;
        }

        QString usage() const
        {
            return m_format.usage() + ": " + m_description + (!m_default.isEmpty() ? " (default: " + m_default + ")" : "") + (m_required ? "(required)" : "");
        }

    public:
        std::function<void(CommandOption&, const QString&)> getter;
    protected:
        bool    m_isBool;
        Format  m_format;
        QString m_description;
        bool    m_required;
        QString m_default;
        bool    m_isSetted = false;
    };

public:
    CommandLine(const QString& description, QList<CommandOption>&& options):
        m_description(description),
        m_options(qMove(options))
    {}

    bool parse(int argc, char** argv)
    {
        for(int i = 1; i < argc; ++i){
            bool matched = false;
            for(CommandOption& opt: m_options){
                if (opt.parse(&i, argv)){
                    matched = true;
                    break;
                }
            }
            if (!matched)
                m_position.append(argv[i]);
        }
        return check();
    }

    QString usage() const
    {
        QString out;
        QTextStream st(&out);
        st << m_description << endl;
        for(const CommandOption& opt: m_options){
            st << "  " << opt.usage() << endl;
        }
        return out;
    }

    const QString& errorString() const
    { return m_error; }

    const QList<QString> positionalArgs() const
    { return m_position; }

private:
    bool check() const
    {
        for(const CommandOption& opt: m_options){
            if (opt.m_required && !opt.m_isSetted){
                m_error = "Option \"" + opt.m_description + "\" was not setted";
                return false;
            }
        }
        return true;
    }
private:
    QString m_description;
    QList<CommandOption> m_options;
    mutable QString m_error;
    QList<QString> m_position;
};

}
