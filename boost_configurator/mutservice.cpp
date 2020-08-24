#include "mutservice.h"
#include <iostream>

MutService::MutService(QObject *parent) : QObject(parent)
{

}

MutService::~MutService()
{
	close();
}

void MutService::registerDataProvider(IMutDataProvider* provider, std::set<uint8_t> localVars)
{
	mProvider = provider;
	mLocalVars = localVars;
}

void MutService::open(const QString& MUTPortName, const QString& LoggerPortName)
{
	close();

	// Open MUT port
	mMUTSerialPort = new QSerialPort();
	mMUTSerialPort->setPortName(MUTPortName);

	connect(mMUTSerialPort, &QSerialPort::readyRead, this, &MutService::handleMUTReadyRead);
	connect(mMUTSerialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
			this, &MutService::handleError);
//	connect(&mTimer, &QTimer::timeout, this, &MutService::handleTimeout);

	mMUTSerialPort->setBaudRate(1953);
	mMUTSerialPort->setDataBits(QSerialPort::Data8);
	mMUTSerialPort->setParity(QSerialPort::NoParity);
	bool ok = mMUTSerialPort->open(QIODevice::ReadWrite);
	if (!ok)
	{
		std::cout << "Failed to open serial port : " << mMUTSerialPort->errorString().toStdString() << std::endl;
	}

	// Open logger port
	mLoggerSerialPort = new QSerialPort();
	mLoggerSerialPort->setPortName(LoggerPortName);

	connect(mLoggerSerialPort, &QSerialPort::readyRead, this, &MutService::handleLoggerReadyRead);
	connect(mLoggerSerialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
			this, &MutService::handleError);
//	connect(&mTimer, &QTimer::timeout, this, &MutService::handleTimeout);

	mLoggerSerialPort->setBaudRate(115200);
	mLoggerSerialPort->setDataBits(QSerialPort::Data8);
	mLoggerSerialPort->setParity(QSerialPort::NoParity);
	ok = mLoggerSerialPort->open(QIODevice::ReadWrite);
	if (!ok)
	{
		std::cout << "Failed to open serial port : " << mLoggerSerialPort->errorString().toStdString() << std::endl;
	}

}

void MutService::close()
{
	delete mLoggerSerialPort;
	delete mMUTSerialPort;

	mLoggerSerialPort = nullptr;
	mMUTSerialPort = nullptr;
}

void MutService::handleMUTReadyRead()
{
	char c;
	while (mMUTSerialPort->getChar(&c))
	{
		mLoggerSerialPort->putChar(c);
	}
}

void MutService::handleLoggerReadyRead()
{
	char c;
	while (mMUTSerialPort->getChar(&c))
	{
		uint8_t id = (uint8_t)c;
		auto it = mLocalVars.find(id);
		if (it != mLocalVars.end())
		{
			if (mProvider != nullptr)
			{
				uint8_t val = mProvider->getMutData(id);
				mLoggerSerialPort->putChar(c);
				mLoggerSerialPort->putChar((char)val);
			}
		}
		else
		{
			// forward to MUT device
			mMUTSerialPort->putChar(c);
		}
	}
}

void MutService::handleError(QSerialPort::SerialPortError error)
{

}
