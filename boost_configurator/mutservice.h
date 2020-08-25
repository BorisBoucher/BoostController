#ifndef MUTSERVICE_H
#define MUTSERVICE_H

#include <assert.h>
#include <set>
#include <QtSerialPort/qserialport.h>
#include <QObject>

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
