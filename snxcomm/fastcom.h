#ifndef FASTCOM_H
#define FASTCOM_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include <QtSerialPort/qserialport.h>
#include <QTimer>
#include <QThread>
#include <QObject>

/// Fast serial logger and control comm protocol.
///
/// The 'fast' feature comes from the light protocol and lack of error
/// control.
class FastCom : public QThread
{
	Q_OBJECT

	// The serial port for commuication
	QSerialPort*    mSerialPort = nullptr;

	struct Commands
	{
		const std::vector<uint16_t> mReadBytes;
		const std::vector<uint16_t> mReadWords;
		const std::vector<std::tuple<uint16_t, uint8_t>> mWriteBytes;
		const std::vector<std::tuple<uint16_t, uint16_t>> mWriteWords;
	};

	struct Results
	{
		const std::vector<std::tuple<uint16_t, uint8_t>> mReadBytes;
		const std::vector<std::tuple<uint16_t, uint16_t>> mReadWords;
		const std::vector<std::tuple<uint16_t, uint8_t>> mWriteBytes;
		const std::vector<std::tuple<uint16_t, uint16_t>> mWriteWords;
	};

	std::queue<Commands>	mCommandQueue;
	std::mutex			mMutex;
	std::condition_variable
						mCondVar;

	void doProcessCommands(Commands& commands);
public:
	explicit FastCom(QObject *parent = nullptr);

	bool open(const QString& portName);
	bool close();

	void readBytes(const std::vector<uint16_t>& addrs);
	void readWords(const std::vector<uint16_t>& addrs);

	void run() override;
signals:

public slots:
};

#endif // FASTCOM_H
