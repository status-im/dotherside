#include "DOtherSide/DOtherSideStatusSyntaxHighlighter.h"

#include <QQuickTextDocument>
#include <QPalette>
#include <QGuiApplication>

#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Repository>
#include <KSyntaxHighlighting/Theme>

StatusSyntaxHighlighter::StatusSyntaxHighlighter(QTextDocument *parent)
    : KSyntaxHighlighting::SyntaxHighlighter(parent)
{
    KSyntaxHighlighting::Repository repository;
    setTheme(repository.themeForPalette(qGuiApp->palette()));
    setDefinition(repository.definitionForName(QStringLiteral("Markdown")));
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
