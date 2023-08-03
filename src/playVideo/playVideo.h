#pragma once
//播放视频
#include "playvideo_global.h"
#include<QObject>
class QWidget;
class QMediaPlayer;
class QVideoWidget;
class QMediaPlaylist;

class PLAYVIDEO_EXPORT VideoPalyer:public QObject
{
    Q_OBJECT

public:
    VideoPalyer(QWidget* parent);
    virtual~VideoPalyer();

private:
    QMediaPlayer* m_pPlayer;                       //播放器
    QVideoWidget* m_pVideoWnd;                     //播放窗口
    QMediaPlaylist* m_pPlayList;                   //播放列表

public:
    virtual void SetVideo(QString strVideoPath);    //设置要播放的媒体
    virtual void PlayVideo();                       //播放视频
    virtual void SetVolume(int nVolume);            //设置音量
    virtual void StopPlay();                        //停止播放并删除媒体缓存
    virtual void PauseVideo();                      //暂停播放
    virtual void StartVideo();                      //继续播放

private slots:
    void Getduration(qint64 playtime);              //获取播放时长
    void ReFreshLoop();                             //刷新视频播放位置
};

extern "C" PLAYVIDEO_EXPORT VideoPalyer* GetpPlayVideo(QWidget * parent);
