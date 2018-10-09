#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <deque>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QTableWidgetItem>
#include <QListWidgetItem>


#include "../snxcomm/snxcomm.h"
#include "config_data.h"
#include "mutservice.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public IMutDataProvider
{
    Q_OBJECT

	MutService	mMutService;
    SnxComm* mSnxComm = nullptr;
    QTimer* mTimer = nullptr;
//    QtCharts::QLineSeries *mSeries = nullptr;
//    QtCharts::QChart* mChart = nullptr;

    ConfigData  mConfigData;
    Measures    mMeasures;

    struct ReadItem
    {
        BC_ADDR mAddr;
        bool    mIsFloat;
    };

    std::deque<ReadItem>   mReadQueue;
    time_t  mLastReadResult = 0;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	virtual uint8_t	getMutData(uint8_t varId) override final;

public slots:
    void sceneRectChanged(const QRectF &rect);
    void update();
    void reloadConfig();
    void connectPort();
    void closePort();
    void onWriteByte(uint16_t addr, uint8_t value);
    void onWriteFloat(uint16_t addr, float value);
    void onAddrFloat(uint16_t addr);


private slots:
    void on_reloadButton_clicked();

    void on_tyreCircumEdit_editingFinished();

    void on_pidPEdit_editingFinished();

    void on_pidIEdit_editingFinished();

    void on_pidDEdit_editingFinished();

    void on_table_itemChanged(QTableWidgetItem *item);

    void on_listTable_itemActivated(QListWidgetItem *item);

    void on_listTable_itemSelectionChanged();

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

    void resizeEvent(QResizeEvent *event);


    void processRead();

    Ui::MainWindow *ui;

    int icounter = 0;
};

#endif // MAINWINDOW_H
