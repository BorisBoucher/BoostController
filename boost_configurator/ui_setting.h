/********************************************************************************
** Form generated from reading UI file 'setting.ui'
**
** Created by: Qt User Interface Compiler version 5.12.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTING_H
#define UI_SETTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Setting
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QLineEdit *logDirEdit;
    QPushButton *pushButton;
    QLabel *label_2;
    QComboBox *BCPortComboBox;
    QLabel *label_3;
    QComboBox *MUTPortComboBox;

    void setupUi(QDialog *Setting)
    {
        if (Setting->objectName().isEmpty())
            Setting->setObjectName(QString::fromUtf8("Setting"));
        Setting->setWindowModality(Qt::ApplicationModal);
        Setting->resize(610, 208);
        Setting->setModal(true);
        buttonBox = new QDialogButtonBox(Setting);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 160, 551, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        formLayoutWidget = new QWidget(Setting);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(10, 20, 591, 121));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(formLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        logDirEdit = new QLineEdit(formLayoutWidget);
        logDirEdit->setObjectName(QString::fromUtf8("logDirEdit"));

        horizontalLayout->addWidget(logDirEdit);

        pushButton = new QPushButton(formLayoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButton);


        formLayout->setLayout(0, QFormLayout::FieldRole, horizontalLayout);

        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        BCPortComboBox = new QComboBox(formLayoutWidget);
        BCPortComboBox->setObjectName(QString::fromUtf8("BCPortComboBox"));
        sizePolicy.setHeightForWidth(BCPortComboBox->sizePolicy().hasHeightForWidth());
        BCPortComboBox->setSizePolicy(sizePolicy);

        formLayout->setWidget(1, QFormLayout::FieldRole, BCPortComboBox);

        label_3 = new QLabel(formLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        MUTPortComboBox = new QComboBox(formLayoutWidget);
        MUTPortComboBox->setObjectName(QString::fromUtf8("MUTPortComboBox"));
        sizePolicy.setHeightForWidth(MUTPortComboBox->sizePolicy().hasHeightForWidth());
        MUTPortComboBox->setSizePolicy(sizePolicy);

        formLayout->setWidget(2, QFormLayout::FieldRole, MUTPortComboBox);


        retranslateUi(Setting);
        QObject::connect(buttonBox, SIGNAL(accepted()), Setting, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Setting, SLOT(reject()));

        QMetaObject::connectSlotsByName(Setting);
    } // setupUi

    void retranslateUi(QDialog *Setting)
    {
        Setting->setWindowTitle(QApplication::translate("Setting", "Dialog", nullptr));
        label->setText(QApplication::translate("Setting", "Log storage directory", nullptr));
        pushButton->setText(QApplication::translate("Setting", "...", nullptr));
        label_2->setText(QApplication::translate("Setting", "BC COM Port", nullptr));
        label_3->setText(QApplication::translate("Setting", "MUT Com Port", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Setting: public Ui_Setting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTING_H
