#ifndef FASTCOM_H
#define FASTCOM_H

#include <thread>
#include <mutex>
#include <queue>
#include <QDateTime>

#include <QtSerialPort/qserialport.h>
#include <QTimer>
#include <QThread>
#include <QObject>

/// Fast serial logger and control comm protocol.
///
/// The 'fast' feature comes from the light protocol and lack of error
/// control.
class FastCom : QThread
{
	Q_OBJECT

public:
	struct WriteCommand
	{
		uint16_t mAddr;
		float mValue;
		float mFactor;
		bool mSigned;
	};

	struct ReadCommand
	{
		uint16_t mAddr;
		float mFactor;
		bool mSigned;
	};

	struct PeriodicReadCommand
	{
		ReadCommand mReadCmd;
		size_t mPeriod;
	};

	struct Value
	{
		uint16_t mAddr;
		float mValue;
	};

	struct ReadResults
	{
		QDateTime mTimeStamp;
		std::vector<Value> mValues;
	};

private:

	size_t mPeriodicCounter = 0;
	std::mutex mMutex;
	std::vector<PeriodicReadCommand> mPeriodicReads;
	std::vector<ReadCommand> mOnceReads;
	std::vector<WriteCommand> mOnceWrites;
	std::vector<ReadResults> mReadResults;

	std::atomic<int64_t> mWriteError = {0};

	bool mStopRequested = false;

	QString mPortName;
	// The serial port for commuication
	QSerialPort*    mSerialPort = nullptr;

	void encodeHeader(uint8_t* buffer, bool reading, uint16_t addr);
	void doReads(const std::vector<ReadCommand>& reads);
	bool transaction(uint8_t* sendBuffer, qint64 sendSize,
					 uint8_t* recvBuffer, qint64 recvSize);
	void processWrites();
	void processReads();
	void processPeriodicReads();

	void run();

	bool close();

public:
	explicit FastCom(QObject *parent);
	~FastCom();

	bool open(const QString& portName);

	void writeValues(const std::vector<WriteCommand>& values);
	void readValues(const std::vector<ReadCommand>& addrs);
	void setPrediodicRead(const std::vector<PeriodicReadCommand>& reads);
	std::vector<ReadResults> getReadResult();
signals:

public slots:
};

#endif // FASTCOM_H
