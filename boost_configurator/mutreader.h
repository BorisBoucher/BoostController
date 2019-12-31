#ifndef MUTREADER_H
#define MUTREADER_H

#include <thread>
#include <mutex>
#include <QObject>
#include <QtSerialPort/qserialport.h>

struct MutData
{
	int	mKnockCount;
	float mO2Front;
	float mO2Rear;
	float mFuelTrimLow;
	float mFuelTrimMid;
	float mFuelTrimHigh;
	float mO2FuelTrim;
	float mIgnAdv;
	float mIngPulseFront;
	float mIngPulseRear;
	float mAccelEnrichment;
};

class MutReader : public QObject
{
	Q_OBJECT

	bool mExitThread;
	MutData	mMutData;
	std::thread mThread;
	std::mutex mMutex;

	// The serial port for commuication
	QSerialPort*    mSerialPort = nullptr;

	void run();
public:

	explicit MutReader(QObject *parent = nullptr);

	bool open(const QString& portName);
	void close();

	bool isOpen();
	MutData getData();

signals:

public slots:
};

#endif // MUTREADER_H
