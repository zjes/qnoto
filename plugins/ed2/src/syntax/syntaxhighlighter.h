#pragma once

#include <QTextBlock>
#include <QMutex>

class QTextDocument;
class QTextCharFormat;
class QTextBlockUserData;
class Formater;

class TextBlock
{
public:
    TextBlock()
    {}

    TextBlock(const QTextBlock& blk):
        block(blk)
    {}

    void setFormat(int from, int length, const QTextCharFormat& format)
    {
        if (!ranges.isEmpty()){
            auto& last = ranges.last();
            if (last.format == format && last.start + last.length == from)
                ranges.last().length += length;
            else
                ranges.append(QTextLayout::FormatRange{from, length, format});
        } else {
            ranges.append(QTextLayout::FormatRange{from, length, format});
        }
    }

    QTextBlock block;
    QVector<QTextLayout::FormatRange> ranges;
    bool stateChanged = false;
};

class SyntaxHighlighter : public QObject
{
    Q_OBJECT
public:
    SyntaxHighlighter(QTextDocument *parent);
    virtual ~SyntaxHighlighter();

    void setDocument(QTextDocument *doc);
    QTextDocument *document() const;

protected:
    virtual void highlightBlock(TextBlock &block) = 0;

private:
    bool reformatBlock(TextBlock &block);
    bool applyFormatChanges(TextBlock& block);
    void blockProcessed(const TextBlock& block);
    void rehighlight(int from, int charsRemoved, int charsAdded);
private:
    QTextDocument* m_document = nullptr;
    Formater*      m_worker;
    QMutex         m_mut;
};

Q_DECLARE_METATYPE(QSharedPointer<TextBlock>)
