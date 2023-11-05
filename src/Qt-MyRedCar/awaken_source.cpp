#include "awaken_source.h"
#include<QtRemoteObjects/QRemoteObjectNode>
#include <QtRemoteObjects/QRemoteObjectHost>

void Awaken_source::awaken()
{
	emit AwakenSig();
}


//Server
Server::Server(QObject* parent) : QObject(parent)
{
    m_pHost = new QRemoteObjectHost(this);
    m_pHost->setHostUrl(QUrl("local:qtmyredcar"));
    m_pInterface = new Awaken_source();
    m_pHost->enableRemoting(m_pInterface);

    connect(m_pInterface, &Awaken_source::AwakenSig, this, &Server::SendAwaken);
}

Server::~Server()
{
    delete m_pInterface;
    delete m_pHost;
}

void Server::SendAwaken()
{
    emit Awaken();
}