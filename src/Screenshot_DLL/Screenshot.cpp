#include <QtMultimedia/QMediaPlayer>
#include <QDebug>
#include <QEventLoop>
#include <QUrl>
#include <QMetaType>
#include <QtMultimedia/QVideoSurfaceFormat>
#include "Screenshot.h"
#pragma execution_character_set("utf-8")

//VideoSurface class
VideoSurface::VideoSurface(QObject* parent) :QAbstractVideoSurface(parent) 
{

}

QList<QVideoFrame::PixelFormat> VideoSurface::
supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const 
{

    QList<QVideoFrame::PixelFormat> listPixelFormats;

    listPixelFormats << QVideoFrame::Format_ARGB32
        << QVideoFrame::Format_ARGB32_Premultiplied
        << QVideoFrame::Format_RGB32
        << QVideoFrame::Format_RGB24
        << QVideoFrame::Format_RGB565
        << QVideoFrame::Format_RGB555
        << QVideoFrame::Format_ARGB8565_Premultiplied
        << QVideoFrame::Format_BGRA32
        << QVideoFrame::Format_BGRA32_Premultiplied
        << QVideoFrame::Format_BGR32
        << QVideoFrame::Format_BGR24
        << QVideoFrame::Format_BGR565
        << QVideoFrame::Format_BGR555
        << QVideoFrame::Format_BGRA5658_Premultiplied
        << QVideoFrame::Format_AYUV444
        << QVideoFrame::Format_AYUV444_Premultiplied
        << QVideoFrame::Format_YUV444
        << QVideoFrame::Format_YUV420P
        << QVideoFrame::Format_YV12
        << QVideoFrame::Format_UYVY
        << QVideoFrame::Format_YUYV
        << QVideoFrame::Format_NV12
        << QVideoFrame::Format_NV21
        << QVideoFrame::Format_IMC1
        << QVideoFrame::Format_IMC2
        << QVideoFrame::Format_IMC3
        << QVideoFrame::Format_IMC4
        << QVideoFrame::Format_Y8
        << QVideoFrame::Format_Y16
        << QVideoFrame::Format_Jpeg
        << QVideoFrame::Format_CameraRaw
        << QVideoFrame::Format_AdobeDng;
    return listPixelFormats;
}

bool VideoSurface::present(const QVideoFrame& frame) 
{
    if (frame.isValid()) 
    {
        emit frameAvailable(frame);
        return true;
    }
    else 
    {
        emit finish();
        return false;
    }
}

//Screenshot class
Screenshot::Screenshot() 
{
    qRegisterMetaType<QVideoFrame>("QVideoFrame&");
    m_pMediaPlayer = new QMediaPlayer;
    m_pVideoOut = new VideoSurface;
    m_bSuccess = false;
    m_pMediaPlayer->setVideoOutput(m_pVideoOut);
    m_pMediaPlayer->setMuted(true);
    m_pLoop = nullptr;
    connect(m_pVideoOut, &VideoSurface::frameAvailable,
        this, &Screenshot::ProcessFrame, Qt::QueuedConnection);
    connect(m_pVideoOut, &VideoSurface::finish,
        this, &Screenshot::Finish, Qt::QueuedConnection);
}

Screenshot::~Screenshot() 
{
    m_pMediaPlayer->stop();
    m_pMediaPlayer->setMedia(nullptr);
    delete m_pMediaPlayer;
    delete m_pVideoOut;
}

bool Screenshot::IsSuccess() 
{
    return m_bSuccess;
}

void Screenshot::ProcessFrame(QVideoFrame frame) 
{
    static int count = 0;
    ++count;
    if (count == 30) {
        m_pMediaPlayer->stop();
        frame.map(QAbstractVideoBuffer::ReadOnly);
        int w = frame.width();
        int h = frame.height();
        QImage recvImage(frame.bits(), w, h, frame.bytesPerLine(),
            QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat()));
        recvImage = recvImage.copy((w - h) / 2, 0, h, h);
        recvImage.save(m_strSavePath.arg(count), "JPG");
        frame.unmap();
        m_bSuccess = true;
        Finish();
    }
}

void Screenshot::GetJpg(QString path) 
{
    m_pMediaPlayer->setMedia(QUrl(path));
    m_strSavePath = path.left(path.lastIndexOf(".")).append(".jpg");
    m_pMediaPlayer->play();
}

void Screenshot::SetLoop(QEventLoop* loop)
{
    this->m_pLoop = loop;
}

void Screenshot::Finish()
{
    m_pLoop->exit();
}

void Screenshot::ReleaseScreenshot()
{
    delete this;
}

SCREENSHOT_EXPORT Screenshot* GetScreenshot()
{
    return new Screenshot();
}
