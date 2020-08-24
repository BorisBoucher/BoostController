#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plotter.h"
#include <qtimer.h>
#include <QtSerialPort/qserialportinfo.h>
#include <algorithm>
#include "setting.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	QCoreApplication::setOrganizationName("SnxLab");
	QCoreApplication::setOrganizationDomain("snxlab.io");
	QCoreApplication::setApplicationName("Snx BC Configurator");

	QSettings settings;

	mConfig.mOutputDir = settings.value(
			"logDir",
			QStandardPaths::writableLocation(
					QStandardPaths::StandardLocation::AppDataLocation)).toString();

	mConfig.mBCPort = settings.value(
			"BCPort",
			"").toString();

	mConfig.mMutPort = settings.value(
			"MUTPort",
			"").toString();

	// Create log directory
	QDir dir;
	dir.mkpath(mConfig.mOutputDir);

	initAccessors();
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

	ui->comboBox->setCurrentIndex(ui->comboBox->findText(mConfig.mBCPort));
	ui->portListMut->setCurrentIndex(ui->portListMut->findText(mConfig.mMutPort));

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
	ui->plot1->xAxis->setRange(0, 10);
	ui->plot1->yAxis->setRange(-0.5, 1.7);
	// Place legend int lower left cornet
	ui->plot1->axisRect()->insetLayout()
			->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignLeft);
	ui->plot1->replot();

	ui->plot1->setInteraction(QCP::iRangeDrag, true);

	ui->plot1->xAxis->setRange(0, 10);
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

	mTableWidgets["Boost Table"] = TableInfo
	{
			"Boost Table",
			"Gear", "RPM",
			{"1", "2", "3", "4", "5", "6"},
			{"1000", "2000", "3000", "4000", "5000", "6000", "7000"},
			BOOST_GEAR_1, 7
	};
	ui->listTable->addItem("Boost Table");

	mTableWidgets["Throttle Adjust"] = TableInfo
	{
			"Throttle Adjust",
			"Boost", "Throttle",
			{"%"},
			{"0%", "20%", "40%", "60%", "80%", "100%"},
			THROTTLE_ADJ, 0x0
	};
	ui->listTable->addItem("Throttle Adjust");

	mTableWidgets["Throttle Move Adjust"] = TableInfo
	{
			"Throttle Move Adjust",
			"Boost", "Throttle Move (%/s)",
			{"%"},
			{"-1000", "-660", "-330", "0", "+330", "+660", "+1000"},
			THROTTLE_DER_ADJ, 0x0
	};
	ui->listTable->addItem("Throttle Move Adjust");

	mTableWidgets["Sol DC"] = TableInfo
	{
			"Sol DC",
			"Target\nBoost\n(Bar)", "Load",
			{"0.0", "0.25", "0.5", "0.75", "1.0", "1.25"},
			{"25%", "50%", "75%", "100%"},
			DC_TABLE_1, 4
	};
	ui->listTable->addItem("Sol DC");

	auto layout = new QVBoxLayout();
	layout->addWidget(new Plotter);
	ui->tabPlotter->setLayout(layout);
	//	auto plotter = new Plotter();
	//	plotter->setParent(ui->tabPlotter);
}

void MainWindow::processFastcomReads()
{
	if (mFastCom != nullptr)
	{
		std::set<uint16_t> addrs;
		const std::vector<FastCom::ReadResults> vrr = mFastCom->getReadResult();

		for (auto& rr : vrr)
		{
			for (auto& v : rr.mValues)
			{
				addrs.insert(v.mAddr);
				const Accessor& acc = mAccessors[v.mAddr];
				if (acc.mIsFloat)
				{
					*reinterpret_cast<float*>(reinterpret_cast<char*>(this)+acc.mOffset) = v.mValue;
				}
				else
				{
					if (!acc.mSigned)
					{
						*reinterpret_cast<int8_t*>(reinterpret_cast<char*>(this)+acc.mOffset) = static_cast<uint8_t>(int8_t(v.mValue));
					}
					else
					{
						*reinterpret_cast<int8_t*>(reinterpret_cast<char*>(this)+acc.mOffset) = int8_t(v.mValue);
					}
				}
			}
		}

		notifyReads(addrs);
	}
}

void MainWindow::fastComWrite(const std::initializer_list<uint16_t>& addrs)
{
	if (mFastCom != nullptr)
	{
		std::vector<FastCom::WriteCommand> cmds;
		for (auto addr : addrs)
		{
			const Accessor& acc = mAccessors[addr];

			float value;
			if (acc.mIsFloat)
			{
				value = *reinterpret_cast<float*>(reinterpret_cast<char*>(this)+acc.mOffset);
			}
			else
			{
				if (!acc.mSigned)
				{
					value = *reinterpret_cast<uint8_t*>(reinterpret_cast<char*>(this)+acc.mOffset);
				}
				else
				{
					value = *reinterpret_cast<int8_t*>(reinterpret_cast<char*>(this)+acc.mOffset);
				}
			}

			cmds.push_back({addr, value, acc.mFactor, acc.mSigned});
		}

		mFastCom->writeValues(cmds);
	}
}

void MainWindow::readFastCom(const std::initializer_list<uint16_t>& addrs)
{
	std::vector<FastCom::ReadCommand> reads;

	for (auto addr : addrs)
	{
		const Accessor& acc = mAccessors[addr];

		reads.push_back({addr, acc.mFactor, acc.mSigned});
	}

	if (mFastCom != nullptr)
	{
		mFastCom->readValues(reads);
	}
}

void MainWindow::setPeriodicRead(const std::initializer_list<std::pair<uint16_t, size_t>>& addrs)
{
	std::vector<FastCom::PeriodicReadCommand> reads;

	for (auto addr : addrs)
	{
		const Accessor& acc = mAccessors[addr.first];

		reads.push_back({{addr.first, acc.mFactor, acc.mSigned}, addr.second});
	}

	if (mFastCom != nullptr)
	{
		mFastCom->setPrediodicRead(reads);
	}
}

uint8_t* MainWindow::getByteParam(uint16_t addr)
{
	auto it = mAccessors.find(addr);
	if (it == mAccessors.end() || it->second.mIsFloat)
	{
		return nullptr;
	}

	return reinterpret_cast<uint8_t*>(reinterpret_cast<char*>(this)+it->second.mOffset);
}

bool MainWindow::setByteParam(uint16_t addr, uint8_t value)
{
	bool ret = false;
	uint8_t* p = getByteParam(addr);
	if (p != nullptr)
	{
		ret = *p != value;
		*p = value;
	}

	return ret;
}

#define configOffset(MemberName) offsetof(MainWindow, mConfigData) + offsetof(ConfigData, MemberName)
#define measureOffset(MemberName) offsetof(MainWindow, mMeasures) + offsetof(Measures, MemberName)
#define simulationOffset(MemberName) offsetof(MainWindow, mSimulationData) + offsetof(SimulationData, MemberName)

void MainWindow::initAccessors()
{
	auto addAccessors = [&](const std::initializer_list<Accessor>& accs)
	{
		for (const auto& a : accs)
		{
			auto ib = mAccessors.insert(std::make_pair(a.mAddr, a));
			assert(ib.second);
		}
	};

	// Scalar data
	addAccessors(
	{
		//float? addr         factor signed? offset
		{true,  MAP,           50.0f, false, measureOffset(mMAP)			},	// MAP
		{true,  THROTTLE,       1.0f, false, measureOffset(mThrottle)		},	// Throttle
		{true,  WGDC,           1.0f, false, measureOffset(mSolDC)			},	// WFDC
		{false, GEAR,           1.0f, false, measureOffset(mGear)			},	// Gear
		{true,  ENG_LOAD,       1.0f, false, measureOffset(mLoad)			},	// Engine load
		{true,  CPU_LOAD,       1.0f, false, measureOffset(mCPULoad)		},	// CPU load
		{true,  TARGET_BOOST,  50.0f, false, measureOffset(mTargetBoost)	},	// Target boost
		{true,  TARGET_OUTPUT, 50.0f, false, measureOffset(mTargetOutput)},	// Target output

		{false,  WGDC_TEST,      1.0f, false, simulationOffset(mForceWG)		},	// Force WG DC

		{false,  VERSION_MAJOR,  1.0f, false, configOffset(mVersionMajor)	},	// FW version major
		{false,  VERSION_MINOR,  1.0f, false, configOffset(mVersionMinor)	},	// FW version minor

		{true,  FUEL_PRESSURE,  25.0f, false, measureOffset(mFUEL_PRESSURE)	},	// Fuel Pressure

		{true,  TYRE_CIRC,      1000.0f, false, configOffset(mTyreSize)		},	// Tyre circ
		{true,  SPEED_RATIO,    1000.0f, false, configOffset(mSpeedSensorRatio)},
		{true,  REF_BOOST,      1000.0f, false, configOffset(mReferenceBoost)	},
		{true,  PID_P,          1000.0f, false, configOffset(mPidP)			},
		{true,  PID_I,          1000.0f, false, configOffset(mPidI)			},
		{true,  PID_D,          1000.0f, false, configOffset(mPidD)			},
		{true,  RPM,               1.0f, false, measureOffset(mRPM)			},
		{true,  SPEED,             1.0f, false, measureOffset(mSpeed)		},
		{true,  THROTTLE_DERIV,    1.0f, true,  measureOffset(mThrottleDeriv)},
		{true,  AIR_FLOW,          1.0f, false, measureOffset(mAirFlow)		},
		{false, MAX_THROTTLE,      1.0f, false, configOffset(mMaxThrottle)	},
		{false, FUEL_PUMP_PRIME,   1.0f, false, configOffset(mFuelPumpPrime)},
	});

	// Tables
	// -----------------

	// Gear ratio
	for(uint16_t i=0; i<6; i++)
	{
		uint16_t addr = GEAR_RATIO_1 + i;
		addAccessors(
		{
			//   float? addr   factor  signed? offset
			{true, addr, 1000.0f, false, configOffset(mGearRatio[i])},	// gear ratio
		});
	}

	// boost x load => WGDC
	for (uint16_t i = 0; i< 6; ++i)
	{
		for (uint16_t j = 0; j< 4; ++j)
		{
			uint16_t addr = DC_TABLE_1 + i*4 + j;
			addAccessors(
			{
				//   float? addr   factor signed? offset
				{false,  addr, 1.0f, false, configOffset(mWGDCTable[i][j])},	// Boost x load to WGDC
			});
		}
	}

	// gear x RPM => boost correction
	for (uint16_t i = 0; i< 6; ++i)
	{
		for (uint16_t j = 0; j< 7; ++j)
		{
			uint16_t addr = BOOST_GEAR_1 + i*7 + j;
			addAccessors(
			{
				//   float? addr   factor signed? offset
				{false,  addr, 1.0f, false, configOffset(mBoostTable[i][j])},	// gear x RPM => boost correction
			});
		}
	}

	// Throttle => boost correction
	for (uint16_t i = 0; i< 6; ++i)
	{
		uint16_t addr = THROTTLE_ADJ + i;
		addAccessors(
		{
			//   float? addr   factor signed? offset
			{false,  addr, 1.0f, false, configOffset(mThrottleTable[i])},	// Throttle => boost correction
		});
	}

	// Throttle deriv => boost correction
	for (uint16_t i = 0; i< 7; ++i)
	{
		uint16_t addr = THROTTLE_DER_ADJ + i;
		addAccessors(
		{
			//   float? addr   factor signed? offset
			{false,  addr, 1.0f, false, configOffset(mThrottleDerivTable[i])},	// Throttle deriv => boost correction
		});
	}
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
	(void) event;
	//    mChart->resize(ui->graphicsView->size());
}

void MainWindow::sceneRectChanged(const QRectF &/*rect*/)
{
	//    mChart->resize(ui->graphicsView->size());
}

void MainWindow::update()
{
	processFastcomReads();

	static auto lastGraph = std::chrono::steady_clock::now();
	static auto start = std::chrono::steady_clock::now();
	auto now = std::chrono::steady_clock::now();
	auto dur = now - start;
	auto i = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count()/1000.0;

//	if (now - lastGraph > std::chrono::milliseconds(100))
	{
		ui->plot2->graph(0)->addData(i, double(mMeasures.mRPM));
		ui->plot2->graph(1)->addData(i, double(mMeasures.mSpeed));
		ui->plot2->xAxis->setRange(i-10, i);
		ui->plot2->replot();

		ui->plot1->graph(0)->addData(i, double(mMeasures.mMAP - 1.0f));
		ui->plot1->graph(1)->addData(i, double(mMeasures.mThrottle / 100.0f));
		ui->plot1->graph(2)->addData(i, double(mMeasures.mThrottleDeriv / 10000.0f));
		ui->plot1->graph(3)->addData(i, double(mMeasures.mTargetBoost - 1.0f));
		ui->plot1->graph(4)->addData(i, double(mMeasures.mTargetOutput - 1.0f));
		ui->plot1->xAxis->setRange(i-10, i);
		ui->plot1->replot();
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
				&mMeasures.mFUEL_PRESSURE,
				&mMeasures.mMAP,
				&mMeasures.mTargetBoost,
				&mMeasures.mSolDC,
				mutOpen ? &mutData.mKnockCount : nullptr,
				mutOpen ? &mutData.mO2FuelTrim : nullptr,
				mutOpen ? &mutData.mO2Front : nullptr,
				mutOpen ? &mutData.mO2Rear : nullptr,
				mutOpen ? &mutData.mIngPulseFront : nullptr,
				mutOpen ? &mutData.mIngPulseRear : nullptr,
				mutOpen ? &mutData.mAccelEnrichment : nullptr,
				&mMeasures.mAirFlow
				);
}

void MainWindow::reloadConfig()
{
	// read all configuration data from controler
	readFastCom(
				{
					VERSION_MAJOR,
					VERSION_MINOR,
					TYRE_CIRC,
					GEAR_RATIO_1,
					GEAR_RATIO_2,
					GEAR_RATIO_3,
					GEAR_RATIO_4,
					GEAR_RATIO_5,
					GEAR_RATIO_6,
					SPEED_RATIO,
					REF_BOOST,
					PID_P,
					PID_I,
					PID_D,
					WGDC_TEST,
					MAX_THROTTLE,
					FUEL_PUMP_PRIME,
				});
}

void MainWindow::connectPort()
{
	if (mFastCom != nullptr)
	{
		return;
	}

	mFastCom = new FastCom(this);
	bool ok = mFastCom->open("\\\\.\\" + ui->comboBox->currentText());
	if (! ok)
	{
		QMessageBox::warning(this, "Failed to open serial port",
							 "The serial port open command failed");
		delete mFastCom;
		mFastCom = nullptr;
		ui->actionConnect->setChecked(false);
		ui->openButton->setEnabled(true);

		return;
	}

	ui->closeButton->setEnabled(true);
	ui->openButton->setEnabled(false);
	ui->reloadButton->setEnabled(true);
	ui->actionConnect->setChecked(true);

	mTimer->start(18);

	// Wait 3s for the arduino to reboot (thank to the auto reboot feature :(
//	QThread::sleep(3);

	reloadConfig();
	setPeriodicRead(
				{
					{RPM,            4},
					{SPEED,          4},
					{MAP,            1},
					{FUEL_PRESSURE,  1},
					{THROTTLE,       1},
					{WGDC,           1},
					{GEAR,           8},
					{ENG_LOAD,       4},
					{CPU_LOAD,       4},
					{TARGET_BOOST,   1},
					{TARGET_OUTPUT,  1},
					{THROTTLE_DERIV, 1},
					{AIR_FLOW,       2},
				});
}

void MainWindow::closePort()
{
	if (mFastCom != nullptr)
	{
		mTimer->stop();
		delete mFastCom;
		mFastCom = nullptr;
		ui->openButton->setEnabled(true);
		ui->closeButton->setEnabled(false);
		ui->reloadButton->setEnabled(false);
		ui->actionConnect->setChecked(false);
	}
}

void MainWindow::notifyReads(const std::set<uint16_t>& addrs)
{
	auto setVersion = [&]()
	{
		ui->firmwareVersion->setText(
					QString::number(static_cast<int>(this->mConfigData.mVersionMajor))
					+ "."
					+ QString::number(static_cast<int>(this->mConfigData.mVersionMinor)));
	};

	for (auto addr : addrs)
	{
		switch (addr)
		{
		case TYRE_CIRC:
			ui->tyreCircumEdit->setText(QString::number(mConfigData.mTyreSize, 'f', 2));
			break;
		case GEAR_RATIO_1:
			ui->gear1Edit->setText(QString::number(mConfigData.mGearRatio[0], 'f', 3));
			break;
		case GEAR_RATIO_2:
			ui->gear2Edit->setText(QString::number(double(mConfigData.mGearRatio[1]), 'f', 3));
			break;
		case GEAR_RATIO_3:
			ui->gear3Edit->setText(QString::number(double(mConfigData.mGearRatio[2]), 'f', 3));
			break;
		case GEAR_RATIO_4:
			ui->gear4Edit->setText(QString::number(double(mConfigData.mGearRatio[3]), 'f', 3));
			break;
		case GEAR_RATIO_5:
			ui->gear5Edit->setText(QString::number(double(mConfigData.mGearRatio[4]), 'f', 3));
			break;
		case GEAR_RATIO_6:
			ui->gear6Edit->setText(QString::number(double(mConfigData.mGearRatio[5]), 'f', 3));
			break;
		case SPEED_RATIO:
			ui->speedRatioEdit->setText(QString::number(double(mConfigData.mSpeedSensorRatio), 'f', 3));
			break;
		case REF_BOOST:
			ui->baseBoostEdit->setText(QString::number(double(mConfigData.mReferenceBoost), 'f', 2));
			break;
		case PID_P:
			ui->pidPEdit->setText(QString::number(double(mConfigData.mPidP), 'f', 4));
			break;
		case PID_I:
			ui->pidIEdit->setText(QString::number(double(mConfigData.mPidI), 'f', 4));
			break;
		case PID_D:
			ui->pidDEdit->setText(QString::number(double(mConfigData.mPidD), 'f', 4));
			break;

		case RPM:
			ui->rpmEdit->setText(QString::number(double(mMeasures.mRPM), 'f', 0));
			break;
		case SPEED:
			ui->speedEdit->setText(QString::number(double(mMeasures.mSpeed ), 'f', 1));
			break;
		case MAP:
			ui->mapEdit->setText(QString::number(double(mMeasures.mMAP-1.0f), 'f', 2));
			break;
		case FUEL_PRESSURE:
			ui->fuelPressEdit->setText(QString::number(double(mMeasures.mFUEL_PRESSURE), 'f', 2));
			break;
		case THROTTLE:
		{
			ui->throttleEdit->setText(QString::number(double(mMeasures.mThrottle), 'f', 0));
			break;
		}
		case TARGET_BOOST:
			ui->targetBoostEdit->setText(QString::number(double(mMeasures.mTargetBoost-1.0f), 'f', 2));
			break;
		case TARGET_OUTPUT:
			ui->targetOutputEdit->setText(QString::number(double(mMeasures.mTargetOutput), 'f', 5));
			break;
		case WGDC:
			ui->solDcEdit->setText(QString::number(double(mMeasures.mSolDC), 'f', 0));
			break;
		case ENG_LOAD:
			ui->loadEdit->setText(QString::number(double(mMeasures.mLoad), 'f', 0));
			break;
		case CPU_LOAD:
			ui->cpuLoadEdit->setText(QString::number(double(mMeasures.mCPULoad), 'f', 0));
			break;
		case THROTTLE_DERIV:
			//			ui->throttleDevirvEdit->setText(QString::number(double(mMeasures.mThrottleDeriv), 'f', 0));
			break;
		case AIR_FLOW:
			ui->airFlowEdit->setText(QString::number(double(mMeasures.mAirFlow), 'f', 0));
			break;
		case VERSION_MAJOR:
			setVersion();
			break;
		case VERSION_MINOR:
			setVersion();
			break;
		case WGDC_TEST:
			ui->forceWGEdit->setText(QString::number(int(mSimulationData.mForceWG)));
			break;
		case GEAR:
			ui->gearEdit->setText(QString::number(int(mMeasures.mGear)));
			break;
		case MAX_THROTTLE:
			ui->maxThrottleEdit->setText(QString::number(int(mConfigData.mMaxThrottle)));
			break;
		case FUEL_PUMP_PRIME:
			ui->fuelPumpPrimeEdit->setText(QString::number(int(mConfigData.mFuelPumpPrime)));
			break;
		default:
			{
				// check if the addr is in the current displayed table
				auto it = mAddrToTableItem.find(addr);
				if (it != mAddrToTableItem.end())
				{
					auto p = getByteParam(addr);
					if (p != nullptr)
					{
						// update the table
						ui->tableWidget->item(it->second.first, it->second.second)
								->setText(QString::number(int(*p)));
					}
				}
				else
				{
					qDebug("Received addr 0x%4x, unknwon", addr);
				}
			}
		}
	}
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
		return mMeasures.mMAP / 0.1334f;
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

void MainWindow::on_reloadButton_clicked()
{
	reloadConfig();
}

void MainWindow::on_tyreCircumEdit_editingFinished()
{
	mConfigData.mTyreSize = ui->tyreCircumEdit->text().toFloat();
	fastComWrite({TYRE_CIRC});
}

void MainWindow::on_baseBoostEdit_editingFinished()
{
	mConfigData.mReferenceBoost = ui->baseBoostEdit->text().toFloat();
	fastComWrite({REF_BOOST});
}

void MainWindow::on_maxThrottleEdit_editingFinished()
{
	mConfigData.mMaxThrottle = ui->maxThrottleEdit->text().toUInt();
	fastComWrite({MAX_THROTTLE});
}

void MainWindow::on_fuelPumpPrimeEdit_editingFinished()
{
	mConfigData.mFuelPumpPrime = ui->fuelPumpPrimeEdit->text().toUInt();
	fastComWrite({FUEL_PUMP_PRIME});
}

void MainWindow::on_pidPEdit_editingFinished()
{
	mConfigData.mPidP = ui->pidPEdit->text().toFloat();
	fastComWrite({PID_P});
}

void MainWindow::on_pidIEdit_editingFinished()
{
	mConfigData.mPidI = ui->pidIEdit->text().toFloat();
	fastComWrite({PID_I});
}

void MainWindow::on_pidDEdit_editingFinished()
{
	mConfigData.mPidD = ui->pidDEdit->text().toFloat();
	fastComWrite({PID_D});
}

void MainWindow::on_forceWGEdit_editingFinished()
{
	mSimulationData.mForceWG = uint8_t(ui->forceWGEdit->text().toInt());
	fastComWrite({WGDC_TEST});
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

	uint16_t addr = static_cast<uint16_t>(item->data(Qt::UserRole).toInt());
	// ItemChanged signal is notified even for color change !
	if (setByteParam(addr, uint8_t(value)))
	{
		fastComWrite({addr});
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
	table->setRowCount(int(rowNames.size()));
	table->setColumnCount(int(columnNames.size()));
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

	for (size_t i=0; i<rowNames.size(); ++i)
	{
		table->setVerticalHeaderItem(int(i), new QTableWidgetItem(rowNames[i], 0));
		for (size_t j=0; j<columnNames.size(); ++j)
		{
			uint16_t addr = uint16_t(baseAddr + j + rowStride * i);

			table->setHorizontalHeaderItem(int(j), new QTableWidgetItem(columnNames[j], 0));
			auto item = new QTableWidgetItem(QString::number(0), 0);
			item->setTextAlignment(Qt::AlignRight);
			item->setForeground(QColor(255,255,255));
			item->setData(Qt::UserRole, QVariant(addr));
			item->setText("--");
			table->setItem(int(i), int(j), item);

			mAddrToTableItem[addr] = {int(i), int(j)};

			this->readFastCom({addr, 1, false});
		}
	}

	//    ((QGridLayout*)ui->frameForTable->layout())->addWidget(table, 1, 1);
	static_cast<QGridLayout*>(ui->frameForTable->layout())->replaceWidget(ui->tableWidget, table);
	delete ui->tableWidget;
	ui->tableWidget = table;

	ui->tableRowLabel->setText(rowLabel);
	ui->tableColumnLabel->setText(columnLabel);
}

void MainWindow::on_listTable_itemActivated(QListWidgetItem *item)
{
	(void) item;
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
		mLogger.startLog(mConfig.mOutputDir);
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

void MainWindow::on_actionConnect_triggered()
{

}
void MainWindow::on_gear1Edit_editingFinished()
{
	mConfigData.mGearRatio[0] = ui->gear1Edit->text().toFloat();
	fastComWrite({GEAR_RATIO_1});
}
void MainWindow::on_gear2Edit_editingFinished()
{
	mConfigData.mGearRatio[1] = ui->gear2Edit->text().toFloat();
	fastComWrite({GEAR_RATIO_2});
}
void MainWindow::on_gear3Edit_editingFinished()
{
	mConfigData.mGearRatio[2] = ui->gear3Edit->text().toFloat();
	fastComWrite({GEAR_RATIO_3});
}
void MainWindow::on_gear4Edit_editingFinished()
{
	mConfigData.mGearRatio[3] = ui->gear4Edit->text().toFloat();
	fastComWrite({GEAR_RATIO_4});
}
void MainWindow::on_gear5Edit_editingFinished()
{
	mConfigData.mGearRatio[4] = ui->gear5Edit->text().toFloat();
	fastComWrite({GEAR_RATIO_5});
}
void MainWindow::on_gear6Edit_editingFinished()
{
	mConfigData.mGearRatio[5] = ui->gear6Edit->text().toFloat();
	fastComWrite({GEAR_RATIO_6});
}

void MainWindow::on_speedRatioEdit_editingFinished()
{
	mConfigData.mSpeedSensorRatio = ui->speedRatioEdit->text().toFloat();
	fastComWrite({SPEED_RATIO});
}

void MainWindow::on_actionSetup_triggered()
{
	Setting setting(
				mConfig.mOutputDir,
				mConfig.mBCPort,
				mConfig.mMutPort,
				this);
	if (setting.exec() == QDialog::Accepted)
	{
		mConfig.mOutputDir = setting.getLogDir();
		mConfig.mBCPort = setting.getBCPort();
		mConfig.mMutPort = setting.getMUTPort();

		QSettings qs;
		qs.setValue("logDir", mConfig.mOutputDir);
		qs.setValue("BCPort", mConfig.mBCPort);
		qs.setValue("MUTPort", mConfig.mMutPort);

		QDir dir;
		dir.mkpath(mConfig.mOutputDir);

		ui->comboBox->setCurrentIndex(ui->comboBox->findText(mConfig.mBCPort));
		ui->portListMut->setCurrentIndex(ui->portListMut->findText(mConfig.mMutPort));
	}
}

void MainWindow::on_actionOpen_log_dir_triggered()
{
	QDesktopServices::openUrl(QUrl::fromLocalFile(mConfig.mOutputDir));
}
