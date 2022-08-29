#include "DOtherSide/DOtherSideStatusSyntaxHighlighter.h"

#include <QQuickTextDocument>
#include <QPalette>
#include <QGuiApplication>

#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Theme>

StatusSyntaxHighlighter::StatusSyntaxHighlighter(QTextDocument *parent)
    : KSyntaxHighlighting::SyntaxHighlighter(parent)
    , m_repository(new KSyntaxHighlighting::Repository())
{
  setDefinition(m_repository->definitionForName(QStringLiteral("Markdown")));
  setTheme(m_repository->themeForPalette(qApp->palette()));
}

QQuickTextDocument *StatusSyntaxHighlighterHelper::quickTextDocument() const
{
    return m_quicktextdocument;
}

void StatusSyntaxHighlighterHelper::setQuickTextDocument(QQuickTextDocument *quickTextDocument)
{
    m_quicktextdocument = quickTextDocument;
    if (m_quicktextdocument) {
        new StatusSyntaxHighlighter(m_quicktextdocument->textDocument());
    }
}
