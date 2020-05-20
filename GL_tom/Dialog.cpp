#include "Dialog.h"
#include <qlabel.h>
#include <qpushbutton.h>
#include <qgridlayout.h>
Dialog::Dialog(QWidget* parent): QDialog(parent ,Qt::WindowTitleHint | Qt::WindowSystemMenuHint) {
	mi = new QLineEdit;
	ma = new QLineEdit;
	QLabel* label_mi=new QLabel("&MIN");
	QLabel* label_ma = new QLabel("&MAX");
	label_mi->setBuddy(mi);
	label_ma->setBuddy(ma);

	QPushButton* go = new QPushButton("&GO");
	connect(go, SIGNAL(clicked()), SLOT(accept()));

	QGridLayout* l = new QGridLayout;
	l->addWidget(label_mi, 1, 1);
	l->addWidget(label_ma, 1, 5);
	l->addWidget(mi, 3, 1);
	l->addWidget(ma, 3, 5);
	l->addWidget(go, 5, 3);
	setLayout(l);
}

QString Dialog::get_mi() const{
	return mi->text();
}

QString Dialog::get_ma() const{
	return ma->text();
}
