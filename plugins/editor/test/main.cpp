#include <QTest>
#include <QDebug>
#include "src/syntax/rules/anychar.h"
#include "src/syntax/rules/detectspaces.h"
#include "src/syntax/rules/float.h"
#include "src/syntax/rules/hlcchar.h"
#include "src/syntax/rules/hlchex.h"

class RulesTest: public QObject
{
    Q_OBJECT
private slots:
    void checkAnychar()
    {
        syntax::AnyChar a("abs");
        syntax::MatchResult res = a.match("acb", 0, {});
        QCOMPARE(res.offset, 1);
        res = a.match("tacb", 0, {});
        QCOMPARE(res.offset, 0);
    }

    void checkSpaces()
    {
        syntax::DetectSpaces a;
        syntax::MatchResult res = a.match("   acb", 0, {});
        QCOMPARE(res.offset, 3);
        res = a.match("    acb", 0, {});
        QCOMPARE(res.offset, 4);
        res = a.match("acb", 0, {});
        QCOMPARE(res.offset, 0);
    }

    void checkFloat()
    {
        syntax::Float fl;
        auto check = [&fl](const char* txt, int off){
            syntax::MatchResult res = fl.match(txt, 0, {});
            QVERIFY2(res.offset == off, txt);
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
        syntax::MatchResult res = a.match("'a'", 0, {});
        QCOMPARE(res.offset, 3);
        res = a.match("'\\t'", 0, {});
        QCOMPARE(res.offset, 4);
    }

    void checkHlHex()
    {
        syntax::HlCHex a;
        syntax::MatchResult res = a.match("0x11", 0, {});
        QCOMPARE(res.offset, 4);
        res = a.match("0xa1557f", 0, {});
        QCOMPARE(res.offset, 8);
        res = a.match("0xa1557h", 0, {});
        QCOMPARE(res.offset, 7);
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
