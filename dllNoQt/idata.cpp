#include "idata.h"

quint8 IData::deviceId = 0;
 IData::IData():
     m_nIndex(0)
 {

 }

 IData::IData(const QByteArray &buf) :
     m_nIndex(0),
     m_buffer(buf)
 {

 }

 qint16 IData::ReadShort()
 {
     if (m_nIndex+2 > m_buffer.size())
     {
         return (qint16)0xffff;
     }
     qint16 crc2 = ((m_buffer.at(m_nIndex++) << 8) & 0xff00);
     crc2 += ((m_buffer.at(m_nIndex++)) & 0xff);
     return crc2;
 }

 quint16 IData::ReadUShort()
 {
     if (m_nIndex+2 > m_buffer.size())
     {
         return (quint16)0xffff;
     }
     quint16 crc2 = ((m_buffer.at(m_nIndex++) << 8) & 0xff00);
     crc2 += ((m_buffer.at(m_nIndex++)) & 0xff);
     return crc2;
 }

 qint32 IData::ReadInt()
 {
     if (m_nIndex + 4 > m_buffer.size())
     {
         return 0xffffffff;
     }
     uint nTmp = 0;
     for (int j = 0; j < 4; ++j)
     {
         nTmp += (((m_buffer.at(m_nIndex++) << (24 - j * 8))) & (0xff << (24 - j * 8)));
     }
     qint32 nTmp2 = nTmp;
     return nTmp2;
 }

 qint8 IData::ReadByte()
 {
     if (m_nIndex + 1 > m_buffer.size())
     {
         return (qint8)0xff;
     }
     qint8 nTmp = m_buffer.at(m_nIndex++);
     return nTmp;
 }

 quint8 IData::ReadUByte()
 {
     if (m_nIndex + 1 > m_buffer.size())
     {
         return 0xff;
     }
     quint8 nTmp = m_buffer.at(m_nIndex++);
     return nTmp;
 }

 void IData::WriteByte(quint8 c)
 {
     //quint8 nTmp = c & 0xf;
     m_buffer.append(c);
 }

 void IData::WriteShort(qint16 content)
 {
     quint8 nTmp = ((content >> 8) & 0xff);
     m_buffer.append(nTmp);
     nTmp = content & 0xff;
     m_buffer.append(nTmp);
 }

 void IData::WriteInt(qint32 content)
 {
     for (int j = 0; j < 4; ++j)
     {
         quint8 nTmp = ((content >> (24 - j * 8)) & 0xff);
         m_buffer.append(nTmp);
     }
 }

 bool IData::Skip(qint32 skip)
 {
     qint32 nTmp = m_nIndex + skip;
     if (nTmp < 0 || nTmp > m_buffer.size()-1)
     {
         return false;
     }
     m_nIndex = nTmp;
     return true;
 }

void IData::AddProxyEnd()
{
    m_buffer.append((char)0xED);
    //buf.append('\n');
}

void IData::AddProxyBegin(const quint8 nMode, const quint8 nDeviceId)
{
    m_buffer.append((char)0xEE);
    m_buffer.append(nDeviceId);
    m_buffer.append(nMode);
}

bool IData::CheckData()
{
    QByteArray buf = m_buffer;
    if (buf.size() < 6)
    {
        return false;
    }
    quint8 nBeign = buf.at(0);
    quint8 nEnd = buf.at(buf.size() - 1);
    if (nBeign != 0xEE || nEnd != 0xED)
    {
        return false;
    }
    char * content = const_cast<char *>(buf.data());
    content += 5;
    quint16 crc = CRC16_1((uchar *)content, buf.size() - 8);
    quint16 crc2 = ((buf.at(buf.size()-3) << 8) & 0xff00);
    crc2 += ((buf.at(buf.size()-2)) & 0xff);
    if (crc != crc2)
    {
        return false;
    }
    return true;
}

void IData::AddCRCCode(qint16 nLen)
{
    if (m_buffer.size() > 5)
    {
        char * charContent = m_buffer.data();
        charContent += 5;//跳过帧前面部分的固定数据
        quint16 crc = CRC16_1((uchar *)charContent, nLen);
        quint8 nTmp = ((crc >> 8) & 0xff);
        m_buffer.append(nTmp);
        nTmp = crc & 0x00ff;
        m_buffer.append(nTmp);
    }

}

//template<class T>
//void IData::WriteData(IData::T data)
//{
//    int nLen = sizeof(data);
//    switch (nLen) {
//    case 1:
//        WriteByte(data);
//        break;
//    case 2:
//        WriteShort(data);
//        break;
//    case 4:
//        WriteInt(data);
//    default:
//        break;
//    }
//}
