#include <QtSerialPort/qserialportinfo.h>
#include "setting.h"
#include "ui_setting.h"

Setting::Setting(const QString& logDir,
				 const QString& BCPort,
				 const QString& MUTPort,
				 QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Setting)
{
	ui->setupUi(this);

	ui->logDirEdit->setText(logDir);

	auto portList = QSerialPortInfo::availablePorts();

	for (const auto& portInfo : portList)
	{
		ui->BCPortComboBox->addItem(portInfo.portName());
		ui->MUTPortComboBox->addItem(portInfo.portName());
	}

	// preset combo
	ui->BCPortComboBox->setCurrentIndex(ui->BCPortComboBox->findText(BCPort));
	ui->MUTPortComboBox->setCurrentIndex(ui->MUTPortComboBox->findText(MUTPort));
}

Setting::~Setting()
{
	delete ui;
}

QString Setting::getLogDir()
{
	return ui->logDirEdit->text();
}

QString Setting::getBCPort()
{
	return ui->BCPortComboBox->currentText();
}

QString Setting::getMUTPort()
{
	return ui->MUTPortComboBox->currentText();
}
