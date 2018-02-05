#include <QTextDocument>
#include <QTextLayout>
#include <QDebug>
#include <QAbstractTextDocumentLayout>
#include "syntaxhighlighter.h"
#include "formater.h"


SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent):
    m_worker(new Formater(this, &SyntaxHighlighter::reformatBlock))
{
    qRegisterMetaType<TextBlock>("TextBlock");

    connect(m_worker, &Formater::finished, m_worker, &QObject::deleteLater);
    connect(m_worker, &Formater::processed, this, &SyntaxHighlighter::blockProcessed);

    m_worker->start();
    setDocument(parent);
}

SyntaxHighlighter::~SyntaxHighlighter()
{
    m_worker->stop();
    //setDocument(nullptr);
}

void SyntaxHighlighter::setDocument(QTextDocument* doc)
{
    if (m_document) {
        m_document->disconnect(this);

        QTextCursor cursor(m_document);
        cursor.beginEditBlock();
        for (QTextBlock blk = m_document->begin(); blk.isValid(); blk = blk.next())
            blk.layout()->clearFormats();
        cursor.endEditBlock();
    }

    if (!doc)
        return;

    m_document = doc;
    connect(m_document, QOverload<int, int, int>::of(&QTextDocument::contentsChange), this, &SyntaxHighlighter::rehighlight);
}

QTextDocument* SyntaxHighlighter::document() const
{
    return m_document;
}

void SyntaxHighlighter::rehighlight(int from, int charsRemoved, int charsAdded)
{
    if (!m_document)
        return;

    QTextBlock block = m_document->findBlock(from);
    if (!block.isValid())
        return;

    QTextBlock lastBlock = m_document->findBlock(from + charsAdded + (charsRemoved > 0 ? 1 : 0));

    int endPosition = lastBlock.isValid() ? lastBlock.position() + lastBlock.length() : m_document->lastBlock().position();
    if (!endPosition)
        return;

    for(; block.isValid() && block.position() < endPosition; block = block.next()){
        m_worker->add(block);
    }
}

bool SyntaxHighlighter::reformatBlock(TextBlock& block)
{
    highlightBlock(block);
    return applyFormatChanges(block);
}

bool SyntaxHighlighter::applyFormatChanges(TextBlock& block)
{
    bool formatsChanged = false;

    QTextLayout *layout = block.block.layout();

    const int preeditAreaStart = layout->preeditAreaPosition();
    const int preeditAreaLength = layout->preeditAreaText().length();

    if (preeditAreaLength != 0) {
        auto removed = std::remove_if(block.ranges.begin(), block.ranges.end(), [&](const QTextLayout::FormatRange &range) {
            return range.start < preeditAreaStart || range.start + range.length > preeditAreaStart + preeditAreaLength;
        });

        if (removed != block.ranges.end()) {
            block.ranges.erase(removed, block.ranges.end());
            formatsChanged = true;
        }
    } else if (!block.ranges.empty()) {
        formatsChanged = true;
    }

    return formatsChanged || block.stateChanged;
}

void SyntaxHighlighter::blockProcessed(const TextBlock& block)
{
    QMutexLocker locker(&m_mut);
    block.block.layout()->setFormats(block.ranges);
    if (block.stateChanged && block.block.next().isValid()){
        m_worker->add(block.block.next());
    }
    m_document->markContentsDirty(block.block.position(), block.block.length());
    m_document->documentLayout()->updateBlock(block.block);
}
