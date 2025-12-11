#ifndef STATUS_URL_SCHEME_EVENT_H
#define STATUS_URL_SCHEME_EVENT_H

#include <QObject>

namespace Status
{
    class UrlSchemeEvent : public QObject
    {
        Q_OBJECT

        public:
            void emitDeepLinkToQt(const QString& url);
            static void setInstance(UrlSchemeEvent* instance);

        protected:
            bool eventFilter(QObject* obj, QEvent* event) override;

        signals:
            void urlActivated(const QString& url);
    };
}

#endif