#pragma once
//截取视频中的第某帧截图
#include "screenshot_global.h"
#include <QObject>
#include <QtMultimedia/QAbstractVideoSurface>
class QMediaPlayer;
class QEventLoop;


class VideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT

public:
    VideoSurface(QObject* parent = Q_NULLPTR);

    virtual QList<QVideoFrame::PixelFormat>
        supportedPixelFormats
        (QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;
    virtual bool present(const QVideoFrame& frame);

signals:
    void frameAvailable(QVideoFrame frame);
    void finish();
};


class Screenshot : public QObject 
{
public:
    Screenshot();
    virtual ~Screenshot();
    virtual bool IsSuccess();
    virtual void GetJpg(QString path);
    virtual void SetLoop(QEventLoop* loop);
    virtual void Finish();
    virtual void ReleaseScreenshot();


private:
    QMediaPlayer* m_pMediaPlayer;
    VideoSurface* m_pVideoOut;
    QString m_strSavePath;
    bool m_bSuccess;
    QEventLoop* m_pLoop;


private:
    virtual void ProcessFrame(QVideoFrame frame);
};

extern "C" SCREENSHOT_EXPORT Screenshot * GetScreenshot();