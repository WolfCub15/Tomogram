#include "View.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]){
	QApplication a(argc, argv);//создание объекта-приложения
	View w;//создание объекта управления окном
	w.setWindowTitle("Tomogram");//изменение заголовка окна
	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setVersion(3, 5);
	format.setProfile(QSurfaceFormat::CoreProfile);
	w.setFormat(format);
	w.load_Data("E://1.bin");
	w.show();//визуализация окна 
	return a.exec();//запуск цикла обработки сообщений приложения
}
