#ifndef VCODEBLOCKHIGHLIGHTHELPER_H
#define VCODEBLOCKHIGHLIGHTHELPER_H

#include <QObject>
#include <QVector>
#include <QAtomicInteger>
#include <QXmlStreamReader>
#include <QHash>
#include "vconfigmanager.h"

class VDocument;

class VCodeBlockHighlightHelper : public QObject
{
    Q_OBJECT
public:
    VCodeBlockHighlightHelper(HGMarkdownHighlighter *p_highlighter,
                              VDocument *p_vdoc, MarkdownConverterType p_type);

    // @p_text: text of fenced code block.
    // Get the indent level of the first line (fence) and unindent the whole block
    // to make the fence at the highest indent level.
    // This operation is to make sure JS could handle the code block correctly
    // without any context.
    static QString unindentCodeBlock(const QString &p_text);

private slots:
    void handleCodeBlocksUpdated(const QVector<VCodeBlock> &p_codeBlocks);

    void handleTextHighlightResult(const QString &p_html, int p_id, int p_timeStamp);

private:
    struct HLResult
    {
        HLResult() : m_timeStamp(-1)
        {
        }

        HLResult(int p_timeStamp, const QVector<HLUnitPos> &p_units)
            : m_timeStamp(p_timeStamp), m_units(p_units)
        {
        }

        int m_timeStamp;
        QVector<HLUnitPos> m_units;
    };

    void parseHighlightResult(int p_timeStamp, int p_idx, const QString &p_html);

    // @p_text: the raw text of the code block;
    // @p_index: the start index of the span element within @p_text;
    // @p_units: all the highlight units of this code block;
    bool parseSpanElement(QXmlStreamReader &p_xml,
                          const QString &p_text, int &p_index,
                          QVector<HLUnitPos> &p_units);

    void updateHighlightResults(int p_startPos, QVector<HLUnitPos> p_units);

    void addToHighlightCache(const QString &p_text,
                             int p_timeStamp,
                             const QVector<HLUnitPos> &p_units);

    HGMarkdownHighlighter *m_highlighter;
    VDocument *m_vdocument;
    MarkdownConverterType m_type;
    QAtomicInteger<int> m_timeStamp;
    QVector<VCodeBlock> m_codeBlocks;

    // Cache for highlight result, using the code block text as key.
    // The HLResult has relative position only.
    QHash<QString, HLResult> m_cache;
};

#endif // VCODEBLOCKHIGHLIGHTHELPER_H
