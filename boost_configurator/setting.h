#ifndef SETTING_H
#define SETTING_H

#include <QDialog>

namespace Ui {
class Setting;
}

class Setting : public QDialog
{
	Q_OBJECT

public:
	explicit Setting(const QString& logDir,
					 const QString& BCPort,
					 const QString& MUTPort,
					 QWidget *parent = nullptr);
	~Setting();

	QString getLogDir();
	QString getBCPort();
	QString getMUTPort();
private:
	Ui::Setting *ui;
};

#endif // SETTING_H
