#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <initializer_list>
#include <QMainWindow>
#include <deque>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QTableWidgetItem>
#include <QListWidgetItem>


//#include "../snxcomm/snxcomm.h"
#include "../snxcomm/fastcom.h"
#include "config_data.h"
#include "mutservice.h"
#include "logger.h"
#include "mutreader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public IMutDataProvider
{
    Q_OBJECT

	FastCom* mFastCom = nullptr;
	MutService	mMutService;
    QTimer* mTimer = nullptr;
//    QtCharts::QLineSeries *mSeries = nullptr;
//    QtCharts::QChart* mChart = nullptr;

	ConfigData  mConfigData;
	Measures    mMeasures;
	SimulationData   mSimulationData;

	Logger	mLogger;

	MutReader mMutReader;

	struct Accessor
	{
		// If true, data is stored as float, otherwize as byte.
		bool mIsFloat;
		uint16_t mAddr;
		float mFactor;
		bool mSigned;
		size_t mOffset;
	};

	std::map<uint16_t, Accessor> mAccessors;

	struct
	{
		QString	mOutputDir;
		QString mBCPort;
		QString mMutPort;
	} mConfig;

	void processFastcomReads();
	void fastComWrite(const std::initializer_list<uint16_t>& addrs);
	void readFastCom(const std::initializer_list<uint16_t>& addrs);
	void setPeriodicRead(const std::initializer_list<std::pair<uint16_t, size_t>>& addrs);
	void initAccessors();
	uint8_t* getByteParam(uint16_t addr);
	// Return true if the value changed effectively
	bool setByteParam(uint16_t addr, uint8_t value);

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow() override;

	virtual uint8_t	getMutData(uint8_t varId) override final;

	void notifyReads(const std::set<uint16_t>& addrs);
public slots:
    void sceneRectChanged(const QRectF &rect);
    void update();
    void reloadConfig();
    void connectPort();
    void closePort();

private slots:
    void on_reloadButton_clicked();

    void on_tyreCircumEdit_editingFinished();

    void on_pidPEdit_editingFinished();

    void on_pidIEdit_editingFinished();

    void on_pidDEdit_editingFinished();

	void on_table_itemChanged(QTableWidgetItem *item);

    void on_listTable_itemActivated(QListWidgetItem *item);

    void on_listTable_itemSelectionChanged();

	void on_baseBoostEdit_editingFinished();

	void on_maxThrottleEdit_editingFinished();

	void on_fuelPumpPrimeEdit_editingFinished();

	void on_forceWGEdit_editingFinished();

	void on_actionStart_Log_changed();

	void on_actionConnect_changed();

	void on_openButton_clicked();

	void on_buttonOpenMut_clicked();

	void on_buttonCloseMut_clicked();

	void on_actionConnectMut_changed();

	void on_actionConnect_triggered();

	void on_gear1Edit_editingFinished();
	void on_gear2Edit_editingFinished();
	void on_gear3Edit_editingFinished();
	void on_gear4Edit_editingFinished();
	void on_gear5Edit_editingFinished();
	void on_gear6Edit_editingFinished();

	void on_speedRatioEdit_editingFinished();

	void on_actionSetup_triggered();

	void on_actionOpen_log_dir_triggered();

private:

    struct TableInfo
    {
        QString name;
        QString rowLabel;
        QString columnLabel;
        std::vector<QString> rowNames;
        std::vector<QString> columnNames;
        uint16_t baseAddr;
        uint16_t rowStride;
    };

    std::map<QString, TableInfo> mTableWidgets;

	std::map<uint16_t, std::pair<int, int>> mAddrToTableItem;

    void createTableWidget(QString name,
                           QString rowLabel,
                           QString columnLabel,
                           const std::vector<QString>& rowNames,
                           const std::vector<QString>& columnNames,
                           uint16_t baseAddr, uint16_t rowStride);

	void resizeEvent(QResizeEvent *event) override;

    Ui::MainWindow *ui;

    int icounter = 0;
};

#endif // MAINWINDOW_H
