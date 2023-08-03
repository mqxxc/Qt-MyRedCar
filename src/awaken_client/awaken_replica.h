#pragma once
#include<qobject.h>

//唤醒主进程
class awakenReplica;
class QRemoteObjectNode;

class Awaken_replica :public QObject
{
	Q_OBJECT

public:
	Awaken_replica(QObject* p = nullptr);
	~Awaken_replica();
	void SendAwaken();


private:
	awakenReplica* m_pReplica;
	QRemoteObjectNode* m_pRemoteNode;
};