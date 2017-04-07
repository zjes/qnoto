#include <QFile>
#include <QDebug>
#include <QDomDocument>
#include "includes/convert.h"
#include "parser.h"
#include "definition.h"
#include "keywordlist.h"
#include "context.h"
#include "item-data.h"

#include "rules/rule.h"
#include "rules/anychar.h"
#include "rules/detectchar.h"
#include "rules/detect2char.h"
#include "rules/detectidentifier.h"
#include "rules/detectspaces.h"
#include "rules/float.h"
#include "rules/hlcchar.h"
#include "rules/hlchex.h"
#include "rules/hlcoct.h"
#include "rules/hlcstringchar.h"
#include "rules/int.h"
#include "rules/keyword.h"
#include "rules/linecontinue.h"
#include "rules/rangedetect.h"
#include "rules/regexpr.h"
#include "rules/stringdetect.h"
#include "rules/worddetect.h"
#include "rules/includerules.h"

namespace syntax {

struct Walker
{
    static void walk(const QDomElement& el, const BasePtr& out)
    {
        auto children = el.childNodes();
        for(int i = 0; i < children.size(); ++i){
            if (children.at(i).nodeType() != QDomNode::ElementNode)
                continue;
            visit(children.at(i).toElement(), out);
        }
    }

    static void visit(const QDomElement& el, const BasePtr& out)
    {
        if (is(el, "highlighting")){
            walk(el, out);
        } else if (is(el, "list")){
            visitList(el, out);
        } else if (is(el, "item")){
            visitItem(el, out);
        } else if (is(el, "contexts")){
            walk(el, out);
        } else if (is(el, "context")){
            visitContext(el, out);
        } else if (is(el, "AnyChar")){
            visitAnyChar(el, out);
        } else if (is(el, "DetectChar")){
            visitDetectChar(el, out);
        } else if (is(el, "Detect2Chars")){
            visitDetect2Chars(el, out);
        } else if (is(el, "DetectSpaces")){
            visitRule<DetectSpaces>(el, out);
        } else if (is(el, "DetectIdentifier")){
            visitRule<DetectIdentifier>(el, out);
        } else if (is(el, "Float")){
            visitRule<Float>(el, out);
        } else if (is(el, "HlCChar")){
            visitRule<HlCChar>(el, out);
        } else if (is(el, "HlCHex")){
            visitRule<HlCHex>(el, out);
        } else if (is(el, "HlCOct")){
            visitRule<HlCOct>(el, out);
        } else if (is(el, "Int")){
            visitRule<Int>(el, out);
        } else if (is(el, "HlCStringChar")){
            visitRule<HlCStringChar>(el, out);
        } else if (is(el, "keyword")){
            visitKeywordRule(el, out);
        } else if (is(el, "LineContinue")){
            visitLineContinue(el, out);
        } else if (is(el, "RangeDetect")){
            visitRangeDetect(el, out);
        } else if (is(el, "RegExpr")){
            visitRegExpr(el, out);
        } else if (is(el, "StringDetect")){
            visitStringDetect(el, out);
        } else if (is(el, "WordDetect")){
            visitWordDetect(el, out);
        } else if (is(el, "IncludeRules")){
            visitIncludeRules(el, out);
        } else if (is(el, "itemDatas")){
            walk(el, out);
        } else if (is(el, "itemData")){
            visitItemData(el, out);
        } else {
            qDebug() << "unhandled tag" << el.tagName();
            walk(el, out);
        }
    }

    static void visitList(const QDomElement& el, const BasePtr& out)
    {
        DefinitionPtr def = cast(Definition, out);
        KeywordListPtr lst = def->keywordList(attr(el, "name"));
        walk(el, lst);
    }

    static void visitItem(const QDomElement& el, const BasePtr& out)
    {
        KeywordListPtr list = cast(KeywordList, out);
        list->add(el.text());
    }

    static void visitContext(const QDomElement& el, const BasePtr& out)
    {
        ContextPtr context = cast(Definition, out)->createContext(attr(el, "name"));
        if (el.hasAttribute("attribute"))
            context->setAttribute(attr(el, "attribute"));
        if (el.hasAttribute("dynamic"))
            context->setDynamic(attr<bool>(el, "dynamic"));
        if (el.hasAttribute("fallthrough"))
            context->setFallThrough(attr<bool>(el, "fallthrough"));
        if (el.hasAttribute("fallthroughContext"))
            context->setFallThroughContext(attr(el, "fallthroughContext"));
        if (el.hasAttribute("lineBeginContext"))
            context->setLineBeginContext(attr(el, "lineBeginContext"));
        if (el.hasAttribute("lineEndContext"))
            context->setLineEndContext(attr(el, "lineEndContext"));

        walk(el, context);
    }

    static void setupRule(const QDomElement& el, const RulePtr& rule, const BasePtr& out)
    {
        if (el.hasAttribute("attribute"))
            rule->setAttribute(attr(el, "attribute"));
        if (el.hasAttribute("firstNonSpace"))
            rule->setFirstNonSpace(attr<bool>(el, "firstNonSpace"));
        if (el.hasAttribute("lookAhead"))
            rule->setLookAhead(attr<bool>(el, "lookAhead"));
        if (el.hasAttribute("column"))
            rule->setColumn(attr<int>(el, "column"));
        if (el.hasAttribute("dynamic"))
            rule->setDynamic(attr<bool>(el, "dynamic"));
        if (el.hasAttribute("beginRegion"))
            rule->setBeginRegion(attr(el, "beginRegion"));
        if (el.hasAttribute("endRegion"))
            rule->setEndRegion(attr(el, "endRegion"));
        if (el.hasAttribute("context"))
            rule->setContext(attr(el, "context"));

        ContextPtr context = cast(Context, out);
        if (context)
            context->addRule(rule);
        else if (cast(Rule, out))
            cast(Rule, out)->addRule(rule);

        walk(el, rule);
    }

    static void visitAnyChar(const QDomElement& el, const BasePtr& out)
    {
        RulePtr rule = AnyCharPtr::create(attr(el, "String"));
        setupRule(el, rule, out);
    }

    static void visitDetectChar(const QDomElement& el, const BasePtr& out)
    {
        RulePtr rule = DetectCharPtr::create(attr(el, "char")[0]);
        setupRule(el, rule, out);
    }

    static void visitDetect2Chars(const QDomElement& el, const BasePtr& out)
    {
        RulePtr rule = Detect2CharPtr::create(attr(el, "char")[0], attr(el, "char1")[0]);
        setupRule(el, rule, out);
    }

    template<typename RuleT>
    static void visitRule(const QDomElement& el, const BasePtr& out)
    {
        RulePtr rule = QSharedPointer<RuleT>::create();
        setupRule(el, rule, out);
    }

    static void visitKeywordRule(const QDomElement& el, const BasePtr& out)
    {
        RulePtr rule = KeywordPtr::create(attr(el, "String"));
        setupRule(el, rule, out);
    }

    static void visitLineContinue(const QDomElement& el, const BasePtr& out)
    {
        QChar ch = el.hasAttribute("char") ? attr(el, "char").at(0) : '\\';
        RulePtr rule = LineContinuePtr::create(ch);
        setupRule(el, rule, out);
    }

    static void visitRangeDetect(const QDomElement& el, const BasePtr& out)
    {
        RulePtr rule = RangeDetectPtr::create(attr(el, "char")[0], attr(el, "char1")[0]);
        setupRule(el, rule, out);
    }

    static void visitRegExpr(const QDomElement& el, const BasePtr& out)
    {
        RulePtr rule = RegExprPtr::create(attr(el, "String"), attr<bool>(el, "insensitive"), attr<bool>(el, "minimal"));
        setupRule(el, rule, out);
    }

    static void visitStringDetect(const QDomElement& el, const BasePtr& out)
    {
        RulePtr rule = StringDetectPtr::create(attr(el, "String"), attr<bool>(el, "insensitive"));
        setupRule(el, rule, out);
    }

    static void visitWordDetect(const QDomElement& el, const BasePtr& out)
    {
        RulePtr rule = WordDetectPtr::create(attr(el, "String"));
        setupRule(el, rule, out);
    }

    static void visitIncludeRules(const QDomElement& el, const BasePtr& out)
    {
        RulePtr rule = IncludeRulesPtr::create(attr<bool>(el, "includeAttrib"));
        setupRule(el, rule, out);
    }

    static void visitItemData(const QDomElement& el, const BasePtr& out)
    {
        DefinitionPtr def = cast(Definition, out);
        ItemDataPtr item = ItemDataPtr::create(attr(el, "name"), attr(el, "defStyleNum"));
        if (el.hasAttribute("color"))
            item->setDefColor(attr(el, "color"));
        if (el.hasAttribute("selColor"))
            item->setDefSelColor(attr(el, "selColor"));
        if (el.hasAttribute("bold"))
            item->setDefBold(attr<bool>(el, "bold"));
        if (el.hasAttribute("italic"))
            item->setDefItalic(attr<bool>(el, "italic"));

        def->addItemData(item);
    }

private:
    template<typename Str>
    static bool is(const QDomElement& el, const Str& str)
    {
        return el.tagName() == str;
    }

    template<typename T>
    static T attr(const QDomElement& el, const QString& name)
    {
        return qnoto::convert<T>(el.attributes().namedItem(name).nodeValue());
    }

    static QString attr(const QDomElement& el, const QString& name)
    {
        return el.attributes().namedItem(name).nodeValue();
    }
};

Parser::Parser(const QString& fname):
    m_fname(fname)
{}

void Parser::parse(DefinitionPtr& def)
{
    QDomDocument doc("syntax");
    QFile file(m_fname);
    if (!file.open(QIODevice::ReadOnly))
        return;

    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    qDebug() << "wlk start";
    Walker::visit(doc.documentElement(), def);
    def->init();
}

}
