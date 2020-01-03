#include <QCoreApplication>
#include <WinSock2.h>
#include <iostream>
#include "snxcomm.h"

QT_USE_NAMESPACE

template <typename T>
void swapBytes(T& value)
{
    T   ret;
	char* swapped = static_cast<char*>(&ret);
	char* norm = static_cast<char*>(&value);

    for (size_t i=0; i<sizeof(T); ++i)
    {
        swapped[i] = norm[sizeof(T) - i - 1];
    }

    value = ret;
}

SnxComm::SnxComm(QObject* parent)
    :   QObject(parent)
{
}

SnxComm::~SnxComm()
{
    close();
}

bool SnxComm::open(const QString &portName)
{
    close();
    mSerialPort = new QSerialPort();
    mSerialPort->setPortName(portName);

    connect(mSerialPort, &QSerialPort::readyRead, this, &SnxComm::handleReadyRead);
    connect(mSerialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &SnxComm::handleError);
    connect(&mTimer, &QTimer::timeout, this, &SnxComm::handleTimeout);

	mSerialPort->setBaudRate(57600);
    mSerialPort->setDataBits(QSerialPort::Data8);
    mSerialPort->setParity(QSerialPort::NoParity);
    bool ok = mSerialPort->open(QIODevice::ReadWrite);
    if (!ok)
    {
        std::cout << "Failed to open serial port : " << mSerialPort->errorString().toStdString() << std::endl;
    }
	else
	{
		mSerialPort->sendBreak();
	}

	return ok;
}

void SnxComm::close()
{
    if (mSerialPort != nullptr)
    {
        delete mSerialPort;
        mSerialPort = nullptr;
    }
}

void SnxComm::readByte(uint16_t addr)
{
    if (mSerialPort == nullptr)
    {
        throw "Port closed";
    }

    qDebug("Send read byte x0%04x", addr);

    // prepare message buffer
    MsgRead msg;
    msg.mBeginMarker = 0x01;
    msg.mCommand = READ_BYTE;
    msg.mAddr = addr;
    msg.mEndMarker = 0x04;

	mSerialPort->write(reinterpret_cast<char*>(&msg), sizeof(msg));

    mWaitingAddr = addr;
    mState = WAIT_BYTE;

    // wait the response, 1s max
    mTimer.start(1000);
}

void SnxComm::writeByte(uint16_t addr, uint8_t value)
{
    if (mSerialPort == nullptr)
    {
        throw "Port closed";
    }

    // prepare message buffer
    MsgWriteByte msg;
    msg.mBeginMarker = 0x01;
    msg.mCommand = WRITE_BYTE;
    msg.mAddr = addr;
    msg.mValue = value;
    msg.mEndMarker = 0x04;

	mSerialPort->write(reinterpret_cast<char*>(&msg), sizeof(msg));

    mWaitingAddr = addr;
    mState = WAIT_BYTE;

    // wait the response, 1s max
    mTimer.start(1000);
}

void SnxComm::readFloat(uint16_t addr)
{
    if (mSerialPort == nullptr)
    {
        throw "Port closed";
    }

    // prepare message buffer
    MsgRead msg;
    msg.mBeginMarker = 0x01;
    msg.mCommand = READ_FLOAT;
    msg.mAddr = addr;
    msg.mEndMarker = 0x04;

    qDebug("Send read req");
	mSerialPort->write(reinterpret_cast<char*>(&msg), sizeof(msg));

    mWaitingAddr = addr;
    mState = WAIT_BYTE;

    // wait the response, 1s max
    mTimer.start(1000);
}

void SnxComm::writeFloat(uint16_t addr, float value)
{
    if (mSerialPort == nullptr)
    {
        throw "Port closed";
    }

    // prepare message buffer
    MsgWriteFloat msg;
    msg.mBeginMarker = 0x01;
    msg.mCommand = WRITE_FLOAT;
    msg.mAddr = addr;
    msg.mValue = value;
//    swapBytes(msg.mValue);
    msg.mEndMarker = 0x04;

	mSerialPort->write(reinterpret_cast<char*>(&msg), sizeof(msg));

    mWaitingAddr = addr;
    mState = WAIT_BYTE;

    // wait the response, 1s max
    mTimer.start(1000);
}

void SnxComm::handleReadyRead()
{
 //   while (! mSerialPort->atEnd())
    {
        mReceiveBuffer.append(mSerialPort->readAll());

        qDebug("Received data. Buffer size=%u", mReceiveBuffer.size());

		int expected = 1;

        while (expected != 0 && mReceiveBuffer.size() > 0)
        {
            expected = 0;

			if (mReceiveBuffer.size() > int(sizeof(MsgHeader)))
            {
                // look the header to know the expected message size
				const auto hdr = reinterpret_cast<const MsgHeader*>(mReceiveBuffer.data());

                if (hdr->mBeginMarker != 0x01)
                {
                    // bad header, consume 1 byte
                    expected = 1;
                }
                else
                {
                    switch (hdr->mCommand)
                    {
                    case READ_BYTE:
                        {
                            expected = sizeof(MsgRead);
                            if (mReceiveBuffer.size() < expected)
                                return;

							const auto msg = reinterpret_cast<const MsgRead*>(mReceiveBuffer.data());
        //                    emit onReadByte(qFromBigEndian(msg->mAddr));
                            emit onReadByte(msg->mAddr);
                        }
                        break;
                    case WRITE_BYTE:
                        {
                            expected = sizeof(MsgWriteByte);
                            if (mReceiveBuffer.size() < expected)
                                return;

							auto msg = reinterpret_cast<MsgWriteByte*>(mReceiveBuffer.data());
        //                    swapBytes(msg->mAddr);
                            emit onWriteByte(
                                        msg->mAddr,
                                        msg->mValue);
                        }
                        break;
                    case READ_FLOAT:
                        {
                            expected = sizeof(MsgRead);
                            if (mReceiveBuffer.size() < expected)
                                return;

							auto msg = reinterpret_cast<MsgRead*>(mReceiveBuffer.data());
        //                    swapBytes(msg->mAddr);
                            emit onReadFloat(msg->mAddr);
                        }
                        break;
                    case WRITE_FLOAT:
                        {
                            expected = sizeof(MsgWriteFloat);
                            if (mReceiveBuffer.size() < expected)
                                return;

							auto msg = reinterpret_cast<MsgWriteFloat*>(mReceiveBuffer.data());
        //                    swapBytes(msg->mAddr);
        //                    swapBytes(msg->mValue);
                            emit onWriteFloat(
                                        msg->mAddr,
                                        msg->mValue);
                        }
                        break;
                    case ADDR_ERROR:
                        {
                            expected = sizeof(MsgAddrError);
                            if (mReceiveBuffer.size() < expected)
                                return;

							const auto msg = reinterpret_cast<const MsgAddrError*>(mReceiveBuffer.data());
        //                    emit onReadByte(qFromBigEndian(msg->mAddr));
                            emit onAddrError(msg->mAddr);
                        }
                        break;
                    default:
                        {
                            // transmission error ! consume 2 bytes
                            expected = 2;
                            break;
                        }
                    }
                }
            }
            // consume data
			mReceiveBuffer = mReceiveBuffer.right(mReceiveBuffer.size() - expected);

            if (mTimer.isActive())
                mTimer.stop();
        }
    }
}

void SnxComm::handleTimeout()
{
}

void SnxComm::handleError(QSerialPort::SerialPortError serialPortError)
{
	(void) serialPortError;
}

