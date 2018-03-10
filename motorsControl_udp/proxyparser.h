#ifndef PROXYPARSER_H
#define PROXYPARSER_H
/**
 *@brief:buffer data recive from seriport,and check data available,then send to innfosproxy to decode
 *@author:lzj
 *@date: 2017/02/28
**/
#include <QObject>
#include "idata.h"

class ProxyParser : public QObject
{
    Q_OBJECT
public:
    static ProxyParser * getInstance();
    void parse(quint8 communicateUnitId,const QByteArray & buf);
protected:
    explicit ProxyParser(QObject *parent = 0);
    void handleError();//data error
    bool headCheck(const QByteArray & data);
    bool dataCheck(const QByteArray & data);
    void dispatchData(quint8 communicateUnitId,QByteArray & buf);
signals:
private:
    class GC{
    public:
        ~GC()
        {
            if(m_pParser!=nullptr)
            {
                delete m_pParser;
                m_pParser = nullptr;
            }
        }
        static GC gc;
    };

public slots:
private:
    QByteArray m_remainData;//data wait for parse;
    static ProxyParser * m_pParser;
};

#endif // PROXYPARSER_H
