#ifndef STATUSSYNTAXHIGHLIGHTER_H
#define STATUSSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class QQuickTextDocument;
class SpellChecker;

class StatusSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    StatusSyntaxHighlighter(QTextDocument *parent = nullptr);

    SpellChecker* _spellchecker() const;

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat singlelineBoldFormat;
    QTextCharFormat singleLineItalicFormat;
    QTextCharFormat singlelineCodeBlockFormat;
    QTextCharFormat singleLineStrikeThroughFormat;
    QTextCharFormat multiLineCodeBlockFormat;
    QTextCharFormat spellcheckerFormat;
    SpellChecker *spellchecker;
};

class StatusSyntaxHighlighterHelper : public QObject {
  Q_OBJECT
  Q_PROPERTY(QQuickTextDocument *quickTextDocument READ quickTextDocument WRITE
                 setQuickTextDocument NOTIFY quickTextDocumentChanged)
public:
  StatusSyntaxHighlighterHelper(QObject *parent = nullptr)
      : QObject(parent), m_quicktextdocument(nullptr) {}
  QQuickTextDocument *quickTextDocument() const;
  void setQuickTextDocument(QQuickTextDocument *quickTextDocument);

  Q_INVOKABLE QVariantList suggestions(const QString& word);
  Q_INVOKABLE void setSpellcheckingEnable(bool enable);
  Q_INVOKABLE bool spellcheckingEnabled() const;
  Q_INVOKABLE void addToUserDictionary(const QString& word);

signals:
  void quickTextDocumentChanged();

private:
  QQuickTextDocument *m_quicktextdocument;
  StatusSyntaxHighlighter *m_highlighter;
};
#endif // STATUSSYNTAXHIGHLIGHTER_H
