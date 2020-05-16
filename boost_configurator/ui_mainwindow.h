/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionStart_Log;
    QAction *actionConnect;
    QAction *actionConnectMut;
    QAction *actionSetup;
    QAction *actionOpen_log_dir;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_4;
    QFrame *frame_4;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_38;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout_4;
    QComboBox *comboBox;
    QPushButton *openButton;
    QPushButton *closeButton;
    QPushButton *reloadButton;
    QSpacerItem *horizontalSpacer_2;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout_5;
    QGridLayout *gridLayout_2;
    QLineEdit *gear2Edit;
    QLineEdit *gear4Edit;
    QFrame *line;
    QLineEdit *tyreCircumEdit;
    QLabel *label_16;
    QLineEdit *firmwareVersion;
    QPushButton *buttonCloseMut;
    QLabel *label_3;
    QLabel *label_8;
    QLineEdit *gear3Edit;
    QLabel *label_15;
    QLineEdit *pidIEdit;
    QLabel *label_41;
    QComboBox *portListMut;
    QLabel *label_6;
    QLabel *label_14;
    QLabel *label_7;
    QLabel *label_13;
    QLineEdit *gear1Edit;
    QLabel *label_2;
    QLabel *label;
    QFrame *line_2;
    QLabel *label_36;
    QLabel *label_12;
    QLabel *label_9;
    QPushButton *buttonOpenMut;
    QLabel *label_10;
    QLineEdit *gear5Edit;
    QLineEdit *maxThrottleEdit;
    QLineEdit *speedRatioEdit;
    QLabel *label_17;
    QLabel *label_5;
    QLineEdit *pidPEdit;
    QLineEdit *gear6Edit;
    QLabel *label_4;
    QLabel *label_11;
    QLineEdit *baseBoostEdit;
    QLineEdit *pidDEdit;
    QLabel *label_44;
    QLabel *label_45;
    QLineEdit *forceWGEdit;
    QLabel *label_40;
    QLabel *label_39;
    QLabel *label_49;
    QLineEdit *fuelPumpPrimeEdit;
    QLabel *label_50;
    QLabel *label_46;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;
    QWidget *tab_3;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QListWidget *listTable;
    QFrame *frameForTable;
    QGridLayout *gridLayout_4;
    QLabel *tableColumnLabel;
    QLabel *tableRowLabel;
    QTableWidget *tableWidget;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QGridLayout *gridLayout_5;
    QLabel *label_37;
    QLabel *label_24;
    QLabel *label_30;
    QLineEdit *knockSum;
    QLineEdit *targetOutputEdit;
    QLabel *label_29;
    QLineEdit *rpmEdit;
    QLabel *label_21;
    QLabel *label_32;
    QLabel *label_20;
    QLabel *label_28;
    QLabel *label_31;
    QLineEdit *gearEdit;
    QLabel *label_47;
    QLabel *label_23;
    QLineEdit *targetBoostEdit;
    QLabel *label_33;
    QLabel *label_34;
    QLabel *label_27;
    QLineEdit *solDcEdit;
    QLabel *label_43;
    QLabel *label_26;
    QLabel *label_19;
    QLineEdit *cpuLoadEdit;
    QLineEdit *throttleEdit;
    QLineEdit *airFlowEdit;
    QLabel *label_42;
    QLabel *label_25;
    QLineEdit *loadEdit;
    QLabel *label_18;
    QLineEdit *mapEdit;
    QLabel *label_22;
    QLineEdit *speedEdit;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_35;
    QLineEdit *fuelPressEdit;
    QLabel *label_48;
    QVBoxLayout *verticalLayout;
    QCustomPlot *plot1;
    QCustomPlot *plot2;
    QWidget *tabPlotter;
    QMenuBar *menuBar;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(965, 692);
        QIcon icon;
        icon.addFile(QString::fromUtf8("turbo.png"), QSize(), QIcon::Normal, QIcon::On);
        MainWindow->setWindowIcon(icon);
        actionStart_Log = new QAction(MainWindow);
        actionStart_Log->setObjectName(QString::fromUtf8("actionStart_Log"));
        actionStart_Log->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("log_file.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStart_Log->setIcon(icon1);
        actionConnect = new QAction(MainWindow);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        actionConnect->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("bluetooth.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConnect->setIcon(icon2);
        actionConnectMut = new QAction(MainWindow);
        actionConnectMut->setObjectName(QString::fromUtf8("actionConnectMut"));
        actionConnectMut->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8("MUT.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConnectMut->setIcon(icon3);
        actionSetup = new QAction(MainWindow);
        actionSetup->setObjectName(QString::fromUtf8("actionSetup"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8("config.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSetup->setIcon(icon4);
        actionOpen_log_dir = new QAction(MainWindow);
        actionOpen_log_dir->setObjectName(QString::fromUtf8("actionOpen_log_dir"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8("open_dir.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen_log_dir->setIcon(icon5);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetFixedSize);
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tabWidget->setTabsClosable(false);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_4 = new QVBoxLayout(tab);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        frame_4 = new QFrame(tab);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setFrameShape(QFrame::Box);
        frame_4->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame_4);
        verticalLayout_3->setSpacing(3);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(3, 3, 3, 3);
        label_38 = new QLabel(frame_4);
        label_38->setObjectName(QString::fromUtf8("label_38"));
        QFont font;
        font.setPointSize(10);
        label_38->setFont(font);

        verticalLayout_3->addWidget(label_38);

        frame_3 = new QFrame(frame_4);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        horizontalLayout_4 = new QHBoxLayout(frame_3);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(3, 3, 3, 3);
        comboBox = new QComboBox(frame_3);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(comboBox);

        openButton = new QPushButton(frame_3);
        openButton->setObjectName(QString::fromUtf8("openButton"));

        horizontalLayout_4->addWidget(openButton);

        closeButton = new QPushButton(frame_3);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setEnabled(false);

        horizontalLayout_4->addWidget(closeButton);

        reloadButton = new QPushButton(frame_3);
        reloadButton->setObjectName(QString::fromUtf8("reloadButton"));
        reloadButton->setEnabled(false);

        horizontalLayout_4->addWidget(reloadButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);


        verticalLayout_3->addWidget(frame_3);


        verticalLayout_4->addWidget(frame_4);

        frame_2 = new QFrame(tab);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout_5 = new QHBoxLayout(frame_2);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gear2Edit = new QLineEdit(frame_2);
        gear2Edit->setObjectName(QString::fromUtf8("gear2Edit"));
        QFont font1;
        font1.setPointSize(12);
        gear2Edit->setFont(font1);
        gear2Edit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(gear2Edit, 8, 1, 1, 1);

        gear4Edit = new QLineEdit(frame_2);
        gear4Edit->setObjectName(QString::fromUtf8("gear4Edit"));
        gear4Edit->setFont(font1);
        gear4Edit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(gear4Edit, 10, 1, 1, 1);

        line = new QFrame(frame_2);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line, 15, 0, 1, 6);

        tyreCircumEdit = new QLineEdit(frame_2);
        tyreCircumEdit->setObjectName(QString::fromUtf8("tyreCircumEdit"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(tyreCircumEdit->sizePolicy().hasHeightForWidth());
        tyreCircumEdit->setSizePolicy(sizePolicy2);
        tyreCircumEdit->setFont(font1);
        tyreCircumEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(tyreCircumEdit, 4, 1, 1, 1);

        label_16 = new QLabel(frame_2);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_16, 8, 4, 1, 1);

        firmwareVersion = new QLineEdit(frame_2);
        firmwareVersion->setObjectName(QString::fromUtf8("firmwareVersion"));
        firmwareVersion->setFont(font1);
        firmwareVersion->setInputMethodHints(Qt::ImhFormattedNumbersOnly);
        firmwareVersion->setAlignment(Qt::AlignCenter);
        firmwareVersion->setReadOnly(true);

        gridLayout_2->addWidget(firmwareVersion, 4, 5, 1, 1);

        buttonCloseMut = new QPushButton(frame_2);
        buttonCloseMut->setObjectName(QString::fromUtf8("buttonCloseMut"));

        gridLayout_2->addWidget(buttonCloseMut, 16, 5, 2, 1);

        label_3 = new QLabel(frame_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 6, 0, 1, 1);

        label_8 = new QLabel(frame_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_8, 11, 0, 1, 1);

        gear3Edit = new QLineEdit(frame_2);
        gear3Edit->setObjectName(QString::fromUtf8("gear3Edit"));
        gear3Edit->setFont(font1);
        gear3Edit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(gear3Edit, 9, 1, 1, 1);

        label_15 = new QLabel(frame_2);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_15, 7, 4, 1, 1);

        pidIEdit = new QLineEdit(frame_2);
        pidIEdit->setObjectName(QString::fromUtf8("pidIEdit"));
        pidIEdit->setFont(font1);
        pidIEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(pidIEdit, 8, 5, 1, 1);

        label_41 = new QLabel(frame_2);
        label_41->setObjectName(QString::fromUtf8("label_41"));

        gridLayout_2->addWidget(label_41, 4, 4, 1, 1);

        portListMut = new QComboBox(frame_2);
        portListMut->setObjectName(QString::fromUtf8("portListMut"));
        portListMut->setEnabled(false);
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(2);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(portListMut->sizePolicy().hasHeightForWidth());
        portListMut->setSizePolicy(sizePolicy3);

        gridLayout_2->addWidget(portListMut, 16, 1, 1, 1);

        label_6 = new QLabel(frame_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_6, 9, 0, 1, 1);

        label_14 = new QLabel(frame_2);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_2->addWidget(label_14, 6, 4, 1, 1);

        label_7 = new QLabel(frame_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_7, 10, 0, 1, 1);

        label_13 = new QLabel(frame_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_2->addWidget(label_13, 14, 2, 1, 1);

        gear1Edit = new QLineEdit(frame_2);
        gear1Edit->setObjectName(QString::fromUtf8("gear1Edit"));
        gear1Edit->setFont(font1);
        gear1Edit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(gear1Edit, 7, 1, 1, 1);

        label_2 = new QLabel(frame_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 4, 2, 1, 1);

        label = new QLabel(frame_2);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 4, 0, 1, 1);

        line_2 = new QFrame(frame_2);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(2);
        sizePolicy4.setHeightForWidth(line_2->sizePolicy().hasHeightForWidth());
        line_2->setSizePolicy(sizePolicy4);
        line_2->setMinimumSize(QSize(0, 0));
        line_2->setLineWidth(1);
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line_2, 4, 3, 11, 1);

        label_36 = new QLabel(frame_2);
        label_36->setObjectName(QString::fromUtf8("label_36"));

        gridLayout_2->addWidget(label_36, 16, 0, 1, 1);

        label_12 = new QLabel(frame_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_2->addWidget(label_12, 14, 0, 1, 1);

        label_9 = new QLabel(frame_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_9, 12, 0, 1, 1);

        buttonOpenMut = new QPushButton(frame_2);
        buttonOpenMut->setObjectName(QString::fromUtf8("buttonOpenMut"));
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(2);
        sizePolicy5.setHeightForWidth(buttonOpenMut->sizePolicy().hasHeightForWidth());
        buttonOpenMut->setSizePolicy(sizePolicy5);

        gridLayout_2->addWidget(buttonOpenMut, 16, 4, 2, 1);

        label_10 = new QLabel(frame_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_2->addWidget(label_10, 13, 0, 1, 1);

        gear5Edit = new QLineEdit(frame_2);
        gear5Edit->setObjectName(QString::fromUtf8("gear5Edit"));
        gear5Edit->setFont(font1);
        gear5Edit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(gear5Edit, 11, 1, 1, 1);

        maxThrottleEdit = new QLineEdit(frame_2);
        maxThrottleEdit->setObjectName(QString::fromUtf8("maxThrottleEdit"));
        maxThrottleEdit->setFont(font1);
        maxThrottleEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(maxThrottleEdit, 11, 5, 1, 1);

        speedRatioEdit = new QLineEdit(frame_2);
        speedRatioEdit->setObjectName(QString::fromUtf8("speedRatioEdit"));
        speedRatioEdit->setFont(font1);
        speedRatioEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(speedRatioEdit, 13, 1, 1, 1);

        label_17 = new QLabel(frame_2);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_17, 9, 4, 1, 1);

        label_5 = new QLabel(frame_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_5, 8, 0, 1, 1);

        pidPEdit = new QLineEdit(frame_2);
        pidPEdit->setObjectName(QString::fromUtf8("pidPEdit"));
        pidPEdit->setFont(font1);
        pidPEdit->setInputMethodHints(Qt::ImhFormattedNumbersOnly);
        pidPEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(pidPEdit, 7, 5, 1, 1);

        gear6Edit = new QLineEdit(frame_2);
        gear6Edit->setObjectName(QString::fromUtf8("gear6Edit"));
        gear6Edit->setFont(font1);
        gear6Edit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(gear6Edit, 12, 1, 1, 1);

        label_4 = new QLabel(frame_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_4, 7, 0, 1, 1);

        label_11 = new QLabel(frame_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_2->addWidget(label_11, 13, 2, 1, 1);

        baseBoostEdit = new QLineEdit(frame_2);
        baseBoostEdit->setObjectName(QString::fromUtf8("baseBoostEdit"));
        baseBoostEdit->setFont(font1);
        baseBoostEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(baseBoostEdit, 14, 1, 1, 1);

        pidDEdit = new QLineEdit(frame_2);
        pidDEdit->setObjectName(QString::fromUtf8("pidDEdit"));
        pidDEdit->setFont(font1);
        pidDEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(pidDEdit, 9, 5, 1, 1);

        label_44 = new QLabel(frame_2);
        label_44->setObjectName(QString::fromUtf8("label_44"));

        gridLayout_2->addWidget(label_44, 11, 4, 1, 1);

        label_45 = new QLabel(frame_2);
        label_45->setObjectName(QString::fromUtf8("label_45"));

        gridLayout_2->addWidget(label_45, 11, 6, 1, 1);

        forceWGEdit = new QLineEdit(frame_2);
        forceWGEdit->setObjectName(QString::fromUtf8("forceWGEdit"));
        forceWGEdit->setFont(font1);
        forceWGEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(forceWGEdit, 14, 5, 1, 1);

        label_40 = new QLabel(frame_2);
        label_40->setObjectName(QString::fromUtf8("label_40"));
        label_40->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_40, 14, 4, 1, 1);

        label_39 = new QLabel(frame_2);
        label_39->setObjectName(QString::fromUtf8("label_39"));

        gridLayout_2->addWidget(label_39, 13, 4, 1, 1);

        label_49 = new QLabel(frame_2);
        label_49->setObjectName(QString::fromUtf8("label_49"));

        gridLayout_2->addWidget(label_49, 12, 4, 1, 1);

        fuelPumpPrimeEdit = new QLineEdit(frame_2);
        fuelPumpPrimeEdit->setObjectName(QString::fromUtf8("fuelPumpPrimeEdit"));
        fuelPumpPrimeEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(fuelPumpPrimeEdit, 12, 5, 1, 1);

        label_50 = new QLabel(frame_2);
        label_50->setObjectName(QString::fromUtf8("label_50"));

        gridLayout_2->addWidget(label_50, 12, 6, 1, 1);

        label_46 = new QLabel(frame_2);
        label_46->setObjectName(QString::fromUtf8("label_46"));

        gridLayout_2->addWidget(label_46, 14, 6, 1, 1);


        horizontalLayout_5->addLayout(gridLayout_2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);


        verticalLayout_4->addWidget(frame_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);

        tabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        gridLayout_3 = new QGridLayout(tab_3);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetDefaultConstraint);
        listTable = new QListWidget(tab_3);
        listTable->setObjectName(QString::fromUtf8("listTable"));
        listTable->setMaximumSize(QSize(200, 16777215));
        QFont font2;
        font2.setPointSize(14);
        listTable->setFont(font2);

        horizontalLayout_3->addWidget(listTable);

        frameForTable = new QFrame(tab_3);
        frameForTable->setObjectName(QString::fromUtf8("frameForTable"));
        frameForTable->setFrameShape(QFrame::NoFrame);
        frameForTable->setFrameShadow(QFrame::Raised);
        gridLayout_4 = new QGridLayout(frameForTable);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setSizeConstraint(QLayout::SetMaximumSize);
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        tableColumnLabel = new QLabel(frameForTable);
        tableColumnLabel->setObjectName(QString::fromUtf8("tableColumnLabel"));
        QSizePolicy sizePolicy6(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(tableColumnLabel->sizePolicy().hasHeightForWidth());
        tableColumnLabel->setSizePolicy(sizePolicy6);
        QFont font3;
        font3.setPointSize(10);
        font3.setBold(true);
        font3.setWeight(75);
        tableColumnLabel->setFont(font3);
        tableColumnLabel->setFrameShape(QFrame::Box);
        tableColumnLabel->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(tableColumnLabel, 0, 1, 1, 1);

        tableRowLabel = new QLabel(frameForTable);
        tableRowLabel->setObjectName(QString::fromUtf8("tableRowLabel"));
        QSizePolicy sizePolicy7(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(tableRowLabel->sizePolicy().hasHeightForWidth());
        tableRowLabel->setSizePolicy(sizePolicy7);
        tableRowLabel->setMaximumSize(QSize(50, 16777215));
        tableRowLabel->setFont(font3);
        tableRowLabel->setFrameShape(QFrame::Box);

        gridLayout_4->addWidget(tableRowLabel, 1, 0, 1, 1);

        tableWidget = new QTableWidget(frameForTable);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setEnabled(true);
        QSizePolicy sizePolicy8(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy8);
        tableWidget->setMaximumSize(QSize(16777215, 16777215));
        tableWidget->setRowCount(0);
        tableWidget->setColumnCount(0);

        gridLayout_4->addWidget(tableWidget, 1, 1, 1, 1);


        horizontalLayout_3->addWidget(frameForTable);


        gridLayout_3->addLayout(horizontalLayout_3, 1, 0, 1, 1);

        tabWidget->addTab(tab_3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_2 = new QVBoxLayout(tab_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        frame = new QFrame(tab_2);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy9(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy9.setHorizontalStretch(0);
        sizePolicy9.setVerticalStretch(0);
        sizePolicy9.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy9);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_5 = new QGridLayout(frame);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_5->setSizeConstraint(QLayout::SetDefaultConstraint);
        label_37 = new QLabel(frame);
        label_37->setObjectName(QString::fromUtf8("label_37"));

        gridLayout_5->addWidget(label_37, 11, 0, 1, 1);

        label_24 = new QLabel(frame);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        QSizePolicy sizePolicy10(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy10.setHorizontalStretch(0);
        sizePolicy10.setVerticalStretch(0);
        sizePolicy10.setHeightForWidth(label_24->sizePolicy().hasHeightForWidth());
        label_24->setSizePolicy(sizePolicy10);

        gridLayout_5->addWidget(label_24, 3, 3, 1, 1);

        label_30 = new QLabel(frame);
        label_30->setObjectName(QString::fromUtf8("label_30"));

        gridLayout_5->addWidget(label_30, 6, 0, 1, 1);

        knockSum = new QLineEdit(frame);
        knockSum->setObjectName(QString::fromUtf8("knockSum"));
        QSizePolicy sizePolicy11(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy11.setHorizontalStretch(0);
        sizePolicy11.setVerticalStretch(0);
        sizePolicy11.setHeightForWidth(knockSum->sizePolicy().hasHeightForWidth());
        knockSum->setSizePolicy(sizePolicy11);
        knockSum->setMaximumSize(QSize(80, 16777215));
        knockSum->setBaseSize(QSize(0, 0));
        QFont font4;
        font4.setPointSize(20);
        font4.setBold(true);
        font4.setWeight(75);
        knockSum->setFont(font4);
        knockSum->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        knockSum->setReadOnly(true);

        gridLayout_5->addWidget(knockSum, 6, 2, 1, 1);

        targetOutputEdit = new QLineEdit(frame);
        targetOutputEdit->setObjectName(QString::fromUtf8("targetOutputEdit"));
        sizePolicy11.setHeightForWidth(targetOutputEdit->sizePolicy().hasHeightForWidth());
        targetOutputEdit->setSizePolicy(sizePolicy11);
        targetOutputEdit->setMaximumSize(QSize(80, 16777215));
        targetOutputEdit->setBaseSize(QSize(0, 0));
        targetOutputEdit->setFont(font1);
        targetOutputEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        targetOutputEdit->setReadOnly(true);

        gridLayout_5->addWidget(targetOutputEdit, 9, 2, 1, 1);

        label_29 = new QLabel(frame);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        sizePolicy10.setHeightForWidth(label_29->sizePolicy().hasHeightForWidth());
        label_29->setSizePolicy(sizePolicy10);

        gridLayout_5->addWidget(label_29, 10, 3, 1, 1);

        rpmEdit = new QLineEdit(frame);
        rpmEdit->setObjectName(QString::fromUtf8("rpmEdit"));
        sizePolicy11.setHeightForWidth(rpmEdit->sizePolicy().hasHeightForWidth());
        rpmEdit->setSizePolicy(sizePolicy11);
        rpmEdit->setMinimumSize(QSize(0, 0));
        rpmEdit->setMaximumSize(QSize(80, 16777215));
        rpmEdit->setSizeIncrement(QSize(0, 0));
        rpmEdit->setBaseSize(QSize(0, 0));
        rpmEdit->setFont(font1);
        rpmEdit->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        rpmEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        rpmEdit->setReadOnly(true);

        gridLayout_5->addWidget(rpmEdit, 0, 2, 1, 1);

        label_21 = new QLabel(frame);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        sizePolicy10.setHeightForWidth(label_21->sizePolicy().hasHeightForWidth());
        label_21->setSizePolicy(sizePolicy10);

        gridLayout_5->addWidget(label_21, 7, 0, 1, 1);

        label_32 = new QLabel(frame);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        gridLayout_5->addWidget(label_32, 8, 0, 1, 1);

        label_20 = new QLabel(frame);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        sizePolicy10.setHeightForWidth(label_20->sizePolicy().hasHeightForWidth());
        label_20->setSizePolicy(sizePolicy10);

        gridLayout_5->addWidget(label_20, 2, 3, 1, 1);

        label_28 = new QLabel(frame);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        sizePolicy10.setHeightForWidth(label_28->sizePolicy().hasHeightForWidth());
        label_28->setSizePolicy(sizePolicy10);

        gridLayout_5->addWidget(label_28, 10, 0, 1, 1);

        label_31 = new QLabel(frame);
        label_31->setObjectName(QString::fromUtf8("label_31"));

        gridLayout_5->addWidget(label_31, 11, 3, 1, 1);

        gearEdit = new QLineEdit(frame);
        gearEdit->setObjectName(QString::fromUtf8("gearEdit"));
        sizePolicy11.setHeightForWidth(gearEdit->sizePolicy().hasHeightForWidth());
        gearEdit->setSizePolicy(sizePolicy11);
        gearEdit->setMinimumSize(QSize(0, 0));
        gearEdit->setMaximumSize(QSize(80, 16777215));
        gearEdit->setSizeIncrement(QSize(0, 0));
        gearEdit->setBaseSize(QSize(0, 0));
        gearEdit->setFont(font1);
        gearEdit->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        gearEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        gearEdit->setReadOnly(true);

        gridLayout_5->addWidget(gearEdit, 4, 2, 1, 1);

        label_47 = new QLabel(frame);
        label_47->setObjectName(QString::fromUtf8("label_47"));

        gridLayout_5->addWidget(label_47, 14, 0, 1, 1);

        label_23 = new QLabel(frame);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        sizePolicy10.setHeightForWidth(label_23->sizePolicy().hasHeightForWidth());
        label_23->setSizePolicy(sizePolicy10);

        gridLayout_5->addWidget(label_23, 3, 0, 1, 1);

        targetBoostEdit = new QLineEdit(frame);
        targetBoostEdit->setObjectName(QString::fromUtf8("targetBoostEdit"));
        sizePolicy11.setHeightForWidth(targetBoostEdit->sizePolicy().hasHeightForWidth());
        targetBoostEdit->setSizePolicy(sizePolicy11);
        targetBoostEdit->setMaximumSize(QSize(80, 16777215));
        targetBoostEdit->setBaseSize(QSize(0, 0));
        targetBoostEdit->setFont(font4);
        targetBoostEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        targetBoostEdit->setReadOnly(true);

        gridLayout_5->addWidget(targetBoostEdit, 8, 2, 1, 1);

        label_33 = new QLabel(frame);
        label_33->setObjectName(QString::fromUtf8("label_33"));

        gridLayout_5->addWidget(label_33, 8, 3, 1, 1);

        label_34 = new QLabel(frame);
        label_34->setObjectName(QString::fromUtf8("label_34"));

        gridLayout_5->addWidget(label_34, 9, 0, 1, 1);

        label_27 = new QLabel(frame);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        sizePolicy10.setHeightForWidth(label_27->sizePolicy().hasHeightForWidth());
        label_27->setSizePolicy(sizePolicy10);

        gridLayout_5->addWidget(label_27, 4, 0, 1, 1);

        solDcEdit = new QLineEdit(frame);
        solDcEdit->setObjectName(QString::fromUtf8("solDcEdit"));
        sizePolicy11.setHeightForWidth(solDcEdit->sizePolicy().hasHeightForWidth());
        solDcEdit->setSizePolicy(sizePolicy11);
        solDcEdit->setMinimumSize(QSize(0, 0));
        solDcEdit->setMaximumSize(QSize(80, 16777215));
        solDcEdit->setSizeIncrement(QSize(0, 0));
        solDcEdit->setBaseSize(QSize(0, 0));
        solDcEdit->setFont(font1);
        solDcEdit->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        solDcEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        solDcEdit->setReadOnly(true);

        gridLayout_5->addWidget(solDcEdit, 5, 2, 1, 1);

        label_43 = new QLabel(frame);
        label_43->setObjectName(QString::fromUtf8("label_43"));

        gridLayout_5->addWidget(label_43, 13, 3, 1, 1);

        label_26 = new QLabel(frame);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        sizePolicy10.setHeightForWidth(label_26->sizePolicy().hasHeightForWidth());
        label_26->setSizePolicy(sizePolicy10);

        gridLayout_5->addWidget(label_26, 5, 3, 1, 1);

        label_19 = new QLabel(frame);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        sizePolicy10.setHeightForWidth(label_19->sizePolicy().hasHeightForWidth());
        label_19->setSizePolicy(sizePolicy10);

        gridLayout_5->addWidget(label_19, 2, 0, 1, 1);

        cpuLoadEdit = new QLineEdit(frame);
        cpuLoadEdit->setObjectName(QString::fromUtf8("cpuLoadEdit"));
        sizePolicy11.setHeightForWidth(cpuLoadEdit->sizePolicy().hasHeightForWidth());
        cpuLoadEdit->setSizePolicy(sizePolicy11);
        cpuLoadEdit->setMaximumSize(QSize(80, 16777215));
        cpuLoadEdit->setBaseSize(QSize(0, 0));
        cpuLoadEdit->setFont(font1);
        cpuLoadEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        cpuLoadEdit->setReadOnly(true);

        gridLayout_5->addWidget(cpuLoadEdit, 11, 2, 1, 1);

        throttleEdit = new QLineEdit(frame);
        throttleEdit->setObjectName(QString::fromUtf8("throttleEdit"));
        sizePolicy11.setHeightForWidth(throttleEdit->sizePolicy().hasHeightForWidth());
        throttleEdit->setSizePolicy(sizePolicy11);
        throttleEdit->setMinimumSize(QSize(0, 0));
        throttleEdit->setMaximumSize(QSize(80, 16777215));
        throttleEdit->setSizeIncrement(QSize(0, 0));
        throttleEdit->setBaseSize(QSize(0, 0));
        throttleEdit->setFont(font1);
        throttleEdit->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        throttleEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        throttleEdit->setReadOnly(true);

        gridLayout_5->addWidget(throttleEdit, 3, 2, 1, 1);

        airFlowEdit = new QLineEdit(frame);
        airFlowEdit->setObjectName(QString::fromUtf8("airFlowEdit"));
        sizePolicy11.setHeightForWidth(airFlowEdit->sizePolicy().hasHeightForWidth());
        airFlowEdit->setSizePolicy(sizePolicy11);
        airFlowEdit->setMaximumSize(QSize(80, 16777215));
        airFlowEdit->setBaseSize(QSize(0, 0));
        airFlowEdit->setFont(font1);
        airFlowEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        airFlowEdit->setReadOnly(true);

        gridLayout_5->addWidget(airFlowEdit, 13, 2, 1, 1);

        label_42 = new QLabel(frame);
        label_42->setObjectName(QString::fromUtf8("label_42"));

        gridLayout_5->addWidget(label_42, 13, 0, 1, 1);

        label_25 = new QLabel(frame);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        sizePolicy10.setHeightForWidth(label_25->sizePolicy().hasHeightForWidth());
        label_25->setSizePolicy(sizePolicy10);

        gridLayout_5->addWidget(label_25, 5, 0, 1, 1);

        loadEdit = new QLineEdit(frame);
        loadEdit->setObjectName(QString::fromUtf8("loadEdit"));
        sizePolicy11.setHeightForWidth(loadEdit->sizePolicy().hasHeightForWidth());
        loadEdit->setSizePolicy(sizePolicy11);
        loadEdit->setMinimumSize(QSize(0, 0));
        loadEdit->setMaximumSize(QSize(80, 16777215));
        loadEdit->setSizeIncrement(QSize(0, 0));
        loadEdit->setBaseSize(QSize(0, 0));
        loadEdit->setFont(font1);
        loadEdit->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        loadEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        loadEdit->setReadOnly(true);

        gridLayout_5->addWidget(loadEdit, 10, 2, 1, 1);

        label_18 = new QLabel(frame);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        sizePolicy10.setHeightForWidth(label_18->sizePolicy().hasHeightForWidth());
        label_18->setSizePolicy(sizePolicy10);

        gridLayout_5->addWidget(label_18, 0, 0, 1, 1);

        mapEdit = new QLineEdit(frame);
        mapEdit->setObjectName(QString::fromUtf8("mapEdit"));
        sizePolicy11.setHeightForWidth(mapEdit->sizePolicy().hasHeightForWidth());
        mapEdit->setSizePolicy(sizePolicy11);
        mapEdit->setMinimumSize(QSize(0, 0));
        mapEdit->setMaximumSize(QSize(80, 16777215));
        mapEdit->setSizeIncrement(QSize(0, 0));
        mapEdit->setBaseSize(QSize(0, 0));
        mapEdit->setFont(font4);
        mapEdit->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        mapEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        mapEdit->setReadOnly(true);

        gridLayout_5->addWidget(mapEdit, 7, 2, 1, 1);

        label_22 = new QLabel(frame);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        sizePolicy10.setHeightForWidth(label_22->sizePolicy().hasHeightForWidth());
        label_22->setSizePolicy(sizePolicy10);

        gridLayout_5->addWidget(label_22, 7, 3, 1, 1);

        speedEdit = new QLineEdit(frame);
        speedEdit->setObjectName(QString::fromUtf8("speedEdit"));
        sizePolicy11.setHeightForWidth(speedEdit->sizePolicy().hasHeightForWidth());
        speedEdit->setSizePolicy(sizePolicy11);
        speedEdit->setMinimumSize(QSize(0, 0));
        speedEdit->setMaximumSize(QSize(80, 16777215));
        speedEdit->setSizeIncrement(QSize(0, 0));
        speedEdit->setBaseSize(QSize(0, 0));
        speedEdit->setFont(font1);
        speedEdit->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        speedEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        speedEdit->setReadOnly(true);

        gridLayout_5->addWidget(speedEdit, 2, 2, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_2, 16, 2, 1, 1);

        label_35 = new QLabel(frame);
        label_35->setObjectName(QString::fromUtf8("label_35"));

        gridLayout_5->addWidget(label_35, 9, 3, 1, 1);

        fuelPressEdit = new QLineEdit(frame);
        fuelPressEdit->setObjectName(QString::fromUtf8("fuelPressEdit"));
        sizePolicy11.setHeightForWidth(fuelPressEdit->sizePolicy().hasHeightForWidth());
        fuelPressEdit->setSizePolicy(sizePolicy11);
        fuelPressEdit->setMaximumSize(QSize(80, 16777215));
        fuelPressEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_5->addWidget(fuelPressEdit, 14, 2, 1, 1);

        label_48 = new QLabel(frame);
        label_48->setObjectName(QString::fromUtf8("label_48"));

        gridLayout_5->addWidget(label_48, 14, 3, 1, 1);


        horizontalLayout->addWidget(frame);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        plot1 = new QCustomPlot(tab_2);
        plot1->setObjectName(QString::fromUtf8("plot1"));
        sizePolicy6.setHeightForWidth(plot1->sizePolicy().hasHeightForWidth());
        plot1->setSizePolicy(sizePolicy6);

        verticalLayout->addWidget(plot1);

        plot2 = new QCustomPlot(tab_2);
        plot2->setObjectName(QString::fromUtf8("plot2"));

        verticalLayout->addWidget(plot2);


        horizontalLayout->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout);

        tabWidget->addTab(tab_2, QString());
        tabPlotter = new QWidget();
        tabPlotter->setObjectName(QString::fromUtf8("tabPlotter"));
        tabWidget->addTab(tabPlotter, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);


        horizontalLayout_2->addLayout(gridLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 965, 21));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setMovable(false);
        toolBar->setIconSize(QSize(32, 32));
        toolBar->setFloatable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        QWidget::setTabOrder(tyreCircumEdit, gear1Edit);
        QWidget::setTabOrder(gear1Edit, gear2Edit);
        QWidget::setTabOrder(gear2Edit, gear3Edit);
        QWidget::setTabOrder(gear3Edit, gear4Edit);
        QWidget::setTabOrder(gear4Edit, gear5Edit);
        QWidget::setTabOrder(gear5Edit, gear6Edit);
        QWidget::setTabOrder(gear6Edit, speedRatioEdit);
        QWidget::setTabOrder(speedRatioEdit, baseBoostEdit);
        QWidget::setTabOrder(baseBoostEdit, pidPEdit);
        QWidget::setTabOrder(pidPEdit, pidIEdit);
        QWidget::setTabOrder(pidIEdit, pidDEdit);
        QWidget::setTabOrder(pidDEdit, maxThrottleEdit);
        QWidget::setTabOrder(maxThrottleEdit, fuelPumpPrimeEdit);
        QWidget::setTabOrder(fuelPumpPrimeEdit, forceWGEdit);
        QWidget::setTabOrder(forceWGEdit, rpmEdit);
        QWidget::setTabOrder(rpmEdit, loadEdit);
        QWidget::setTabOrder(loadEdit, cpuLoadEdit);
        QWidget::setTabOrder(cpuLoadEdit, firmwareVersion);
        QWidget::setTabOrder(firmwareVersion, comboBox);
        QWidget::setTabOrder(comboBox, openButton);
        QWidget::setTabOrder(openButton, closeButton);
        QWidget::setTabOrder(closeButton, reloadButton);
        QWidget::setTabOrder(reloadButton, portListMut);
        QWidget::setTabOrder(portListMut, buttonOpenMut);
        QWidget::setTabOrder(buttonOpenMut, buttonCloseMut);
        QWidget::setTabOrder(buttonCloseMut, speedEdit);
        QWidget::setTabOrder(speedEdit, solDcEdit);
        QWidget::setTabOrder(solDcEdit, tabWidget);
        QWidget::setTabOrder(tabWidget, listTable);
        QWidget::setTabOrder(listTable, tableWidget);
        QWidget::setTabOrder(tableWidget, knockSum);
        QWidget::setTabOrder(knockSum, targetOutputEdit);
        QWidget::setTabOrder(targetOutputEdit, gearEdit);
        QWidget::setTabOrder(gearEdit, targetBoostEdit);
        QWidget::setTabOrder(targetBoostEdit, throttleEdit);
        QWidget::setTabOrder(throttleEdit, airFlowEdit);
        QWidget::setTabOrder(airFlowEdit, mapEdit);
        QWidget::setTabOrder(mapEdit, fuelPressEdit);

        toolBar->addAction(actionStart_Log);
        toolBar->addAction(actionConnect);
        toolBar->addAction(actionConnectMut);
        toolBar->addAction(actionOpen_log_dir);
        toolBar->addSeparator();
        toolBar->addAction(actionSetup);

        retranslateUi(MainWindow);
        QObject::connect(reloadButton, SIGNAL(clicked()), MainWindow, SLOT(reloadConfig()));
        QObject::connect(openButton, SIGNAL(clicked()), MainWindow, SLOT(connectPort()));
        QObject::connect(closeButton, SIGNAL(clicked()), MainWindow, SLOT(closePort()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actionStart_Log->setText(QApplication::translate("MainWindow", "Start Log", nullptr));
#ifndef QT_NO_SHORTCUT
        actionStart_Log->setShortcut(QApplication::translate("MainWindow", "F1", nullptr));
#endif // QT_NO_SHORTCUT
        actionConnect->setText(QApplication::translate("MainWindow", "Connect", nullptr));
#ifndef QT_NO_SHORTCUT
        actionConnect->setShortcut(QApplication::translate("MainWindow", "F2", nullptr));
#endif // QT_NO_SHORTCUT
        actionConnectMut->setText(QApplication::translate("MainWindow", "ConnectMut", nullptr));
#ifndef QT_NO_TOOLTIP
        actionConnectMut->setToolTip(QApplication::translate("MainWindow", "Connect MUT logger", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionConnectMut->setShortcut(QApplication::translate("MainWindow", "F3", nullptr));
#endif // QT_NO_SHORTCUT
        actionSetup->setText(QApplication::translate("MainWindow", "Setup", nullptr));
#ifndef QT_NO_TOOLTIP
        actionSetup->setToolTip(QApplication::translate("MainWindow", "Setup", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionSetup->setShortcut(QApplication::translate("MainWindow", "F5", nullptr));
#endif // QT_NO_SHORTCUT
        actionOpen_log_dir->setText(QApplication::translate("MainWindow", "Open log dir", nullptr));
#ifndef QT_NO_TOOLTIP
        actionOpen_log_dir->setToolTip(QApplication::translate("MainWindow", "Open log directory", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionOpen_log_dir->setShortcut(QApplication::translate("MainWindow", "F4", nullptr));
#endif // QT_NO_SHORTCUT
        label_38->setText(QApplication::translate("MainWindow", "Comm port", nullptr));
        openButton->setText(QApplication::translate("MainWindow", "OPEN", nullptr));
        closeButton->setText(QApplication::translate("MainWindow", "CLOSE", nullptr));
        reloadButton->setText(QApplication::translate("MainWindow", "Reload", nullptr));
        gear2Edit->setInputMask(QString());
        gear4Edit->setInputMask(QString());
        tyreCircumEdit->setInputMask(QString());
        label_16->setText(QApplication::translate("MainWindow", "I", nullptr));
        firmwareVersion->setInputMask(QString());
        buttonCloseMut->setText(QApplication::translate("MainWindow", "CLOSE", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "Gear Ratio", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "5th gear", nullptr));
        gear3Edit->setInputMask(QString());
        label_15->setText(QApplication::translate("MainWindow", "P", nullptr));
        pidIEdit->setInputMask(QString());
        label_41->setText(QApplication::translate("MainWindow", "Firmware Version", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "3rd gear", nullptr));
        label_14->setText(QApplication::translate("MainWindow", "PID", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "4th gear", nullptr));
        label_13->setText(QApplication::translate("MainWindow", "BAR", nullptr));
        gear1Edit->setInputMask(QString());
        label_2->setText(QApplication::translate("MainWindow", "m", nullptr));
        label->setText(QApplication::translate("MainWindow", "Tyre Circumference", nullptr));
        label_36->setText(QApplication::translate("MainWindow", "MUT Port", nullptr));
        label_12->setText(QApplication::translate("MainWindow", "Base target boost", nullptr));
        label_9->setText(QApplication::translate("MainWindow", "6th gear", nullptr));
        buttonOpenMut->setText(QApplication::translate("MainWindow", "OPEN ", nullptr));
        label_10->setText(QApplication::translate("MainWindow", "Speed sensor", nullptr));
        gear5Edit->setInputMask(QString());
        maxThrottleEdit->setInputMask(QString());
        speedRatioEdit->setInputMask(QString());
        label_17->setText(QApplication::translate("MainWindow", "D", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "2nd gear", nullptr));
        pidPEdit->setInputMask(QString());
        gear6Edit->setInputMask(QString());
        label_4->setText(QApplication::translate("MainWindow", "1st gear", nullptr));
        label_11->setText(QApplication::translate("MainWindow", "Hz/km/h", nullptr));
        baseBoostEdit->setInputMask(QString());
        pidDEdit->setInputMask(QString());
        label_44->setText(QApplication::translate("MainWindow", "Max throttle", nullptr));
        label_45->setText(QApplication::translate("MainWindow", "%", nullptr));
        forceWGEdit->setInputMask(QString());
        forceWGEdit->setText(QApplication::translate("MainWindow", "0", nullptr));
        label_40->setText(QApplication::translate("MainWindow", "Force Waste Gate", nullptr));
        label_39->setText(QApplication::translate("MainWindow", "Simulation", nullptr));
        label_49->setText(QApplication::translate("MainWindow", "Prime pump", nullptr));
        label_50->setText(QApplication::translate("MainWindow", "s", nullptr));
        label_46->setText(QApplication::translate("MainWindow", "%", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Configuration", nullptr));
        tableColumnLabel->setText(QApplication::translate("MainWindow", "GEAR", nullptr));
        tableRowLabel->setText(QApplication::translate("MainWindow", "RPM", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "Tables", nullptr));
        label_37->setText(QApplication::translate("MainWindow", "CPU Load", nullptr));
        label_24->setText(QApplication::translate("MainWindow", "%", nullptr));
        label_30->setText(QApplication::translate("MainWindow", "Knock", nullptr));
        label_29->setText(QApplication::translate("MainWindow", "%", nullptr));
        label_21->setText(QApplication::translate("MainWindow", "BOOST", nullptr));
        label_32->setText(QApplication::translate("MainWindow", "Target Boost", nullptr));
        label_20->setText(QApplication::translate("MainWindow", "km/h", nullptr));
        label_28->setText(QApplication::translate("MainWindow", "Load", nullptr));
        label_31->setText(QApplication::translate("MainWindow", "%", nullptr));
        label_47->setText(QApplication::translate("MainWindow", "Fuel Press", nullptr));
        label_23->setText(QApplication::translate("MainWindow", "Throttle", nullptr));
        label_33->setText(QApplication::translate("MainWindow", "Bar", nullptr));
        label_34->setText(QApplication::translate("MainWindow", "Taget Output", nullptr));
        label_27->setText(QApplication::translate("MainWindow", "Gear", nullptr));
        label_43->setText(QApplication::translate("MainWindow", "Hz", nullptr));
        label_26->setText(QApplication::translate("MainWindow", "%", nullptr));
        label_19->setText(QApplication::translate("MainWindow", "Speed", nullptr));
        label_42->setText(QApplication::translate("MainWindow", "Air flow", nullptr));
        label_25->setText(QApplication::translate("MainWindow", "Sol DC", nullptr));
        label_18->setText(QApplication::translate("MainWindow", "RPM", nullptr));
        label_22->setText(QApplication::translate("MainWindow", "Bar", nullptr));
        label_35->setText(QApplication::translate("MainWindow", "Bar", nullptr));
        label_48->setText(QApplication::translate("MainWindow", "Bar", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Monitor", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabPlotter), QApplication::translate("MainWindow", "Page", nullptr));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
