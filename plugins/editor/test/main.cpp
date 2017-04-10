#include <QTest>
#include <QDebug>
#include "src/syntax/rules/anychar.h"
#include "src/syntax/rules/detectspaces.h"
#include "src/syntax/rules/float.h"
#include "src/syntax/rules/hlcchar.h"
#include "src/syntax/rules/hlchex.h"
#include "src/syntax/rules/detectidentifier.h"
#include "src/syntax/rules/regexpr.h"

class RulesTest: public QObject
{
    Q_OBJECT
private slots:
    void checkAnychar()
    {
        syntax::AnyChar a("abs");
        int offset = a.match("acb", 0);
        QCOMPARE(offset, 1);
        offset = a.match("tacb", 0);
        QCOMPARE(offset, 0);
    }

    void checkSpaces()
    {
        syntax::DetectSpaces a;
        int offset = a.match("   acb", 0);
        QCOMPARE(offset, 3);
        offset = a.match("    acb", 0);
        QCOMPARE(offset, 4);
        offset = a.match("acb", 0);
        QCOMPARE(offset, 0);
    }

    void checkFloat()
    {
        syntax::Float fl;
        auto check = [&fl](const char* txt, int off){
            int offset = fl.match(txt, 0);
            QVERIFY2(offset == off, txt);
        };
        check("1.0", 3);
        check("1.", 2);
        check(".1", 2);
        check("3.14", 4);
        check("1.e-10", 6);
        check("1.1e-10", 7);
        check(".1f", 3);
        check("aaa.bbb", 0);
    }

    void checkHlChar()
    {
        syntax::HlCChar a;
        int offset = a.match("'a'", 0);
        QCOMPARE(offset, 3);
        offset = a.match("'\\t'", 0);
        QCOMPARE(offset, 4);
    }

    void checkHlHex()
    {
        syntax::HlCHex a;
        int offset = a.match("0x11", 0);
        QCOMPARE(offset, 4);
        offset = a.match("0xa1557f", 0);
        QCOMPARE(offset, 8);
        offset = a.match("0xa1557h", 0);
        QCOMPARE(offset, 7);
    }

    void checkIdentifier()
    {
        syntax::DetectIdentifier a;
        int offset = a.match("aaaa", 0);
        QCOMPARE(offset, 4);
        offset = a.match("aa_bb", 0);
        QCOMPARE(offset, 5);
        offset = a.match("ad1 aasd", 0);
        QCOMPARE(offset, 3);
        offset = a.match("</aaaa>", 0);
        QCOMPARE(offset, 0);
    }

    void ceckRegexp()
    {
        syntax::RegExpr a("(#|%\\:|\\?\\?=)\\s*(?:el(?:se|if)|(cmake)?define|undef|line|error|warning|pragma)", false, true);
        int offset = a.match("#pragma once", 0);
        QCOMPARE(offset, 7);
    }

};

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);


    RulesTest rt;

    QTest::qExec(&rt, argc, argv);
    return 0;
}

#include "main.moc"
