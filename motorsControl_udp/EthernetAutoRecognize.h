#ifndef AUTORECOGINZE_H
#define AUTORECOGINZE_H
#include "AbstractAutoRecongnize.h"
#include <QObject>
#include <QMap>
#include <QHostAddress>
#include <QUdpSocket>

class EthernetAutoRecognize : public AbstractAutoRecongnize
{
    Q_OBJECT
public:
    explicit EthernetAutoRecognize(QObject *parent = 0);
    void startRecognize()override;
    void waitTimeout()override;
protected:

    void findCommunicationUnits();
signals:

public slots:

private slots:
    void onIpBroadcast();
private:

private:
    bool m_bFindAvaliable;
    QUdpSocket * m_pSocket;
    QVector<QHostAddress> m_addrVector;
};

#endif // AUTORECOGINZE_H
