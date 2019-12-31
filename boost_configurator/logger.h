#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>

class Logger : public QObject
{
	Q_OBJECT

	QString	mFileName;

	QFile	mLogFile;
	size_t	mLogId = 0;
	qint64 mLogStart;
public:
	explicit Logger(QObject *parent = nullptr);

//	class IPlotter
//	{
//	public:
//		virtual void setChannel(std::vector<ChannelInfo>& channelInfos) =0;

//		virtual void addSample(size_t channel, double date, double value) =0;


//	};

//	void setPlotter(IPlotter* plotter):

	void startLog();
	void stopLog();
	bool isStarted();

	void addLogline(
			float*	TPS,
			float*	RPM,
			float*	speed,
			int gear,
			float*	timingAdv,
			float*	fuelTrimLow,
			float*	fuelTrimMid,
			float*	fuelTrimHigh,
			float*	MAP,
			float*	targetBoost,
			float*	solDC,
			int*	knockSum,
			float*	O2FuelTrim,
			float*	O2Front,
			float*	O2Rear,
			float*	injPulseFront,
			float*	injPulseRear,
			float*	accelEnrich
			);

//	void loadLog(const QString& filename);
//	void saveLog(const QString& filename);
signals:

public slots:
};

#endif // LOGGER_H
