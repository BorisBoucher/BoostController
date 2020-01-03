#ifndef SNXCOMM_H
#define SNXCOMM_H

#include <qstring.h>
#include <QtSerialPort/qserialport.h>
#include <QTimer>
#include <QByteArray>
#include <QtEndian>


class SnxComm : public QObject
{
    Q_OBJECT


    QSerialPort*    mSerialPort = nullptr;

    QTimer          mTimer;

    enum Command
    {
        READ_BYTE = 0,
        WRITE_BYTE = 1,
        READ_FLOAT = 2,
        WRITE_FLOAT = 3,
        ADDR_ERROR = 4
    };

    enum State
    {
        READY,
        WAIT_BYTE,
        WAIT_FLOAT
    };
    State mState = READY;

    uint16_t mWaitingAddr;

    QByteArray  mReceiveBuffer;

#pragma pack(push, 1)
    struct MsgHeader
    {
        uint8_t     mBeginMarker;
        uint8_t     mCommand;
        uint16_t    mAddr;
    };

    struct MsgRead : public MsgHeader
    {
        uint8_t     mEndMarker;
    };

    using MsgAddrError = MsgRead;

    struct MsgWriteByte : public MsgHeader
    {
        uint8_t mValue;
        uint8_t mEndMarker;
    };

    struct MsgWriteFloat : public MsgHeader
    {
        float mValue;
        uint8_t mEndMarker;
    };

#pragma pack(pop)

public:
    SnxComm(QObject* parent);
    ~SnxComm();

	bool open(const QString& portName);
    void close();

    void readByte(uint16_t addr);
    void writeByte(uint16_t addr, uint8_t value);
    void readFloat(uint16_t addr);
    void writeFloat(uint16_t addr, float value);

private slots:
    void handleReadyRead();
    void handleTimeout();
    void handleError(QSerialPort::SerialPortError error);

signals:
    void onReadByte(uint16_t addr);
    void onWriteByte(uint16_t addr, uint8_t value);
    void onReadFloat(uint16_t addr);
    void onWriteFloat(uint16_t addr, float value);
    void onAddrError(uint16_t addr);
    void readError(uint16_t addr);
};

#endif // SNXCOMM_H
