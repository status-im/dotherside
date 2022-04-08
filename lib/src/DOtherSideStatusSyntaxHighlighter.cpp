#include "DOtherSide/DOtherSideStatusSyntaxHighlighter.h"

#include "DOtherSide/DosSpellchecker.h"

#include <QQuickTextDocument>
#include <QGuiApplication>
#include <QInputMethod>

StatusSyntaxHighlighter::StatusSyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
    , spellchecker(new SpellChecker(this))
{
    HighlightingRule rule;

    auto inputMethod = QGuiApplication::inputMethod();
    connect(inputMethod, &QInputMethod::localeChanged, [&](){
        spellchecker->setLang(QGuiApplication::inputMethod()->locale().bcp47Name());
    });

    spellchecker->setLang(inputMethod->locale().bcp47Name());
// SPELLCHECKER
    spellcheckerFormat.setBackground(QBrush("#40ff0000"));
// SPELLCHECKER

//BOLD
    singlelineBoldFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral("\\*\\*(.*?)\\*\\*"));
    rule.format = singlelineBoldFormat;
    highlightingRules.append(rule);
//BOLD

//ITALIC
    singleLineItalicFormat.setFontItalic(true);
    rule.pattern = QRegularExpression(QStringLiteral("\\*(.*?)\\*"));
    rule.format = singleLineItalicFormat;
    highlightingRules.append(rule);
//ITALIC

//CODE
    singlelineCodeBlockFormat.setFontFamily("Roboto Mono");
    rule.pattern = QRegularExpression(QStringLiteral("\\`(.*?)\\`"));
    rule.format = singlelineCodeBlockFormat;
    highlightingRules.append(rule);
//CODE

//STRIKETHROUGH
    singleLineStrikeThroughFormat.setFontStrikeOut(true);
    rule.pattern = QRegularExpression(QStringLiteral("\\~+(.*?)\\~+"));
    rule.format = singleLineStrikeThroughFormat;
    highlightingRules.append(rule);
//STRIKETHROUGH

//CODE BLOCK
    multiLineCodeBlockFormat.setFontFamily("Roboto Mono");
    rule.pattern = QRegularExpression(QStringLiteral("\\`\\`\\`(.*?)\\`\\`\\`"));
    rule.format = multiLineCodeBlockFormat;
    highlightingRules.append(rule);
//CODE BLOCK
}

SpellChecker* StatusSyntaxHighlighter::_spellchecker() const
{
    return spellchecker;
}

QVariantList StatusSyntaxHighlighterHelper::suggestions(const QString& word)
{
    return m_highlighter->_spellchecker()->suggest(word);
}

void StatusSyntaxHighlighterHelper::setSpellcheckingEnable(bool enable)
{
    m_highlighter->_spellchecker()->setEnabled(enable);
    m_highlighter->rehighlight();
}

void StatusSyntaxHighlighterHelper::addToUserDictionary(const QString& word)
{
    m_highlighter->_spellchecker()->addToUserWordlist(word);
    m_highlighter->rehighlight();
}

bool StatusSyntaxHighlighterHelper::spellcheckingEnabled() const
{
    return m_highlighter->_spellchecker()->enabled();
}

void StatusSyntaxHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    QRegularExpression expression("\\S+");
    QRegularExpressionMatchIterator i = expression.globalMatch(text);
    while(i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        if (!spellchecker->spell(match.captured())) {
            setFormat(match.capturedStart(), match.capturedLength(), spellcheckerFormat);
        }
    }
    setCurrentBlockState(0);
}

QQuickTextDocument *StatusSyntaxHighlighterHelper::quickTextDocument() const {
    return m_quicktextdocument;
}

void StatusSyntaxHighlighterHelper::setQuickTextDocument(
        QQuickTextDocument *quickTextDocument) {
    m_quicktextdocument = quickTextDocument;
    if (m_quicktextdocument) {
       m_highlighter = new StatusSyntaxHighlighter(m_quicktextdocument->textDocument());
    }
}
