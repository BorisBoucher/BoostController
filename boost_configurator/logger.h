#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>

class Logger : public QObject
{
	Q_OBJECT

	QString	mFileName;

	QFile	mLogFile;
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

	void loadLog(const QString& filename);
	void saveLog(const QString& filename);
signals:

public slots:
};

#endif // LOGGER_H
