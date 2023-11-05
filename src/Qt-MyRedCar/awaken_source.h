#pragma once
//Awaken_source服务与客户端链接实现类,Server服务端类
#include"rep_awaken_source.h"

//class QRemoteObjectNode;
class QRemoteObjectHost;


class Awaken_source :public awakenSource 
{
	Q_OBJECT

public:
	virtual void awaken();          //唤醒时发射唤醒信号


signals:
	void AwakenSig();              //唤醒信号
};


class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QObject* parent = nullptr);
    ~Server();


private:
    Awaken_source* m_pInterface ;
    QRemoteObjectHost* m_pHost;


signals:
    void Awaken();                  //唤醒信号


private slots:
    void SendAwaken();              //发送唤醒信号
};