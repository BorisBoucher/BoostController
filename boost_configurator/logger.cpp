#include "logger.h"
#include <QDateTime>
#include <QTextStream>

Logger::Logger(QObject *parent) : QObject(parent)
{

}

void Logger::startLog(const QString& logDir)
{
	stopLog();

	// Compute log file name
	QDateTime now = QDateTime::currentDateTime();

	mFileName = logDir + "/Log_" + now.toString("yyyy.MM.dd_hh.mm.ss") + ".csv";

	mLogFile.setFileName(mFileName);
	mLogFile.open(QIODevice::WriteOnly);

	mLogId = 1;
	mLogStart = now.toMSecsSinceEpoch();

	// write header lines
	QTextStream out(&mLogFile);
	out << "LogID,LogEntryDate,LogEntryTime,LogEntrySeconds,LogNotes,"
		<< "TPS,RPM,Speed,Gear,TimingAdv,FuelTrim_Low,FuelTrim_Mid,FuelTrim_High,"
		   "FuelPress,MAP,TargetBoost,WGDC,"
		<< "knockSum,O2FuelTrim,O2Front,O2Rear,InjPulseFront,InjPulseRear,AccelEnrich,"
		<< "AirFlow"
		<< endl;
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

void Logger::addLogline(
		float* TPS,
		float* RPM,
		float* speed,
		int    gear,
		float* timingAdv,
		float* fuelTrimLow,
		float* fuelTrimMid,
		float* fuelTrimHigh,
		float* fuelPressure,
		float* MAP,
		float* targetBoost,
		float* solDC,
		int*   knockSum,
		float* O2FuelTrim,
		float* O2Front,
		float* O2Rear,
		float* injPulseFront,
		float* injPulseRear,
		float* accelEnrich,
		float* airFlow
		)
{
	if (isStarted())
	{
		QTextStream out(&mLogFile);

		QDateTime now = QDateTime::currentDateTime();

		auto dateStr = now.toString("yyyy-MM-dd");
		auto timeStr = now.toString("hh:mm:ss.zzz");

		auto formatFloat = [&out] (float *pf, int nbDec = 3, bool withSep = true)
		{
			if (pf != nullptr)
			{
				char buffer[20];
				sprintf(buffer, "%.*f", nbDec, *pf);
				out << buffer;
			}
			if (withSep)
			{
				out << ',';
			}
		};
		auto formatInt = [&out] (int *pi, bool withSep = true)
		{
			if (pi != nullptr)
			{
				out << *pi;
			}
			if (withSep)
			{
				out << ',';
			}
		};

		// 1,2019-06-22,10:54:57.50849,0.4929
		out << mLogId << ','
				<< dateStr << ','
				<< timeStr << ','
				<< 	((now.toMSecsSinceEpoch() - mLogStart) / 1000.f) << ','
				<< ',';							// log notes
				formatFloat(TPS, 0);
				formatFloat(RPM, 0);
				formatFloat(speed, 0);
				out << gear << ',';
				formatFloat(timingAdv);
				formatFloat(fuelTrimLow);
				formatFloat(fuelTrimMid);
				formatFloat(fuelTrimHigh);
				formatFloat(fuelPressure, 2);
				formatFloat(MAP, 2);
				formatFloat(targetBoost, 2);
				formatFloat(solDC, 0);
				formatInt(knockSum);
				formatFloat(O2FuelTrim);
				formatFloat(O2Front, 2);
				formatFloat(O2Rear, 2);
				formatFloat(injPulseFront, 1);
				formatFloat(injPulseRear, 1);
				formatFloat(accelEnrich, 0);
				formatFloat(airFlow, 0, false);
				out << endl;

		++mLogId;
	}
}

//void Logger::loadLog(const QString& filename)
//{

//}

//void Logger::saveLog(const QString& filename)
//{

//}
