#ifndef MUTSERVICE_H
#define MUTSERVICE_H

#include <assert.h>
#include <set>
#include <QtSerialPort/qserialport.h>
#include <QObject>


/// Comm protocol with the boost controler or other device.
class SnxProtocol : public QObject
{
	Q_OBJECT

	std::vector<uint8_t>	mInputBuffer;

	void writeByte(uint32_t& crc, std::vector<uint8_t>& msg, uint8_t value)
	{
		switch (value)
		{
		case 0x02:
			msg.push_back(0x03);
			msg.push_back(0xf2);
			break;
		case 0x03:
			msg.push_back(0x03);
			msg.push_back(0xf3);
			break;
		default:
			msg.push_back(value);
		}

		crc += value;
	}

private slots:
	void handleReadyRead()
	{
		while (mCommPort->bytesAvailable() > 0)
		{
			char c;
			mCommPort->getChar(&c);
			mInputBuffer.push_back(uint8_t(c));
		}

		// process received data. First find a message start
		auto it = mInputBuffer.begin();
		do
		{
			// First find a message start
			while (it != mInputBuffer.end() && *it != 0x02)
				++it;

			if (it == mInputBuffer.end())
			{
				// drop garbage
				mInputBuffer.clear();
			}
			else
			{
				// try to read the message
				if (mInputBuffer.end() - it <= 2)
				{
					// incomplete message. Wait next ready read
					return;
				}
				// msg begin
				uint32_t crc = 0;
				std::vector<uint8_t> msg;
				msg.push_back(*it++);
				++it;
				// size
				writeByte(crc, msg, *it++);
				uint32_t size = msg.back() + 1; // data + crc
				// Read message data
				while (it != mInputBuffer.end() && size > 0)
				{
					writeByte(crc, msg, *it++);
				}
				if (size == 0)
				{
					// check CRC
					if (msg.back() != uint8_t(crc))
					{
						// bad message, drop all
						mInputBuffer.clear();
					}
					// message is complete, process it
					processMessage(msg);

					// consume data of this message
					mInputBuffer = std::vector<uint8_t>(it, mInputBuffer.end());
					it = mInputBuffer.begin();
				}
			}
		} while (it != mInputBuffer.end());
	}

	void handleError(QSerialPort::SerialPortError error)
	{
		(void) error;
	}

	void processMessage(const std::vector<uint8_t>& msg)
	{
		if (msg.size() < 3)
			return;

		auto type = msg[2];
		switch (type)
		{
		case 'r':
			processReadBytesResponse(msg);
			break;
		case 'R':
			processReadMemoryResponse(msg);
			break;
		case 'A':
			processAckResponse(msg);
			break;
		case 'X':
			processErrorResponse(msg);
			break;
		}
	}

	void processReadBytesResponse(const std::vector<uint8_t>& msg)
	{
		size_t nbBytes = (msg[1] - 1) / 3;
		assert(nbBytes * 3 +1 == msg[1]);
		if (msg.size() != msg[1] + 3)
			return;

		std::vector<AddrInfo> addrInfos;
		for (size_t i=0; i< nbBytes; ++i)
		{
			AddrInfo addrInfo;
			addrInfo.mAddr = msg[3 + i*3] * 0x100 + msg[3 + i*3 + 1];
			addrInfo.mValue = msg[3 + i*3 + 2];
		}
		emit onReadBytes(addrInfos);
	}

	void processReadMemoryResponse(const std::vector<uint8_t>& msg)
	{
		size_t nbBytes = (msg[1] - 3);
		assert(nbBytes * 3 +1 == msg[1]);
		if (msg.size() != msg[1] + 6)
			return;

		std::vector<uint8_t> data;
		uint32_t addr;
		addr = msg[3] * 0x100 + msg[4];
		for (size_t i=0; i< nbBytes; ++i)
		{
			data.push_back(msg[5 + i]);
		}
		emit onReadMemory(addr, data);
	}

	void processAckResponse(const std::vector<uint8_t>& msg)
	{
		if (msg.size() != 4)
			return;

		emit onWriteAck();
	}

	void processErrorResponse(const std::vector<uint8_t>& msg)
	{
		if (msg.size() != 5)
			return;

		Error err;
		switch (msg[3])
		{
		case 'C':	err = CRC_ERROR;		break;
		case 'R':	err = INVALID_READ;		break;
		case 'W':	err = INVALID_WRITE;	break;
		case 'L':	err = READ_ONLY;		break;
		case 'O':	err = OVERFLOW_ERROR;	break;
		case 'U':	err = UNKNOWN_COMMAND;	break;
		default:
			return;
		}

		emit onError(err);
	}

public:

	enum Error
	{
		CRC_ERROR,
		INVALID_READ,
		INVALID_WRITE,
		READ_ONLY,
		OVERFLOW_ERROR,
		UNKNOWN_COMMAND
	};

	QSerialPort*	mCommPort = nullptr;

	bool open(const QString portName)
	{
		mCommPort = new QSerialPort();
		mCommPort->setPortName(portName);

		connect(mCommPort, &QSerialPort::readyRead, this, &SnxProtocol::handleReadyRead);
		connect(mCommPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
				this, &SnxProtocol::handleError);

		mCommPort->setBaudRate(115200);
		mCommPort->setDataBits(QSerialPort::Data8);
		mCommPort->setParity(QSerialPort::NoParity);
		bool ok = mCommPort->open(QIODevice::ReadWrite);

		return ok;
	}

	void close()
	{
		if (mCommPort != nullptr)
		{
			delete mCommPort;
			mCommPort = nullptr;
		}
	}

	struct AddrInfo
	{
		uint32_t	mAddr;
		uint8_t		mValue;
	};

	void readBytes(const std::vector<uint32_t>& addrs)
	{
		std::vector<uint8_t> msg;
		uint32_t crc = 0;
		msg.push_back(0x02);
		writeByte(crc, msg, uint8_t(addrs.size()*2+1));
		writeByte(crc, msg, 'r');
		for (auto addr : addrs)
		{
			writeByte(crc, msg, uint8_t(addr >> 8));
			writeByte(crc, msg, uint8_t(addr & 0xff));
		}
		writeByte(crc, msg, static_cast<uint8_t>(crc));
		mCommPort->write(reinterpret_cast<char*>(msg.data()), msg.size());
	}

	void readMemory(uint32_t addr, size_t size)
	{
		std::vector<uint8_t> msg;
		uint32_t crc = 0;
		msg.push_back(0x02);
		writeByte(crc, msg, 4);
		writeByte(crc, msg, 'R');
		writeByte(crc, msg, uint8_t(addr >> 8));
		writeByte(crc, msg, uint8_t(addr & 0xff));
		writeByte(crc, msg, uint8_t(size));
		writeByte(crc, msg, uint8_t(crc));
		mCommPort->write(reinterpret_cast<char*>(msg.data()), msg.size());
	}

	void writeBytes(const std::vector<AddrInfo>& writeInfos)
	{
		std::vector<uint8_t> msg;
		uint32_t crc = 0;
		msg.push_back(0x02);
		writeByte(crc, msg, uint8_t(writeInfos.size()*3+1));
		writeByte(crc, msg, 'w');
		for (auto writeInfo : writeInfos)
		{
			writeByte(crc, msg, uint8_t(writeInfo.mAddr >> 8));
			writeByte(crc, msg, uint8_t(writeInfo.mAddr & 0xff));
			writeByte(crc, msg, uint8_t(writeInfo.mValue));
		}
		writeByte(crc, msg, uint8_t(crc));
		mCommPort->write(reinterpret_cast<char*>(msg.data()), msg.size());
	}

	void writeMemory(uint32_t addr, const std::vector<uint8_t>& data)
	{
		std::vector<uint8_t> msg;
		uint32_t crc = 0;
		msg.push_back(0x02);
		writeByte(crc, msg, uint8_t(data.size()+3));
		writeByte(crc, msg, 'W');
		writeByte(crc, msg, uint8_t(addr >> 8));
		writeByte(crc, msg, uint8_t(addr & 0xff));
		for (auto b : data)
		{
			writeByte(crc, msg, b);
		}
		writeByte(crc, msg, uint8_t(crc));
		mCommPort->write(reinterpret_cast<char*>(msg.data()), msg.size());
	}

signals:
	void onReadBytes(const std::vector<AddrInfo>& readInfo);
	void onReadMemory(uint32_t addr, const std::vector<uint8_t>& data);
	void onWriteAck();
	void onError(Error error);
};


class IMutDataProvider
{
public:
	virtual uint8_t	getMutData(uint8_t varId) =0;
};

/// MUT service and pass thrue.
/// Provide data acquired from boost controler as well as data from the
/// MUT interface of the 3000GT.
class MutService : public QObject
{
    Q_OBJECT

	IMutDataProvider*	mProvider = nullptr;
	std::set<uint8_t>	mLocalVars;

	// Serial port used to read the 3000GT MUT interface
	QSerialPort*		mMUTSerialPort = nullptr;

	// Serial port used to receive MUT logger request
	QSerialPort*		mLoggerSerialPort = nullptr;
public:
    explicit MutService(QObject *parent = nullptr);
	~MutService();

	void registerDataProvider(IMutDataProvider* provider, std::set<uint8_t> localVars);

	void open(const QString& MUTPortName, const QString& LoggerPortName);
	void close();

signals:
	uint8_t onReadVar(uint8_t varID);

public slots:

private slots:
	void handleMUTReadyRead();
	void handleLoggerReadyRead();
//	void handleTimeout();
	void handleError(QSerialPort::SerialPortError error);

};

#endif // MUTSERVICE_H
