#pragma once

#include <KSyntaxHighlighting/SyntaxHighlighter>

class QQuickTextDocument;

class StatusSyntaxHighlighter : public KSyntaxHighlighting::SyntaxHighlighter
{
    Q_OBJECT

public:
    StatusSyntaxHighlighter(QTextDocument *parent = nullptr);
};

class StatusSyntaxHighlighterHelper : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QQuickTextDocument *quickTextDocument READ quickTextDocument WRITE
                 setQuickTextDocument NOTIFY quickTextDocumentChanged)
public:
  StatusSyntaxHighlighterHelper(QObject *parent = nullptr)
      : QObject(parent) {}
  QQuickTextDocument *quickTextDocument() const;
  void setQuickTextDocument(QQuickTextDocument *quickTextDocument);

signals:
  void quickTextDocumentChanged();

private:
  QQuickTextDocument *m_quicktextdocument{nullptr};
};
