#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plotter.h"
#include <qtimer.h>
#include <QtSerialPort/qserialportinfo.h>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->tableView->setModel(&myModel);

	mMutService.registerDataProvider(this, {0x2f, 0x38, 0x02, 0x03});

    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(update()));

    auto portList = QSerialPortInfo::availablePorts();

    for (const auto& portInfo : portList)
    {
        ui->comboBox->addItem(portInfo.portName());
		ui->portListMut->addItem(portInfo.portName());
	}

    QLocale myLocale(QLocale::Language::English);
    QLocale::setDefault(myLocale);

    // set validator on input field
    ui->tyreCircumEdit->setValidator(new QDoubleValidator(0.5, 3.0, 3));
    ui->gear1Edit->setValidator(new QDoubleValidator(0.1, 20.0, 3));
    ui->gear2Edit->setValidator(new QDoubleValidator(0.1, 20.0, 3));
    ui->gear3Edit->setValidator(new QDoubleValidator(0.1, 20.0, 3));
    ui->gear4Edit->setValidator(new QDoubleValidator(0.1, 20.0, 3));
    ui->gear5Edit->setValidator(new QDoubleValidator(0.1, 20.0, 3));
    ui->gear6Edit->setValidator(new QDoubleValidator(0.1, 20.0, 3));
    ui->speedRatioEdit->setValidator(new QDoubleValidator(0.1, 200, 3));
    ui->baseBoostEdit->setValidator(new QDoubleValidator(0.01, 5.0, 3));
    ui->pidPEdit->setValidator(new QDoubleValidator(0.0, 3.0, 4));
    ui->pidIEdit->setValidator(new QDoubleValidator(0.0, 3.0, 4));
    ui->pidDEdit->setValidator(new QDoubleValidator(0.0, 3.0, 4));

    // prepare chart
//    mChart = new QtCharts::QChart();
//    mSeries = new QtCharts::QLineSeries();

//    for (int i =0; i<360; ++i)
//    {
//        mSeries->append(i,sin(i / 180.0f * 3.14159));
//    }
//    mChart->addSeries(mSeries);
//    mChart->createDefaultAxes();

//    QGraphicsScene* scene = new QGraphicsScene();
//    scene->addItem(mChart);
//    ui->graphicsView->setScene(scene);
//    mChart->resize(ui->graphicsView->size());
//    mChart->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));

//    connect(scene, &QGraphicsScene::sceneRectChanged, this, &MainWindow::sceneRectChanged );

//    QVector<double> x(101), y(101); // initialize with entries 0..100
//    for (int i=0; i<101; ++i)
//    {
//      x[i] = i/50.0 - 1; // x goes from -1 to 1
//      y[i] = x[i]*x[i]; // let's plot a quadratic function
//    }
    // create graph:
	ui->plot1->legend->setVisible(true);
    ui->plot1->addGraph();
    ui->plot1->addGraph();
    ui->plot1->addGraph();
    ui->plot1->addGraph();
    ui->plot1->addGraph();
//    ui->plot1->graph(0)->setData(x, y);
    ui->plot1->graph(0)->setPen(QPen(QColor(255,0,0)));
	ui->plot1->graph(0)->setName("MAP");
	ui->plot1->graph(1)->setName("THROTTLE");
	ui->plot1->graph(2)->setPen(QPen(QColor(0,255,0)));
	ui->plot1->graph(2)->setName("THROTTLE'");
	ui->plot1->graph(3)->setPen(QPen(QColor(0,0,0)));
	ui->plot1->graph(3)->setName("TARGET_BOOST");
	ui->plot1->graph(4)->setPen(QPen(QColor(0,255,255)));
	ui->plot1->graph(4)->setName("TARGET_OUTPUT");
	// give the axes some labels:
	ui->plot1->xAxis->setLabel("time");
    // set axes ranges, so we see all data:
    ui->plot1->xAxis->setRange(0, 2);
	ui->plot1->yAxis->setRange(-0.5, 1.7);
	// Place legend int lower left cornet
	ui->plot1->axisRect()->insetLayout()
			->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignLeft);
    ui->plot1->replot();

    ui->plot1->setInteraction(QCP::iRangeDrag, true);

	ui->plot2->yAxis->setRange(0,7000);
	ui->plot2->yAxis->setLabel("RPM");
	ui->plot2->yAxis2->setLabel("Speed");
	ui->plot2->yAxis2->setRange(0, 300);
	ui->plot2->yAxis2->setVisible(true);

	ui->plot2->addGraph(nullptr, ui->plot2->yAxis);
	ui->plot2->addGraph(nullptr, ui->plot2->yAxis2);

	ui->plot2->graph(0)->setPen(QPen(QColor(255,0,0)));
//	ui->plot2->graph(0)->setName("RPM");
//	ui->plot2->graph(1)->setName("Speed");


	//    ui->myTable->setColumnCount(7);
//    ui->myTable->setRowCount(6);
//    ui->myTable->horizontalHeader()->setVisible(true);
//    ui->myTable->verticalHeader()->setVisible(true);
//    for (int i=0; i<6; ++i)
//    {
//        ui->myTable->setVerticalHeaderItem(i, new QTableWidgetItem("Gear " + QString::number(i+1), 0));
//        for (int j=0; j<7; ++j)
//        {
//            ui->myTable->setHorizontalHeaderItem(j, new QTableWidgetItem(QString::number((j+1)*1000), 0));
//            auto item = new QTableWidgetItem(QString::number(0), 0);
//            item->setTextAlignment(Qt::AlignRight);
//            item->setForeground(QColor(255,255,255));
//            ui->myTable->setItem(i, j, item);
//        }
//    }

    mTableWidgets["Boost Table"] = TableInfo
    {
            "Boost Table",
            "Gear", "RPM",
            {"1", "2", "3", "4", "5", "6"},
            {"1000", "2000", "3000", "4000", "5000", "6000", "7000"},
            0x0100, 0x10
    };
    ui->listTable->addItem("Boost Table");

    mTableWidgets["Throttle Adjust"] = TableInfo
    {
            "Throttle Adjust",
            "Boost", "Throttle",
            {"%"},
            {"0%", "20%", "40%", "60%", "80%", "100%"},
            0x0160, 0x0
    };
    ui->listTable->addItem("Throttle Adjust");

    mTableWidgets["Throttle Move Adjust"] = TableInfo
    {
            "Throttle Move Adjust",
            "Boost", "Throttle Move (%/s)",
            {"%"},
            {"-1000", "-660", "-330", "0", "+330", "+660", "+1000"},
            0x0170, 0x0
    };
    ui->listTable->addItem("Throttle Move Adjust");

    mTableWidgets["Sol DC"] = TableInfo
    {
            "Sol DC",
            "Target\nBoost\n(Bar)", "Load",
            {"0.0", "0.25", "0.5", "0.75", "1.0", "1.25"},
            {"25%", "50%", "75%", "100%"},
            0x0030, 4
    };
    ui->listTable->addItem("Sol DC");

	auto layout = new QVBoxLayout();
	layout->addWidget(new Plotter);
	ui->tabPlotter->setLayout(layout);
//	auto plotter = new Plotter();
//	plotter->setParent(ui->tabPlotter);
}

void MainWindow::resizeEvent(QResizeEvent */*event*/)
{
//    mChart->resize(ui->graphicsView->size());
}

void MainWindow::sceneRectChanged(const QRectF &/*rect*/)
{
//    mChart->resize(ui->graphicsView->size());
}


void MainWindow::update()
{
//    mSnxComm->readByte(1);
    static int i = 360;
//    mSeries->append(i, sin(i/360.0f));

    ++i;

//    ui->plot1->graph(0)->addData(i, 0.5 + sin(i/36.0f) * 0.5);
//    ui->plot1->graph(1)->addData(i, 0.3 * (0.5 + sin(i/20.0f) * 0.5));
//	ui->plot2->graph(0)->addData(i, 0.1 * (0.5 + sin(i/12.0) * 0.5));
//	ui->plot2->graph(1)->addData(i, 0.7 * (0.5 + sin(i/10.0) * 0.5));
	ui->plot2->graph(0)->addData(i, double(mMeasures.mRPM));
	ui->plot2->graph(1)->addData(i, double(mMeasures.mSpeed));
//    ui->plot1->graph(2)->addData(i, 0.5 + sin(i/8.0f) * 0.5);
//    ui->plot1->graph(3)->addData(i, 0.5 + sin(i/7.0f) * 0.5);
//    ui->plot1->graph(4)->addData(i, 0.5 + sin(i/6.0f) * 0.5);
	ui->plot2->xAxis->setRange(i-360, i);
    ui->plot2->replot();

	ui->plot1->graph(0)->addData(i, double(mMeasures.mMAP - 1.0f));
	ui->plot1->graph(1)->addData(i, double(mMeasures.mThrottle / 100.0f));
	ui->plot1->graph(2)->addData(i, double(mMeasures.mThrottleDeriv / 10000.0f));
	ui->plot1->graph(3)->addData(i, double(mMeasures.mTargetBoost - 1.0f));
	ui->plot1->graph(4)->addData(i, double(mMeasures.mTargetOutput - 1.0f));
    ui->plot1->xAxis->setRange(i-360, i);
    ui->plot1->replot();

    if (time(nullptr) - mLastReadResult >= 2)
    {
        // timeout, restart read
        mReadQueue.clear();
        mLastReadResult = time(nullptr);
    }

	bool mutOpen = mMutReader.isOpen();
	MutData mutData = mMutReader.getData();

	if (mutOpen)
	{
		ui->knockSum->setText(QString::number(mutData.mKnockCount));
	}
	else
	{
		ui->knockSum->setText("---");
	}

	// logdata
	mLogger.addLogline(
				&mMeasures.mThrottle,
				&mMeasures.mRPM,
				&mMeasures.mSpeed,
				mMeasures.mGear,
				&mutData.mIgnAdv,
				mutOpen ? &mutData.mFuelTrimLow : nullptr,
				mutOpen ? &mutData.mFuelTrimMid : nullptr,
				mutOpen ? &mutData.mFuelTrimHigh : nullptr,
				&mMeasures.mMAP,
				&mMeasures.mTargetBoost,
				&mMeasures.mSolDC,
				mutOpen ? &mutData.mKnockCount : nullptr,
				mutOpen ? &mutData.mO2FuelTrim : nullptr,
				mutOpen ? &mutData.mO2Front : nullptr,
				mutOpen ? &mutData.mO2Rear : nullptr,
				mutOpen ? &mutData.mIngPulseFront : nullptr,
				mutOpen ? &mutData.mIngPulseRear : nullptr,
				mutOpen ? &mutData.mAccelEnrichment : nullptr
				);

	// next read ?
	if (mReadQueue.empty())
    {
        // trigger a new reading
        mReadQueue.push_back({RPM, true});
        mReadQueue.push_back({SPEED, true});
        mReadQueue.push_back({MAP, true});
        mReadQueue.push_back({THROTTLE, true});
        mReadQueue.push_back({SOL_DC, true});
		mReadQueue.push_back({GEAR, false});
        mReadQueue.push_back({LOAD, true});
        mReadQueue.push_back({CPU_LOAD, true});
        mReadQueue.push_back({TARGET_BOOST, true});
        mReadQueue.push_back({TARGET_OUTPUT, true});
        mReadQueue.push_back({THROTTLE_DERIV, true});
        processRead();
    }
}

void MainWindow::reloadConfig()
{
    // read all configuration data from controler
	mReadQueue.push_back({VERSION_MAJOR, false});
	mReadQueue.push_back({VERSION_MINOR, false});
	mReadQueue.push_back({TYPE_CIRC, true});
	mReadQueue.push_back({GEAR_RATIO_1, true});
    mReadQueue.push_back({GEAR_RATIO_2, true});
    mReadQueue.push_back({GEAR_RATIO_3, true});
    mReadQueue.push_back({GEAR_RATIO_4, true});
    mReadQueue.push_back({GEAR_RATIO_5, true});
    mReadQueue.push_back({GEAR_RATIO_6, true});
    mReadQueue.push_back({SPEED_SENSOR_CAL, true});
    mReadQueue.push_back({REF_BOOST, true});
    mReadQueue.push_back({PID_P, true});
    mReadQueue.push_back({PID_I, true});
    mReadQueue.push_back({PID_D, true});
	mReadQueue.push_back({FORCE_WG, true});

    // read boost table
    for (size_t i=0; i<6; ++i)
    {
        for (size_t j=0; j<7; ++j)
        {
			mReadQueue.push_back({(BC_ADDR)(BOOST_TABLE_1 + i * 0x10 + j), false});
        }
    }

    processRead();
}

void MainWindow::connectPort()
{
	if (mSnxComm != nullptr)
	{
		return;
	}
//	mSnxProtocol = new SnxProtocol();
	mSnxComm = new SnxComm(this);
	bool ok = mSnxComm->open("\\\\.\\" + ui->comboBox->currentText());
	if (! ok)
	{
		QMessageBox::warning(this, "Failed to open serial port",
							 "The serial port open command failed");
		delete mSnxComm;
		mSnxComm = nullptr;
		ui->actionConnect->setChecked(false);
		ui->openButton->setEnabled(true);

		return;
	}
//	mSnxProtocol->open("\\\\.\\" + ui->comboBox->currentText());
	ui->closeButton->setEnabled(true);
    ui->openButton->setEnabled(false);
    ui->reloadButton->setEnabled(true);
	ui->actionConnect->setChecked(true);

    // connect event
//	connect(mSnxProtocol, &SnxProtocol::onReadBytes, this, &MainWindow::onReadBytes);
//	connect(mSnxProtocol, &SnxProtocol::onReadMemory, this, &MainWindow::onReadMemory);
//	connect(mSnxProtocol, &SnxProtocol::onWriteAck, this, &MainWindow::onWriteAck);

	//	connect(mSnxProtocol, &SnxProtocol::onError, this, &MainWindow::onError);
	connect(mSnxComm, &SnxComm::onWriteByte, this, &MainWindow::onWriteByte);
	connect(mSnxComm, &SnxComm::onWriteFloat, this, &MainWindow::onWriteFloat);
	connect(mSnxComm, &SnxComm::onAddrError, this, &MainWindow::onAddrFloat);

	mTimer->start(50);

    reloadConfig();
}

void MainWindow::closePort()
{
	if (mSnxComm != nullptr)
	{
		mTimer->stop();
	//	mSnxProtocol->close();
	//	delete mSnxProtocol;
	//	mSnxProtocol = nullptr;
		mSnxComm->close();
		delete mSnxComm;
		mSnxComm = nullptr;
		ui->openButton->setEnabled(true);
		ui->closeButton->setEnabled(false);
		ui->reloadButton->setEnabled(false);
		ui->actionConnect->setChecked(false);
	}
}

void MainWindow::onReadBytes(const std::vector<SnxProtocol::AddrInfo>& readInfo)
{
	auto setVersion = [&]()
	{
		ui->firmwareVersion->setText(
					QString::number((int)this->mConfigData.mVersionMajor)
					+ "."
					+ QString::number((int)this->mConfigData.mVersionMinor));
	};

	for (const auto addrInfo : readInfo)
	{
		switch (addrInfo.mAddr)
		{
		case VERSION_MAJOR:
			this->mConfigData.mVersionMajor = addrInfo.mValue;
			setVersion();
			break;
		case VERSION_MINOR:
			this->mConfigData.mVersionMinor = addrInfo.mValue;
			setVersion();
			break;
		case GEAR:
			ui->gearEdit->setText(QString::number((int)addrInfo.mValue));
			break;
		default:
		{
			// check if the addr is in the current displayed table
			auto it = mAddrToTableItem.find(addrInfo.mAddr);
			if (it != mAddrToTableItem.end())
			{
				// update the table
				ui->tableWidget->item(it->second.first, it->second.second)
						->setText(QString::number(int(addrInfo.mValue)));
			}
			else
			{
				qDebug("Received addr 0x%4x, unknwon", addrInfo.mAddr);
			}
		}
		}
	}

	// trigger next read
	processRead();
}

void MainWindow::onReadMemory(uint32_t addr, const std::vector<uint8_t>& data)
{}

void MainWindow::onWriteAck()
{
}

void MainWindow::onError(SnxProtocol::Error error)
{
}

MainWindow::~MainWindow()
{
	delete ui;
}

uint8_t MainWindow::getMutData(uint8_t varId)
{
	static float latchedRPM = 0.0f;
	switch (varId)
	{
	case 0x2f:
		// Speed, 0.5 * v (km/h)
		return mMeasures.mSpeed * 0.5f;
	case 0x38:
		// MAP (boost), P / 0.01334 (bar)
		return mMeasures.mMAP / 0.1334;
	case 0x02:
		// RPM low, latch RPM.
		// (RPM * 256 / 1000) & 0xff
		latchedRPM = mMeasures.mRPM;
		return int(latchedRPM * 256.f / 1000.f) & 0xff;
	case 0x03:
		// RPM hi, latched RPM
		// (RPM * 256 / 1000) >> 8
		return (int(latchedRPM * 256.f / 1000.f) >> 8) & 0xff;
	}

	return 0;
}

void MainWindow::onWriteByte(uint16_t addr, uint8_t value)
{
	auto setVersion = [&]()
	{
		ui->firmwareVersion->setText(
					QString::number((int)this->mConfigData.mVersionMajor)
					+ "."
					+ QString::number((int)this->mConfigData.mVersionMinor));
	};

	switch (addr)
    {
	case VERSION_MAJOR:
		this->mConfigData.mVersionMajor = value;
		setVersion();
		break;
	case VERSION_MINOR:
		this->mConfigData.mVersionMinor = value;
		setVersion();
		break;
	case GEAR:
        ui->gearEdit->setText(QString::number((int)value));
		mMeasures.mGear = value;
		break;
	case FORCE_WG:
		mSimulationData.mForceWG = value;
		ui->forceWGEdit->setText(QString::number((int)value));
		break;
	default:
    {
        // check if the addr is in the current displayed table
        auto it = mAddrToTableItem.find(addr);
        if (it != mAddrToTableItem.end())
        {
            // update the table
            ui->tableWidget->item(it->second.first, it->second.second)
                    ->setText(QString::number(int(value)));
        }
        else
        {
            qDebug("Received addr 0x%4x, unknwon", addr);
        }
    }
    }

    // trigger next read
    processRead();
}

void MainWindow::onAddrFloat(uint16_t addr)
{
    qDebug("Received error on %u", addr);
    processRead();
}

void MainWindow::onWriteFloat(uint16_t addr, float value)
{
//    qDebug("Received float %u=%f", addr, value);
    switch (addr)
    {
    case TYPE_CIRC:
        ui->tyreCircumEdit->setText(QString::number(value, 'f', 2));
        mConfigData.mTyreSize = value;
        break;
    case GEAR_RATIO_1:
        ui->gear1Edit->setText(QString::number(value, 'f', 3));
        mConfigData.mGearRatio[0] = value;
        break;
    case GEAR_RATIO_2:
        ui->gear2Edit->setText(QString::number(value, 'f', 3));
        mConfigData.mGearRatio[1] = value;
        break;
    case GEAR_RATIO_3:
        ui->gear3Edit->setText(QString::number(value, 'f', 3));
        mConfigData.mGearRatio[2] = value;
        break;
    case GEAR_RATIO_4:
        ui->gear4Edit->setText(QString::number(value, 'f', 3));
        mConfigData.mGearRatio[3] = value;
        break;
    case GEAR_RATIO_5:
        ui->gear5Edit->setText(QString::number(value, 'f', 3));
        mConfigData.mGearRatio[4] = value;
        break;
    case GEAR_RATIO_6:
        ui->gear6Edit->setText(QString::number(value, 'f', 3));
        mConfigData.mGearRatio[5] = value;
        break;
    case SPEED_SENSOR_CAL:
        ui->speedRatioEdit->setText(QString::number(value, 'f', 3));
        mConfigData.mSpeedSensorRatio = value;
        break;
    case REF_BOOST:
        ui->baseBoostEdit->setText(QString::number(value, 'f', 2));
        mConfigData.mReferenceBoost = value;
        break;
    case PID_P:
        ui->pidPEdit->setText(QString::number(value, 'f', 4));
        mConfigData.mPidP = value;
        break;
    case PID_I:
        ui->pidIEdit->setText(QString::number(value, 'f', 4));
        mConfigData.mPidI = value;
        break;
    case PID_D:
        ui->pidDEdit->setText(QString::number(value, 'f', 4));
        mConfigData.mPidD = value;
        break;

    case RPM:
        ui->rpmEdit->setText(QString::number(value, 'f', 0));
        mMeasures.mRPM = value;
        break;
    case SPEED:
        ui->speedEdit->setText(QString::number(value, 'f', 1));
        mMeasures.mSpeed = value;
        break;
    case MAP:
		ui->mapEdit->setText(QString::number(value-1.0f, 'f', 2));
        mMeasures.mMAP = value;
        break;
    case THROTTLE:
    {
        ui->throttleEdit->setText(QString::number(value, 'f', 0));
        mMeasures.mThrottle = value;
        break;
    }
	case TARGET_BOOST:
		ui->targetBoostEdit->setText(QString::number(value-1.0f, 'f', 2));
        mMeasures.mTargetBoost = value;
        break;
    case TARGET_OUTPUT:
        ui->targetOutputEdit->setText(QString::number(value, 'f', 5));
        mMeasures.mTargetOutput = value;
        break;
    case SOL_DC:
        ui->solDcEdit->setText(QString::number(value, 'f', 0));
        mMeasures.mSolDC = value;
        break;
    case LOAD:
        ui->loadEdit->setText(QString::number(value, 'f', 0));
        mMeasures.mLoad = value;
        break;
    case CPU_LOAD:
        ui->cpuLoadEdit->setText(QString::number(value, 'f', 0));
        mMeasures.mCPULoad = value;
        break;
    case THROTTLE_DERIV:
//        ui->cpuLoadEdit->setText(QString::number(value, 'f', 0));
        mMeasures.mThrottleDeriv = value;
        break;
    }

//    QThread::msleep(500);
    // trigger next read
    processRead();
}

void MainWindow::on_reloadButton_clicked()
{
    reloadConfig();
}

void MainWindow::processRead()
{
    if (!mReadQueue.empty())
    {
        const ReadItem& ri = mReadQueue.front();
        if (mSnxComm != nullptr)
        {
            if (ri.mIsFloat)
            {
                qDebug("Reading float %u", ri.mAddr);
                mSnxComm->readFloat(ri.mAddr);
            }
            else
            {
                mSnxComm->readByte(ri.mAddr);
            }
        }

        mReadQueue.pop_front();
    }

    mLastReadResult = time(nullptr);
}

void MainWindow::on_tyreCircumEdit_editingFinished()
{
    mConfigData.mTyreSize = ui->tyreCircumEdit->text().toFloat();
    if (mSnxComm != nullptr)
        mSnxComm->writeFloat(TYPE_CIRC, mConfigData.mTyreSize);
}

void MainWindow::on_baseBoostEdit_editingFinished()
{
	mConfigData.mReferenceBoost = ui->baseBoostEdit->text().toFloat();
	if (mSnxComm != nullptr)
		mSnxComm->writeFloat(REF_BOOST, mConfigData.mReferenceBoost);
}

void MainWindow::on_pidPEdit_editingFinished()
{
    mConfigData.mPidP = ui->pidPEdit->text().toFloat();
    if (mSnxComm != nullptr)
        mSnxComm->writeFloat(PID_P, mConfigData.mPidP);
}

void MainWindow::on_pidIEdit_editingFinished()
{
    mConfigData.mPidI = ui->pidIEdit->text().toFloat();
    if (mSnxComm != nullptr)
        mSnxComm->writeFloat(PID_I, mConfigData.mPidI);
}

void MainWindow::on_pidDEdit_editingFinished()
{
    mConfigData.mPidD = ui->pidDEdit->text().toFloat();
    if (mSnxComm != nullptr)
        mSnxComm->writeFloat(PID_D, mConfigData.mPidD);
}

void MainWindow::on_buttonOpenLog_clicked()
{

}

void MainWindow::on_forceWGEdit_editingFinished()
{
	mSimulationData.mForceWG = ui->forceWGEdit->text().toInt();
	if (mSnxComm != nullptr)
		mSnxComm->writeByte(FORCE_WG, mSimulationData.mForceWG);
}

QColor percentToColor(float percent)
{
    percent = std::max(0.0f, percent);
    percent = std::min(100.0f, percent);
	percent /= 100.0f;
    float r = 255 * (percent);
    float b = 255 * (1-percent);
	return QColor(int(r), 20, int(b));
}

void MainWindow::on_table_itemChanged(QTableWidgetItem *item)
{
    if (item->text() == "--")
        return;
    float value = item->text().toFloat();
    item->setBackgroundColor(percentToColor(value));

    if (mSnxComm != nullptr)
    {
        uint16_t addr = (uint16_t)(item->data(Qt::UserRole).toInt());
        mSnxComm->writeByte(addr, uint8_t(value));
    }
}

void MainWindow::createTableWidget(QString name,
                                   QString rowLabel,
                                   QString columnLabel,
                                   const std::vector<QString>& rowNames,
                                   const std::vector<QString>& columnNames,
                                   uint16_t baseAddr, uint16_t rowStride)
{
    QTableWidget* table = new QTableWidget();

    table->setFont(QFont("Segeo UI", 12));
    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    table->sizePolicy().setHorizontalStretch(2);
//    table->sizePolicy().setVerticalStretch(0);
    table->setWordWrap(false);
    table->setCornerButtonEnabled(false);
    table->setRowCount(rowNames.size());
    table->setColumnCount(columnNames.size());
    table->horizontalHeader()->setCascadingSectionResizes(false);
    table->horizontalHeader()->setDefaultSectionSize(50);
    table->verticalHeader()->setCascadingSectionResizes(true);
    table->verticalHeader()->setDefaultSectionSize(30);
    table->verticalHeader()->setHighlightSections(true);
    table->verticalHeader()->setMinimumSectionSize(30);
    table->verticalHeader()->setSortIndicatorShown(false);
    table->verticalHeader()->setStretchLastSection(false);

    table->horizontalHeader()->setVisible(true);
    table->verticalHeader()->setVisible(true);

    connect(table, &QTableWidget::itemChanged, this, &MainWindow::on_table_itemChanged);

    mAddrToTableItem.clear();

    for (int i=0; i<rowNames.size(); ++i)
    {
        table->setVerticalHeaderItem(i, new QTableWidgetItem(rowNames[i], 0));
        for (int j=0; j<columnNames.size(); ++j)
        {
            uint16_t addr = baseAddr + j + rowStride * i;

            table->setHorizontalHeaderItem(j, new QTableWidgetItem(columnNames[j], 0));
            auto item = new QTableWidgetItem(QString::number(0), 0);
            item->setTextAlignment(Qt::AlignRight);
            item->setForeground(QColor(255,255,255));
            item->setData(Qt::UserRole, QVariant(addr));
            item->setText("--");
            table->setItem(i, j, item);

            mAddrToTableItem[addr] = std::make_pair(i, j);

            mReadQueue.push_back({(BC_ADDR)addr, false});
        }
    }

//    ((QGridLayout*)ui->frameForTable->layout())->addWidget(table, 1, 1);
    ((QGridLayout*)ui->frameForTable->layout())->replaceWidget(ui->tableWidget, table);
    delete ui->tableWidget;
    ui->tableWidget = table;

    ui->tableRowLabel->setText(rowLabel);
    ui->tableColumnLabel->setText(columnLabel);

    // trigger a read cycle
    processRead();
}

void MainWindow::on_listTable_itemActivated(QListWidgetItem *item)
{
}

void MainWindow::on_listTable_itemSelectionChanged()
{
    auto name = ui->listTable->currentItem()->text();
    auto it = mTableWidgets.find(name);

    if (it != mTableWidgets.end())
    {
        TableInfo& ti = it->second;
        createTableWidget(
                    name,
                    ti.rowLabel, ti.columnLabel,
                    ti.rowNames, ti.columnNames,
                    ti.baseAddr, ti.rowStride);
    }
}

void MainWindow::on_actionStart_Log_changed()
{
	if (ui->actionStart_Log->isChecked())
	{
		mLogger.startLog();
	}
	else
	{
		mLogger.stopLog();
	}
}

void MainWindow::on_actionConnect_changed()
{
	if (ui->actionConnect->isChecked())
	{
		connectPort();
	}
	else
	{
		closePort();
	}
}

void MainWindow::on_openButton_clicked()
{

}

void MainWindow::on_buttonOpenMut_clicked()
{
	bool ok = mMutReader.open("\\\\.\\" + ui->portListMut->currentText());

	if (! ok)
	{
		QMessageBox::warning(this, "Failed to open serial port",
							 "The serial port open command failed");
//		ui->actionConnect->setChecked(false);
		ui->buttonOpenMut->setEnabled(true);

		return;
	}

	ui->buttonOpenMut->setEnabled(false);
	ui->buttonCloseMut->setEnabled(true);
	ui->actionConnectMut->setChecked(true);
}

void MainWindow::on_buttonCloseMut_clicked()
{
	mMutReader.close();
	ui->buttonOpenMut->setEnabled(true);
	ui->buttonCloseMut->setEnabled(false);
	ui->actionConnectMut->setChecked(false);
}

void MainWindow::on_actionConnectMut_changed()
{
	if (ui->actionConnectMut->isChecked())
	{
		on_buttonOpenMut_clicked();
	}
	else
	{
		on_buttonCloseMut_clicked();
	}
}
