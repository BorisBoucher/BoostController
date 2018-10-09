/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
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
    QLabel *label_8;
    QLabel *label;
    QLineEdit *gear1Edit;
    QLabel *label_5;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *gear5Edit;
    QLineEdit *pidPEdit;
    QLineEdit *gear6Edit;
    QLineEdit *pidIEdit;
    QLabel *label_6;
    QLabel *label_17;
    QLabel *label_10;
    QLabel *label_15;
    QLabel *label_9;
    QLineEdit *tyreCircumEdit;
    QLineEdit *gear3Edit;
    QLabel *label_16;
    QLabel *label_7;
    QLineEdit *gear4Edit;
    QLineEdit *gear2Edit;
    QLabel *label_14;
    QLabel *label_4;
    QLineEdit *pidDEdit;
    QLabel *label_11;
    QLineEdit *baseBoostEdit;
    QComboBox *portListMut;
    QLineEdit *speedRatioEdit;
    QPushButton *buttonOpenLog;
    QPushButton *buttonCloseLog;
    QLabel *label_37;
    QLabel *label_12;
    QComboBox *portListLogger;
    QLabel *label_36;
    QFrame *line;
    QLabel *label_13;
    QFrame *line_2;
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
    QLineEdit *solDcEdit;
    QLabel *label_18;
    QLineEdit *throttleEdit;
    QLabel *label_25;
    QLabel *label_24;
    QLabel *label_23;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_27;
    QLabel *label_20;
    QLineEdit *gearEdit;
    QLineEdit *speedEdit;
    QLabel *label_26;
    QLabel *label_22;
    QLineEdit *rpmEdit;
    QLabel *label_19;
    QLineEdit *mapEdit;
    QLabel *label_21;
    QLabel *label_33;
    QLineEdit *targetBoostEdit;
    QLabel *label_32;
    QLabel *label_31;
    QLineEdit *loadEdit;
    QLabel *label_28;
    QLabel *label_29;
    QLabel *label_30;
    QLineEdit *cpuLoadEdit;
    QLineEdit *targetOutputEdit;
    QLabel *label_34;
    QLabel *label_35;
    QVBoxLayout *verticalLayout;
    QCustomPlot *plot1;
    QCustomPlot *plot2;
    QWidget *tabPlotter;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(965, 692);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetFixedSize);
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tabWidget->setTabsClosable(false);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout_4 = new QVBoxLayout(tab);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        frame_4 = new QFrame(tab);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        frame_4->setFrameShape(QFrame::Box);
        frame_4->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame_4);
        verticalLayout_3->setSpacing(3);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(3, 3, 3, 3);
        label_38 = new QLabel(frame_4);
        label_38->setObjectName(QStringLiteral("label_38"));
        QFont font;
        font.setPointSize(10);
        label_38->setFont(font);

        verticalLayout_3->addWidget(label_38);

        frame_3 = new QFrame(frame_4);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        horizontalLayout_4 = new QHBoxLayout(frame_3);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(3, 3, 3, 3);
        comboBox = new QComboBox(frame_3);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(comboBox);

        openButton = new QPushButton(frame_3);
        openButton->setObjectName(QStringLiteral("openButton"));

        horizontalLayout_4->addWidget(openButton);

        closeButton = new QPushButton(frame_3);
        closeButton->setObjectName(QStringLiteral("closeButton"));
        closeButton->setEnabled(false);

        horizontalLayout_4->addWidget(closeButton);

        reloadButton = new QPushButton(frame_3);
        reloadButton->setObjectName(QStringLiteral("reloadButton"));
        reloadButton->setEnabled(false);

        horizontalLayout_4->addWidget(reloadButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);


        verticalLayout_3->addWidget(frame_3);


        verticalLayout_4->addWidget(frame_4);

        frame_2 = new QFrame(tab);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout_5 = new QHBoxLayout(frame_2);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_8 = new QLabel(frame_2);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_8, 11, 0, 1, 1);

        label = new QLabel(frame_2);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_2->addWidget(label, 4, 0, 1, 1);

        gear1Edit = new QLineEdit(frame_2);
        gear1Edit->setObjectName(QStringLiteral("gear1Edit"));
        QFont font1;
        font1.setPointSize(12);
        gear1Edit->setFont(font1);

        gridLayout_2->addWidget(gear1Edit, 7, 1, 1, 1);

        label_5 = new QLabel(frame_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_5, 8, 0, 1, 1);

        label_2 = new QLabel(frame_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_2->addWidget(label_2, 4, 2, 1, 1);

        label_3 = new QLabel(frame_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_2->addWidget(label_3, 6, 0, 1, 1);

        gear5Edit = new QLineEdit(frame_2);
        gear5Edit->setObjectName(QStringLiteral("gear5Edit"));
        gear5Edit->setFont(font1);

        gridLayout_2->addWidget(gear5Edit, 11, 1, 1, 1);

        pidPEdit = new QLineEdit(frame_2);
        pidPEdit->setObjectName(QStringLiteral("pidPEdit"));
        pidPEdit->setFont(font1);
        pidPEdit->setInputMethodHints(Qt::ImhFormattedNumbersOnly);

        gridLayout_2->addWidget(pidPEdit, 7, 5, 1, 1);

        gear6Edit = new QLineEdit(frame_2);
        gear6Edit->setObjectName(QStringLiteral("gear6Edit"));
        gear6Edit->setFont(font1);

        gridLayout_2->addWidget(gear6Edit, 12, 1, 1, 1);

        pidIEdit = new QLineEdit(frame_2);
        pidIEdit->setObjectName(QStringLiteral("pidIEdit"));
        pidIEdit->setFont(font1);

        gridLayout_2->addWidget(pidIEdit, 8, 5, 1, 1);

        label_6 = new QLabel(frame_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_6, 9, 0, 1, 1);

        label_17 = new QLabel(frame_2);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_17, 9, 4, 1, 1);

        label_10 = new QLabel(frame_2);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout_2->addWidget(label_10, 13, 0, 1, 1);

        label_15 = new QLabel(frame_2);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_15, 7, 4, 1, 1);

        label_9 = new QLabel(frame_2);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_9, 12, 0, 1, 1);

        tyreCircumEdit = new QLineEdit(frame_2);
        tyreCircumEdit->setObjectName(QStringLiteral("tyreCircumEdit"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(tyreCircumEdit->sizePolicy().hasHeightForWidth());
        tyreCircumEdit->setSizePolicy(sizePolicy2);
        tyreCircumEdit->setFont(font1);

        gridLayout_2->addWidget(tyreCircumEdit, 4, 1, 1, 1);

        gear3Edit = new QLineEdit(frame_2);
        gear3Edit->setObjectName(QStringLiteral("gear3Edit"));
        gear3Edit->setFont(font1);

        gridLayout_2->addWidget(gear3Edit, 9, 1, 1, 1);

        label_16 = new QLabel(frame_2);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_16, 8, 4, 1, 1);

        label_7 = new QLabel(frame_2);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_7, 10, 0, 1, 1);

        gear4Edit = new QLineEdit(frame_2);
        gear4Edit->setObjectName(QStringLiteral("gear4Edit"));
        gear4Edit->setFont(font1);

        gridLayout_2->addWidget(gear4Edit, 10, 1, 1, 1);

        gear2Edit = new QLineEdit(frame_2);
        gear2Edit->setObjectName(QStringLiteral("gear2Edit"));
        gear2Edit->setFont(font1);

        gridLayout_2->addWidget(gear2Edit, 8, 1, 1, 1);

        label_14 = new QLabel(frame_2);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout_2->addWidget(label_14, 6, 4, 1, 1);

        label_4 = new QLabel(frame_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_4, 7, 0, 1, 1);

        pidDEdit = new QLineEdit(frame_2);
        pidDEdit->setObjectName(QStringLiteral("pidDEdit"));
        pidDEdit->setFont(font1);

        gridLayout_2->addWidget(pidDEdit, 9, 5, 1, 1);

        label_11 = new QLabel(frame_2);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_2->addWidget(label_11, 13, 2, 1, 1);

        baseBoostEdit = new QLineEdit(frame_2);
        baseBoostEdit->setObjectName(QStringLiteral("baseBoostEdit"));
        baseBoostEdit->setFont(font1);

        gridLayout_2->addWidget(baseBoostEdit, 14, 1, 1, 1);

        portListMut = new QComboBox(frame_2);
        portListMut->setObjectName(QStringLiteral("portListMut"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(2);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(portListMut->sizePolicy().hasHeightForWidth());
        portListMut->setSizePolicy(sizePolicy3);

        gridLayout_2->addWidget(portListMut, 16, 1, 1, 2);

        speedRatioEdit = new QLineEdit(frame_2);
        speedRatioEdit->setObjectName(QStringLiteral("speedRatioEdit"));
        speedRatioEdit->setFont(font1);

        gridLayout_2->addWidget(speedRatioEdit, 13, 1, 1, 1);

        buttonOpenLog = new QPushButton(frame_2);
        buttonOpenLog->setObjectName(QStringLiteral("buttonOpenLog"));
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(2);
        sizePolicy4.setHeightForWidth(buttonOpenLog->sizePolicy().hasHeightForWidth());
        buttonOpenLog->setSizePolicy(sizePolicy4);

        gridLayout_2->addWidget(buttonOpenLog, 16, 4, 2, 1);

        buttonCloseLog = new QPushButton(frame_2);
        buttonCloseLog->setObjectName(QStringLiteral("buttonCloseLog"));

        gridLayout_2->addWidget(buttonCloseLog, 16, 5, 2, 1);

        label_37 = new QLabel(frame_2);
        label_37->setObjectName(QStringLiteral("label_37"));

        gridLayout_2->addWidget(label_37, 17, 0, 1, 1);

        label_12 = new QLabel(frame_2);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout_2->addWidget(label_12, 14, 0, 1, 1);

        portListLogger = new QComboBox(frame_2);
        portListLogger->setObjectName(QStringLiteral("portListLogger"));

        gridLayout_2->addWidget(portListLogger, 17, 1, 1, 2);

        label_36 = new QLabel(frame_2);
        label_36->setObjectName(QStringLiteral("label_36"));

        gridLayout_2->addWidget(label_36, 16, 0, 1, 1);

        line = new QFrame(frame_2);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line, 15, 0, 1, 6);

        label_13 = new QLabel(frame_2);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_2->addWidget(label_13, 14, 2, 1, 1);

        line_2 = new QFrame(frame_2);
        line_2->setObjectName(QStringLiteral("line_2"));
        QSizePolicy sizePolicy5(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(2);
        sizePolicy5.setHeightForWidth(line_2->sizePolicy().hasHeightForWidth());
        line_2->setSizePolicy(sizePolicy5);
        line_2->setMinimumSize(QSize(0, 0));
        line_2->setLineWidth(1);
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line_2, 4, 3, 11, 1);


        horizontalLayout_5->addLayout(gridLayout_2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);


        verticalLayout_4->addWidget(frame_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);

        tabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        gridLayout_3 = new QGridLayout(tab_3);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetDefaultConstraint);
        listTable = new QListWidget(tab_3);
        listTable->setObjectName(QStringLiteral("listTable"));
        listTable->setMaximumSize(QSize(200, 16777215));
        QFont font2;
        font2.setPointSize(14);
        listTable->setFont(font2);

        horizontalLayout_3->addWidget(listTable);

        frameForTable = new QFrame(tab_3);
        frameForTable->setObjectName(QStringLiteral("frameForTable"));
        frameForTable->setFrameShape(QFrame::NoFrame);
        frameForTable->setFrameShadow(QFrame::Raised);
        gridLayout_4 = new QGridLayout(frameForTable);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setSizeConstraint(QLayout::SetMaximumSize);
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        tableColumnLabel = new QLabel(frameForTable);
        tableColumnLabel->setObjectName(QStringLiteral("tableColumnLabel"));
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
        tableRowLabel->setObjectName(QStringLiteral("tableRowLabel"));
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
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
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
        tab_2->setObjectName(QStringLiteral("tab_2"));
        verticalLayout_2 = new QVBoxLayout(tab_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        frame = new QFrame(tab_2);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_5 = new QGridLayout(frame);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        solDcEdit = new QLineEdit(frame);
        solDcEdit->setObjectName(QStringLiteral("solDcEdit"));
        sizePolicy1.setHeightForWidth(solDcEdit->sizePolicy().hasHeightForWidth());
        solDcEdit->setSizePolicy(sizePolicy1);
        solDcEdit->setMinimumSize(QSize(0, 0));
        solDcEdit->setSizeIncrement(QSize(0, 0));
        solDcEdit->setBaseSize(QSize(0, 0));
        solDcEdit->setFont(font1);
        solDcEdit->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        solDcEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        solDcEdit->setReadOnly(true);

        gridLayout_5->addWidget(solDcEdit, 5, 2, 1, 1);

        label_18 = new QLabel(frame);
        label_18->setObjectName(QStringLiteral("label_18"));
        QSizePolicy sizePolicy9(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy9.setHorizontalStretch(0);
        sizePolicy9.setVerticalStretch(0);
        sizePolicy9.setHeightForWidth(label_18->sizePolicy().hasHeightForWidth());
        label_18->setSizePolicy(sizePolicy9);

        gridLayout_5->addWidget(label_18, 0, 0, 1, 1);

        throttleEdit = new QLineEdit(frame);
        throttleEdit->setObjectName(QStringLiteral("throttleEdit"));
        sizePolicy1.setHeightForWidth(throttleEdit->sizePolicy().hasHeightForWidth());
        throttleEdit->setSizePolicy(sizePolicy1);
        throttleEdit->setMinimumSize(QSize(0, 0));
        throttleEdit->setSizeIncrement(QSize(0, 0));
        throttleEdit->setBaseSize(QSize(0, 0));
        throttleEdit->setFont(font1);
        throttleEdit->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        throttleEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        throttleEdit->setReadOnly(true);

        gridLayout_5->addWidget(throttleEdit, 4, 2, 1, 1);

        label_25 = new QLabel(frame);
        label_25->setObjectName(QStringLiteral("label_25"));
        sizePolicy9.setHeightForWidth(label_25->sizePolicy().hasHeightForWidth());
        label_25->setSizePolicy(sizePolicy9);

        gridLayout_5->addWidget(label_25, 5, 0, 1, 1);

        label_24 = new QLabel(frame);
        label_24->setObjectName(QStringLiteral("label_24"));
        sizePolicy9.setHeightForWidth(label_24->sizePolicy().hasHeightForWidth());
        label_24->setSizePolicy(sizePolicy9);

        gridLayout_5->addWidget(label_24, 4, 3, 1, 1);

        label_23 = new QLabel(frame);
        label_23->setObjectName(QStringLiteral("label_23"));
        sizePolicy9.setHeightForWidth(label_23->sizePolicy().hasHeightForWidth());
        label_23->setSizePolicy(sizePolicy9);

        gridLayout_5->addWidget(label_23, 4, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_2, 11, 2, 1, 1);

        label_27 = new QLabel(frame);
        label_27->setObjectName(QStringLiteral("label_27"));
        sizePolicy9.setHeightForWidth(label_27->sizePolicy().hasHeightForWidth());
        label_27->setSizePolicy(sizePolicy9);

        gridLayout_5->addWidget(label_27, 6, 0, 1, 1);

        label_20 = new QLabel(frame);
        label_20->setObjectName(QStringLiteral("label_20"));
        sizePolicy9.setHeightForWidth(label_20->sizePolicy().hasHeightForWidth());
        label_20->setSizePolicy(sizePolicy9);

        gridLayout_5->addWidget(label_20, 2, 3, 1, 1);

        gearEdit = new QLineEdit(frame);
        gearEdit->setObjectName(QStringLiteral("gearEdit"));
        sizePolicy1.setHeightForWidth(gearEdit->sizePolicy().hasHeightForWidth());
        gearEdit->setSizePolicy(sizePolicy1);
        gearEdit->setMinimumSize(QSize(0, 0));
        gearEdit->setSizeIncrement(QSize(0, 0));
        gearEdit->setBaseSize(QSize(0, 0));
        gearEdit->setFont(font1);
        gearEdit->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        gearEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        gearEdit->setReadOnly(true);

        gridLayout_5->addWidget(gearEdit, 6, 2, 1, 1);

        speedEdit = new QLineEdit(frame);
        speedEdit->setObjectName(QStringLiteral("speedEdit"));
        sizePolicy1.setHeightForWidth(speedEdit->sizePolicy().hasHeightForWidth());
        speedEdit->setSizePolicy(sizePolicy1);
        speedEdit->setMinimumSize(QSize(0, 0));
        speedEdit->setSizeIncrement(QSize(0, 0));
        speedEdit->setBaseSize(QSize(0, 0));
        speedEdit->setFont(font1);
        speedEdit->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        speedEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        speedEdit->setReadOnly(true);

        gridLayout_5->addWidget(speedEdit, 2, 2, 1, 1);

        label_26 = new QLabel(frame);
        label_26->setObjectName(QStringLiteral("label_26"));
        sizePolicy9.setHeightForWidth(label_26->sizePolicy().hasHeightForWidth());
        label_26->setSizePolicy(sizePolicy9);

        gridLayout_5->addWidget(label_26, 5, 3, 1, 1);

        label_22 = new QLabel(frame);
        label_22->setObjectName(QStringLiteral("label_22"));
        sizePolicy9.setHeightForWidth(label_22->sizePolicy().hasHeightForWidth());
        label_22->setSizePolicy(sizePolicy9);

        gridLayout_5->addWidget(label_22, 3, 3, 1, 1);

        rpmEdit = new QLineEdit(frame);
        rpmEdit->setObjectName(QStringLiteral("rpmEdit"));
        sizePolicy1.setHeightForWidth(rpmEdit->sizePolicy().hasHeightForWidth());
        rpmEdit->setSizePolicy(sizePolicy1);
        rpmEdit->setMinimumSize(QSize(0, 0));
        rpmEdit->setSizeIncrement(QSize(0, 0));
        rpmEdit->setBaseSize(QSize(0, 0));
        rpmEdit->setFont(font1);
        rpmEdit->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        rpmEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        rpmEdit->setReadOnly(true);

        gridLayout_5->addWidget(rpmEdit, 0, 2, 1, 1);

        label_19 = new QLabel(frame);
        label_19->setObjectName(QStringLiteral("label_19"));
        sizePolicy9.setHeightForWidth(label_19->sizePolicy().hasHeightForWidth());
        label_19->setSizePolicy(sizePolicy9);

        gridLayout_5->addWidget(label_19, 2, 0, 1, 1);

        mapEdit = new QLineEdit(frame);
        mapEdit->setObjectName(QStringLiteral("mapEdit"));
        sizePolicy1.setHeightForWidth(mapEdit->sizePolicy().hasHeightForWidth());
        mapEdit->setSizePolicy(sizePolicy1);
        mapEdit->setMinimumSize(QSize(0, 0));
        mapEdit->setSizeIncrement(QSize(0, 0));
        mapEdit->setBaseSize(QSize(0, 0));
        mapEdit->setFont(font1);
        mapEdit->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        mapEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        mapEdit->setReadOnly(true);

        gridLayout_5->addWidget(mapEdit, 3, 2, 1, 1);

        label_21 = new QLabel(frame);
        label_21->setObjectName(QStringLiteral("label_21"));
        sizePolicy9.setHeightForWidth(label_21->sizePolicy().hasHeightForWidth());
        label_21->setSizePolicy(sizePolicy9);

        gridLayout_5->addWidget(label_21, 3, 0, 1, 1);

        label_33 = new QLabel(frame);
        label_33->setObjectName(QStringLiteral("label_33"));

        gridLayout_5->addWidget(label_33, 9, 3, 1, 1);

        targetBoostEdit = new QLineEdit(frame);
        targetBoostEdit->setObjectName(QStringLiteral("targetBoostEdit"));
        sizePolicy1.setHeightForWidth(targetBoostEdit->sizePolicy().hasHeightForWidth());
        targetBoostEdit->setSizePolicy(sizePolicy1);
        targetBoostEdit->setFont(font1);
        targetBoostEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        targetBoostEdit->setReadOnly(true);

        gridLayout_5->addWidget(targetBoostEdit, 9, 2, 1, 1);

        label_32 = new QLabel(frame);
        label_32->setObjectName(QStringLiteral("label_32"));

        gridLayout_5->addWidget(label_32, 9, 0, 1, 1);

        label_31 = new QLabel(frame);
        label_31->setObjectName(QStringLiteral("label_31"));

        gridLayout_5->addWidget(label_31, 8, 3, 1, 1);

        loadEdit = new QLineEdit(frame);
        loadEdit->setObjectName(QStringLiteral("loadEdit"));
        sizePolicy1.setHeightForWidth(loadEdit->sizePolicy().hasHeightForWidth());
        loadEdit->setSizePolicy(sizePolicy1);
        loadEdit->setMinimumSize(QSize(0, 0));
        loadEdit->setSizeIncrement(QSize(0, 0));
        loadEdit->setBaseSize(QSize(0, 0));
        loadEdit->setFont(font1);
        loadEdit->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        loadEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        loadEdit->setReadOnly(true);

        gridLayout_5->addWidget(loadEdit, 7, 2, 1, 1);

        label_28 = new QLabel(frame);
        label_28->setObjectName(QStringLiteral("label_28"));
        sizePolicy9.setHeightForWidth(label_28->sizePolicy().hasHeightForWidth());
        label_28->setSizePolicy(sizePolicy9);

        gridLayout_5->addWidget(label_28, 7, 0, 1, 1);

        label_29 = new QLabel(frame);
        label_29->setObjectName(QStringLiteral("label_29"));
        sizePolicy9.setHeightForWidth(label_29->sizePolicy().hasHeightForWidth());
        label_29->setSizePolicy(sizePolicy9);

        gridLayout_5->addWidget(label_29, 7, 3, 1, 1);

        label_30 = new QLabel(frame);
        label_30->setObjectName(QStringLiteral("label_30"));

        gridLayout_5->addWidget(label_30, 8, 0, 1, 1);

        cpuLoadEdit = new QLineEdit(frame);
        cpuLoadEdit->setObjectName(QStringLiteral("cpuLoadEdit"));
        sizePolicy1.setHeightForWidth(cpuLoadEdit->sizePolicy().hasHeightForWidth());
        cpuLoadEdit->setSizePolicy(sizePolicy1);
        cpuLoadEdit->setFont(font1);
        cpuLoadEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        cpuLoadEdit->setReadOnly(true);

        gridLayout_5->addWidget(cpuLoadEdit, 8, 2, 1, 1);

        targetOutputEdit = new QLineEdit(frame);
        targetOutputEdit->setObjectName(QStringLiteral("targetOutputEdit"));
        sizePolicy1.setHeightForWidth(targetOutputEdit->sizePolicy().hasHeightForWidth());
        targetOutputEdit->setSizePolicy(sizePolicy1);
        targetOutputEdit->setFont(font1);
        targetOutputEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        targetOutputEdit->setReadOnly(true);

        gridLayout_5->addWidget(targetOutputEdit, 10, 2, 1, 1);

        label_34 = new QLabel(frame);
        label_34->setObjectName(QStringLiteral("label_34"));

        gridLayout_5->addWidget(label_34, 10, 0, 1, 1);

        label_35 = new QLabel(frame);
        label_35->setObjectName(QStringLiteral("label_35"));

        gridLayout_5->addWidget(label_35, 10, 3, 1, 1);


        horizontalLayout->addWidget(frame);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        plot1 = new QCustomPlot(tab_2);
        plot1->setObjectName(QStringLiteral("plot1"));
        sizePolicy6.setHeightForWidth(plot1->sizePolicy().hasHeightForWidth());
        plot1->setSizePolicy(sizePolicy6);

        verticalLayout->addWidget(plot1);

        plot2 = new QCustomPlot(tab_2);
        plot2->setObjectName(QStringLiteral("plot2"));

        verticalLayout->addWidget(plot2);


        horizontalLayout->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout);

        tabWidget->addTab(tab_2, QString());
        tabPlotter = new QWidget();
        tabPlotter->setObjectName(QStringLiteral("tabPlotter"));
        tabWidget->addTab(tabPlotter, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);


        horizontalLayout_2->addLayout(gridLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 965, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setMovable(true);
        mainToolBar->setFloatable(true);
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
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
        QWidget::setTabOrder(pidDEdit, tabWidget);
        QWidget::setTabOrder(tabWidget, solDcEdit);
        QWidget::setTabOrder(solDcEdit, throttleEdit);
        QWidget::setTabOrder(throttleEdit, gearEdit);
        QWidget::setTabOrder(gearEdit, speedEdit);
        QWidget::setTabOrder(speedEdit, rpmEdit);
        QWidget::setTabOrder(rpmEdit, mapEdit);
        QWidget::setTabOrder(mapEdit, targetBoostEdit);
        QWidget::setTabOrder(targetBoostEdit, loadEdit);
        QWidget::setTabOrder(loadEdit, cpuLoadEdit);
        QWidget::setTabOrder(cpuLoadEdit, targetOutputEdit);

        retranslateUi(MainWindow);
        QObject::connect(reloadButton, SIGNAL(clicked()), MainWindow, SLOT(reloadConfig()));
        QObject::connect(openButton, SIGNAL(clicked()), MainWindow, SLOT(connectPort()));
        QObject::connect(closeButton, SIGNAL(clicked()), MainWindow, SLOT(closePort()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        label_38->setText(QApplication::translate("MainWindow", "Comm port", Q_NULLPTR));
        openButton->setText(QApplication::translate("MainWindow", "OPEN", Q_NULLPTR));
        closeButton->setText(QApplication::translate("MainWindow", "CLOSE", Q_NULLPTR));
        reloadButton->setText(QApplication::translate("MainWindow", "Reload", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "5th gear", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Tyre Circumference", Q_NULLPTR));
        gear1Edit->setInputMask(QString());
        label_5->setText(QApplication::translate("MainWindow", "2nd gear", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "m", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "Gear Ratio", Q_NULLPTR));
        gear5Edit->setInputMask(QString());
        pidPEdit->setInputMask(QString());
        gear6Edit->setInputMask(QString());
        pidIEdit->setInputMask(QString());
        label_6->setText(QApplication::translate("MainWindow", "3rd gear", Q_NULLPTR));
        label_17->setText(QApplication::translate("MainWindow", "D", Q_NULLPTR));
        label_10->setText(QApplication::translate("MainWindow", "Speed sensor", Q_NULLPTR));
        label_15->setText(QApplication::translate("MainWindow", "P", Q_NULLPTR));
        label_9->setText(QApplication::translate("MainWindow", "6th gear", Q_NULLPTR));
        tyreCircumEdit->setInputMask(QString());
        gear3Edit->setInputMask(QString());
        label_16->setText(QApplication::translate("MainWindow", "I", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "4th gear", Q_NULLPTR));
        gear4Edit->setInputMask(QString());
        gear2Edit->setInputMask(QString());
        label_14->setText(QApplication::translate("MainWindow", "PID", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "1st gear", Q_NULLPTR));
        pidDEdit->setInputMask(QString());
        label_11->setText(QApplication::translate("MainWindow", "Hz/km/h", Q_NULLPTR));
        baseBoostEdit->setInputMask(QString());
        speedRatioEdit->setInputMask(QString());
        buttonOpenLog->setText(QApplication::translate("MainWindow", "OPEN ", Q_NULLPTR));
        buttonCloseLog->setText(QApplication::translate("MainWindow", "CLOSE", Q_NULLPTR));
        label_37->setText(QApplication::translate("MainWindow", "Logger Port", Q_NULLPTR));
        label_12->setText(QApplication::translate("MainWindow", "Base target boost", Q_NULLPTR));
        label_36->setText(QApplication::translate("MainWindow", "MUT Port", Q_NULLPTR));
        label_13->setText(QApplication::translate("MainWindow", "BAR", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Configuration", Q_NULLPTR));
        tableColumnLabel->setText(QApplication::translate("MainWindow", "GEAR", Q_NULLPTR));
        tableRowLabel->setText(QApplication::translate("MainWindow", "RPM", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "Tables", Q_NULLPTR));
        label_18->setText(QApplication::translate("MainWindow", "RPM", Q_NULLPTR));
        label_25->setText(QApplication::translate("MainWindow", "Sol DC", Q_NULLPTR));
        label_24->setText(QApplication::translate("MainWindow", "%", Q_NULLPTR));
        label_23->setText(QApplication::translate("MainWindow", "Throttle", Q_NULLPTR));
        label_27->setText(QApplication::translate("MainWindow", "Gear", Q_NULLPTR));
        label_20->setText(QApplication::translate("MainWindow", "km/h", Q_NULLPTR));
        label_26->setText(QApplication::translate("MainWindow", "%", Q_NULLPTR));
        label_22->setText(QApplication::translate("MainWindow", "Bar", Q_NULLPTR));
        label_19->setText(QApplication::translate("MainWindow", "Speed", Q_NULLPTR));
        label_21->setText(QApplication::translate("MainWindow", "MAP", Q_NULLPTR));
        label_33->setText(QApplication::translate("MainWindow", "Bar", Q_NULLPTR));
        label_32->setText(QApplication::translate("MainWindow", "Target Boost", Q_NULLPTR));
        label_31->setText(QApplication::translate("MainWindow", "%", Q_NULLPTR));
        label_28->setText(QApplication::translate("MainWindow", "Load", Q_NULLPTR));
        label_29->setText(QApplication::translate("MainWindow", "%", Q_NULLPTR));
        label_30->setText(QApplication::translate("MainWindow", "CPU Load", Q_NULLPTR));
        label_34->setText(QApplication::translate("MainWindow", "Taget Output", Q_NULLPTR));
        label_35->setText(QApplication::translate("MainWindow", "Bar", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Monitor", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabPlotter), QApplication::translate("MainWindow", "Page", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
