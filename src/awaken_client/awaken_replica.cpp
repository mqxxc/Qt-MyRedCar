#include "awaken_replica.h"
#include "rep_awaken_replica.h"

Awaken_replica::Awaken_replica(QObject* p) :QObject(p) 
{
	m_pRemoteNode = new QRemoteObjectNode(this);
	m_pRemoteNode->connectToNode(QUrl("local:qtmyredcar"));
	m_pReplica = m_pRemoteNode->acquire<awakenReplica>();
}

Awaken_replica::~Awaken_replica()
{
	if (nullptr != m_pReplica)
	{
		delete m_pReplica;
	}
}

void Awaken_replica::SendAwaken()
{
	m_pReplica->awaken();
}