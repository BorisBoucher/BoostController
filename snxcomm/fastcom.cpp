#include <iostream>
#include <iso646.h>
#include <QtDebug>
#include "fastcom.h"

void FastCom::encodeHeader(uint8_t* buffer, bool reading, uint16_t addr)
{
	uint8_t code = reading ? 0x50 : 0xa0;
	buffer[0] = code + uint8_t((addr >> 8) & 0xf);
	buffer[1] = uint8_t(addr & 0xff);
}

void FastCom::doReads(const std::vector<ReadCommand>& reads)
{
	ReadResults results{QDateTime::currentDateTime(), {}};

	for (const auto& read : reads)
	{
		uint8_t buffer[2];
		uint8_t recvBuffer[2];

		encodeHeader(buffer, true, read.mAddr);

		qint64 recvSize = read.mAddr < 0x800 ? 1 : 2;
		auto ok = transaction(buffer, 2, recvBuffer, recvSize);

		if (ok)
		{
			// save read result in result vector
			float result;
			if (read.mAddr < 0x800)
			{
				if (read.mSigned)
				{
					result = static_cast<int8_t>(recvBuffer[0]);
				}
				else
				{
					result = recvBuffer[0];
				}
			}
			else
			{
				uint16_t u16 = static_cast<uint16_t>((uint16_t(recvBuffer[0]) << 8) + recvBuffer[1]);
				if (read.mSigned)
				{
					result = static_cast<int16_t>(u16);
				}
				else
				{
					result = u16;
				}
			}
			result /= read.mFactor;
			results.mValues.push_back({read.mAddr, result});
		}
		if (!ok)
		{
			++mWriteError;
		}
	}

	// publish results...
	std::lock_guard<std::mutex> lock(mMutex);
	// but only if the main app is not stall
	if (mReadResults.size() < 100)
	{
		mReadResults.push_back(results);
	}

//	std::cout << QDateTime::currentDateTime().toString().toStdString() << ":";
//	std::cout << "Read result published : " << results.mValues.size() << " items" << std::endl;
}

bool FastCom::transaction(uint8_t* sendBuffer, qint64 sendSize,
						  uint8_t* recvBuffer, qint64 recvSize)
{
	// purge reception
	this->mSerialPort->clear(QSerialPort::Input);

	auto sent = mSerialPort->write(reinterpret_cast<const char*>(sendBuffer), sendSize);
	mSerialPort->flush();

	if(sent != sendSize)
	{
		return false;
	}
	// read response
	qint64 received = 0;
	int loop = 0;
	QTime timer;
	timer.start();
	while (timer.elapsed() < 5000 && received < recvSize)
	{
		if (this->mSerialPort->waitForReadyRead(100))
		{
			qint64 count =
					mSerialPort->read(
						reinterpret_cast<char*>(recvBuffer + received),
						recvSize - received);
			received += count;
		}
		++loop;
	}
	if (loop > 1)
	{
		qDebug() << "Loop = " << loop << ", result :" << (received == recvSize) << "took :" << timer.elapsed() << "ms";
	}

	return received == recvSize;
}

void FastCom::processWrites()
{
	std::unique_lock<std::mutex> lock(mMutex);

	// take ownership of the write command list
	auto writes = std::move(mOnceWrites);
	lock.unlock();

	for (const auto& write : writes)
	{
		uint8_t buffer[4];
		uint8_t recvBuffer[4];

		qint64 size;
		encodeHeader(buffer, false, write.mAddr);
		if (write.mAddr < 0x800)
		{
			// 1 byte write
			buffer[2] = uint8_t(int(std::round(write.mValue * write.mFactor)) & 0xff);
			size = 3;
		}
		else
		{
			// 2 byte word
			uint16_t u = uint16_t(int(std::round(write.mValue * write.mFactor)) & 0xffff);
			buffer[2] = uint8_t((u >> 8) & 0xff);
			buffer[3] = uint8_t(u & 0xff);
			size = 4;
		}

		qint64 recvSize = write.mAddr < 0x800 ? 1 : 2;
		auto ok = transaction(buffer, size, recvBuffer, recvSize);

		if (ok)
		{
			// check that we have write confirmation
			for (int i=0; i < recvSize; ++i)
			{
				ok = ok && (buffer[2+i] == recvBuffer[i]);
			}
		}
		if (not ok)
		{
			++mWriteError;
		}
	}
}

void FastCom::processReads()
{
	std::unique_lock<std::mutex> lock(mMutex);

	// Take ownsership of read list
	auto reads = std::move(mOnceReads);
	lock.unlock();

	doReads(reads);
}

void FastCom::processPeriodicReads()
{
	std::unique_lock<std::mutex> lock(mMutex);

	// copy list of periodics reads
	auto perReads = mPeriodicReads;
	lock.unlock();

	std::vector<ReadCommand> reads;
	for (auto& r : perReads)
	{
		if (mPeriodicCounter % r.mPeriod == 0)
		{
			reads.push_back(r.mReadCmd);
		}
	}
	doReads(reads);
	++mPeriodicCounter;
}

FastCom::FastCom(QObject *parent) : QThread(parent)
{

}

FastCom::~FastCom()
{
	mStopRequested = true;
	this->wait();
}

void FastCom::run()
{
	mSerialPort = new QSerialPort();
	mSerialPort->setPortName(mPortName);

	mSerialPort->setBaudRate(57600);
//	mSerialPort->setBaudRate(115200);
	mSerialPort->setDataBits(QSerialPort::Data8);
	mSerialPort->setParity(QSerialPort::NoParity);
	bool ok = mSerialPort->open(QIODevice::ReadWrite);
	if (!ok)
	{
		std::cout << "Failed to open serial port : " << mSerialPort->errorString().toStdString() << std::endl;
	}
	mSerialPort->setTextModeEnabled(false);

	// Operation prorities :
	// 1 - writes
	// 2 - once reads
	// 3 - periodic reads

	do
	{
		processWrites();
		processReads();
		processPeriodicReads();

		// Sleep a little betwen query
		QThread::msleep(10);
	}
	while (not mStopRequested);

	close();
}

bool FastCom::open(const QString& portName)
{
	if (mSerialPort != nullptr)
		return false;

	// Store COM port name
	mPortName = portName;

	// Start comm thread
	mStopRequested = false;
	this->start();

	return true;
}

bool FastCom::close()
{
	if (mSerialPort != nullptr)
	{
		delete mSerialPort;
		mSerialPort = nullptr;
	}

	return true;
}

void FastCom::writeValues(const std::vector<WriteCommand>& values)
{
	std::lock_guard<std::mutex> lock(mMutex);

	for (auto& value : values)
	{
		mOnceWrites.push_back(value);
	}
}

void FastCom::readValues(const std::vector<ReadCommand>& cmds)
{
	std::lock_guard<std::mutex> lock(mMutex);

	for (auto& cmd : cmds)
	{
		mOnceReads.push_back(cmd);
	}
}

void FastCom::setPrediodicRead(const std::vector<PeriodicReadCommand>& cmds)
{
	std::lock_guard<std::mutex> lock(mMutex);

	// Replace periodic reads
	mPeriodicReads = cmds;
}

std::vector<FastCom::ReadResults> FastCom::getReadResult()
{
	std::lock_guard<std::mutex> lock(mMutex);

	return std::move(mReadResults);
}
