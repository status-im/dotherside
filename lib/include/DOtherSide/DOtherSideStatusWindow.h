#ifndef STATUSWINDOW_H
#define STATUSWINDOW_H

#include <QQuickWindow>

class QLocalServer;

class StatusWindow: public QQuickWindow
{
    Q_OBJECT

    Q_PROPERTY(bool isFullScreen READ isFullScreen NOTIFY isFullScreenChanged)

public:

    explicit StatusWindow(QWindow *parent = nullptr);
    ~StatusWindow();

    Q_INVOKABLE void toggleFullScreen();

    bool isFullScreen() const;

signals:
    void isFullScreenChanged();
    void secondInstanceDetected();

private:
    void checkSingleInstance();
    void removeTitleBar();
    void showTitleBar();
    void initCallbacks();

private:
    bool m_isFullScreen;
    QLocalServer *m_localServer;
};

#endif // STATUSWINDOW_H
