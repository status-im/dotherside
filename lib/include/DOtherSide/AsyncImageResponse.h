#include <QtCore/QDebug>
#include <QtGui/QImage>
#include <QtCore/QThreadPool>
#include <QtCore/QObject>
#include <QtQuick/QQuickAsyncImageProvider>
#include <QtNetwork/QNetworkReply>

class AsyncImageResponse : public QQuickImageResponse
{
    Q_OBJECT
public:
    // QQuickImageResponse interface
public:
    explicit AsyncImageResponse(QNetworkRequest req, QSize const& requestedSize, QString const& cacheDir);
    QQuickTextureFactory *textureFactory() const override;

public slots:
    void onResponseFinished();

protected:
    QNetworkAccessManager* m_imageLoader;
    QNetworkReply* m_reply;
    QSize m_requestedSize;
    QImage m_resultImage;
};