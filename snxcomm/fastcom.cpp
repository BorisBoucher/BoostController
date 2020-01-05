#include "fastcom.h"

void FastCom::doProcessCommands(FastCom::Commands& commands)
{
	Results res;

	auto makeHeader = [](bool write, uint16_t addr, QByteArray& ba)
	{
		auto h = write ? 0x50 : 0xa0;
		ba.append(char(h + ((addr >> 8) & 0xf)));
		ba.append(char(addr & 0xff));
	};

	for (uint16_t addr : commands.mReadBytes)
	{
		QByteArray ba;
		ba.reserve(2);
		makeHeader(false, addr, ba);
		mSerialPort->write(ba);

		// read response
//		mSerialPort->ti
//		mSerialPort->read()
	}
}

FastCom::FastCom(QObject *parent) : QThread(parent)
{

}

void FastCom::readBytes(const std::vector<uint16_t>& addrs)
{

}

void FastCom::run()
{
	std::unique_lock<std::mutex> lock(mMutex);

	do
	{
		// wait for work to do
		mCondVar.wait(lock);

		while (! mCommandQueue.empty())
		{
			auto commands = mCommandQueue.back();
			mCommandQueue.pop();
			lock.unlock();
			doProcessCommands(commands);
			lock.lock();
		}
	} while(true);
}
