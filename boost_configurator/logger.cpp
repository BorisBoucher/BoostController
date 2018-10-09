#include "logger.h"
#include <QDateTime>
#include <QTextStream>

Logger::Logger(QObject *parent) : QObject(parent)
{

}

void Logger::startLog()
{
	stopLog();

	// Compute log file name
	QDateTime now = QDateTime::currentDateTime();

	mFileName = "Log_" + now.toString("yyyy.MM.dd_hh.mm.ss") + ".csv";

	mLogFile.setFileName(mFileName);
	mLogFile.open(QIODevice::WriteOnly);

	// write header lines
	QTextStream out(&mLogFile);
	out << "LogID,LogEntryDate,LogEntryTime,LogEntrySeconds,LogNotes,"
		<< "TPS,RPM,TimingAdv,FuelTrim_Low,FuelTrim_Mid,FuelTrim_High" << endl;
}

void Logger::stopLog()
{
	if (mLogFile.isOpen())
		mLogFile.close();
}

bool Logger::isStarted()
{
	return mLogFile.isOpen();
}

void Logger::loadLog(const QString& filename)
{

}

void Logger::saveLog(const QString& filename)
{

}
