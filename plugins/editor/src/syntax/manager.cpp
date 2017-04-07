#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <QDomDocument>
#include <QCoreApplication>
#include "manager.h"
#include "parser.h"
#include "definition.h"

namespace syntax {

Manager& Manager::instance()
{
    static Manager inst;
    return inst;
}

Manager::Manager()
{
    QDir synDir(QCoreApplication::applicationDirPath());
    synDir.cd("syntax");

    for(const QFileInfo& file: synDir.entryInfoList({"*.xml"})){
        readFile(file);
    }

    for(const auto& info: m_info){
        qDebug() << "loaded:" << info.name << "pattern:" << info.pattern << "from:" << info.syntaxFile;
    }
}

void Manager::readFile(const QFileInfo& info)
{
    QDomDocument doc("syntax");
    QFile file(info.absoluteFilePath());
    if (!file.open(QIODevice::ReadOnly))
        return;

    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    auto attrs = doc.namedItem("language").attributes();
    m_info.append({
        attrs.namedItem("name").nodeValue(),
        attrs.namedItem("extensions").nodeValue(),
        info.absoluteFilePath(),
        {}
    });
}

DefinitionPtr Manager::definition(const QString& file)
{
    for(auto& info: m_info){
        for(const QString& pat: info.pattern.split(";", QString::SplitBehavior::SkipEmptyParts)){
            QRegExp rx(pat, Qt::CaseSensitive, QRegExp::WildcardUnix);
            if (rx.indexIn(file) != -1){
                if (!info.definition){
                    initDefinition(info.definition, info.syntaxFile);
                }
                return info.definition;
            }
        }
    }
    return {};
}

void Manager::initDefinition(DefinitionPtr& def, const QString& file)
{
    Parser parser(file);
    def = DefinitionPtr::create();
    parser.parse(def);
}

}
