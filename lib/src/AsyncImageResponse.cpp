#include <QtCore/QDebug>
#include <QtGui/QImage>
#include <QtCore/QThreadPool>
#include <QtCore/QObject>
#include <QtQuick/QQuickAsyncImageProvider>
#include <QtNetwork/QNetworkReply>
#include "DOtherSide/AsyncImageResponse.h"
#include <QtNetwork/QNetworkDiskCache>


AsyncImageResponse::AsyncImageResponse(QNetworkRequest req, QSize const& reqSize, QString const& cacheDir)
{
    m_imageLoader = new QNetworkAccessManager(); // TODO: should come from somewhere
    QNetworkDiskCache* cache = new QNetworkDiskCache(m_imageLoader);
    cache->setCacheDirectory(cacheDir);
    m_imageLoader->setCache(cache);
    m_reply = m_imageLoader->get(req);
    m_requestedSize = reqSize;
    connect(m_reply, &QNetworkReply::finished, this, &AsyncImageResponse::onResponseFinished);
}

void AsyncImageResponse::onResponseFinished()
{
    QByteArray myImageData = m_reply->readAll();
    m_resultImage = QImage::fromData(myImageData);
    if (m_requestedSize.isValid())
    {
        m_resultImage = m_resultImage.scaled(m_requestedSize);
    }
    emit finished();
}

QQuickTextureFactory *AsyncImageResponse::textureFactory() const
{
    return QQuickTextureFactory::textureFactoryForImage(m_resultImage);
}