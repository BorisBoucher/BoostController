
#include <iostream>
#include "mutreader.h"

MutReader::MutReader(QObject *parent) : QObject(parent)
{
	mMutData =
	{
		0,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		0.0f
	};
}


bool MutReader::open(const QString& portName)
{
	close();
	mSerialPort = new QSerialPort();
	mSerialPort->setPortName(portName);

//    connect(mSerialPort, &QSerialPort::readyRead, this, &SnxComm::handleReadyRead);
//    connect(mSerialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
//            this, &SnxComm::handleError);
//    connect(&mTimer, &QTimer::timeout, this, &SnxComm::handleTimeout);

	mSerialPort->setBaudRate(1953);
	mSerialPort->setDataBits(QSerialPort::Data8);
	mSerialPort->setParity(QSerialPort::NoParity);
	bool ok = mSerialPort->open(QIODevice::ReadWrite);
	if (!ok)
	{
		std::cout << "Failed to open MUT serial port : " << mSerialPort->errorString().toStdString() << std::endl;
		delete mSerialPort;
		mSerialPort = nullptr;
	}
	else
	{
		// start reader thread
		mExitThread = false;
		mThread = std::thread(&MutReader::run, this);
	}

	return ok;
}

void MutReader::close()
{
	if (mSerialPort != nullptr)
	{
		mExitThread = true;
		mThread.join();
		mSerialPort->close();
		mSerialPort = nullptr;
	}
}

bool MutReader::isOpen()
{
	return mSerialPort != nullptr;
}

MutData MutReader::getData()
{
	MutData ret;
	std::lock_guard<std::mutex> lock(mMutex);

	ret = mMutData;

	return ret;
}

void MutReader::run()
{
	auto readParam = [this](int paramId) -> uint8_t
	{
		char buffer[2];
		buffer[0] = static_cast<char>(paramId);
		this->mSerialPort->write(buffer, 1);
		this->mSerialPort->waitForBytesWritten(100);

		this->mSerialPort->waitForReadyRead(100);
		auto sizeRead = this->mSerialPort->read(buffer, 2);
		if (sizeRead == 2)
			return static_cast<uint8_t>(buffer[1]);
		else
			return 0;
	};

	MutData data;
	{
		std::lock_guard<std::mutex> lock(mMutex);
		mMutData = data;
	}

	size_t loop = 0;
	do
	{

		// Each loop
		data.mKnockCount = readParam(0x26);
		data.mIgnAdv = readParam(0x06) - 20;
		data.mIngPulseFront = readParam(0x29) * 0.256f;
		data.mIngPulseRear = readParam(0x2a) * 0.256f;
		data.mAccelEnrichment = readParam(0x1d);

		// once every 10 loop
		if (loop % 10 == 0)
		{
			data.mO2Front = readParam(0x13) * 0.01952f;
		}
		if (loop % 10 == 1)
		{
			data.mO2Rear = readParam(0x3e) * 0.01952f;
		}
		if (loop % 10 == 2)
		{
			data.mFuelTrimLow = (readParam(0x0c) - 128) / 5.0f;
		}
		if (loop % 10 == 3)
		{
			data.mFuelTrimMid = (readParam(0x0d) - 128) / 5.0f;
		}
		if (loop % 10 == 4)
		{
			data.mFuelTrimHigh = (readParam(0x0e) - 128) / 5.0f;
		}
		if (loop % 10 == 5)
		{
			data.mO2FuelTrim = (readParam(0x0f) - 128) / 5.0f;
		}

		++loop;

		// publish data
		std::lock_guard<std::mutex> lock(mMutex);
		mMutData = data;
	} while (! mExitThread);
}
