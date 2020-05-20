#pragma once
#include <qdialog.h>
#include <qlineedit.h>
class Dialog : public QDialog{
	Q_OBJECT
private:
	QLineEdit* mi;
	QLineEdit* ma;
public:
	Dialog(QWidget* parent = 0);
	QString get_mi() const;
	QString get_ma() const;
};

