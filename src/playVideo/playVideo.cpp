#include "playVideo.h"
#include<QWidget>
#include<QtMultimedia/QMediaPlayer>
#include<QtMultimediaWidgets/QVideoWidget>
#include<QtMultimedia/QMediaPlaylist>

VideoPalyer::VideoPalyer(QWidget* parent)
{
	m_pPlayer = new QMediaPlayer();
	m_pVideoWnd = new QVideoWidget(parent);
	m_pVideoWnd->resize(parent->size());
	m_pVideoWnd->show();
	m_pPlayList = new QMediaPlaylist();
	m_pPlayer->setPlaylist(m_pPlayList);
	m_pPlayer->setVideoOutput(m_pVideoWnd);
	m_pPlayer->duration();
	connect(m_pPlayer, &QMediaPlayer::positionChanged, this, &VideoPalyer::Getduration);
}

VideoPalyer::~VideoPalyer()
{
	StopPlay();
	delete m_pPlayer;
	delete m_pVideoWnd;
	delete m_pPlayList;
}


void VideoPalyer::SetVideo(QString vpath)
{
	m_pPlayList->clear();
	m_pPlayList->addMedia(QUrl(vpath));
}

void VideoPalyer::PlayVideo()
{
	m_pPlayer->play();
}

void VideoPalyer::SetVolume(int volume)
{
	m_pPlayer->setVolume(volume);
}

void VideoPalyer::StopPlay()
{
	m_pPlayer->stop();
	m_pPlayList->clear();
}

void VideoPalyer::PauseVideo()
{
	m_pPlayer->pause();
}

void VideoPalyer::StartVideo()
{
	m_pPlayer->play();
}

void VideoPalyer::ReFreshLoop()
{
	m_pPlayer->setPosition(0);
}

void VideoPalyer::Getduration(qint64 playtime) 
{
	if (playtime>m_pPlayer->duration()-1000) 
	{
		m_pPlayer->setPosition(0);
	}
}

VideoPalyer* GetpPlayVideo(QWidget* parent)
{
	return new VideoPalyer(parent);
}