#include "Data.h"

short Data::get_min() {
	return min;
}

short Data::get_max() {
	return max;
}

int Data::get_width() {
	return width;
}

int Data::get_height() {
	return height;
}

int Data::get_depth() {
	return depth;
}

Data::~Data() {
	delete[]arr;
	width = 0;
	height = 0;
	depth = 0;
}

void Data::min_max() {
	min = arr[0];
	max = arr[0];
	for (int i = 0; i < (depth * height * width); i++) {
		if (min > arr[i]) min = arr[i];
		if (max < arr[i]) max = arr[i];
	}
	qDebug() << "MIN:"  << min << "   MAX:" << max << endl;
}

int Data::read_file(QString file_name) {
	setlocale(0, "RUS");
	std::ifstream file;
	file.open(file_name.toStdString(), std::ios::binary | std::ios::in);//открытие файла в двоичном режиме для чтения
	if (file.is_open() == 0) {//если файл не открыт
		qDebug() << "FILE CANNOT BE OPEN!" << endl;
	}
	qDebug() << "FILE OPEN!" << endl;
	int w, h, d;
	file.read((char*)&w, sizeof(int));
	file.read((char*)&h, sizeof(int));
	file.read((char*)&d, sizeof(int));
	if (w == 0 || h == 0 || d == 0) {
		qDebug() << "WRONG SIZE!" << endl;
		file.close();
		return 2;
	}
	qDebug() << "WIDTH =  " << w << endl << "HEIGHT = " << h << endl << "DEPTH = " << d;
	file.read((char*)&X, sizeof(float));
	file.read((char*)&Y, sizeof(float));
	file.read((char*)&Z, sizeof(float));
	qDebug() << "X = " << X << endl << "Y = " << Y << endl << "Z = " << Z << endl;
	if (arr != NULL) {
		delete[]arr;
	}
	width = w;
	height = h;
	depth = d;
	long arr_size = width * height * depth;
	arr = new short[arr_size];
	file.read((char*)arr, arr_size * sizeof(short));
	file.close();
	qDebug() << "CLOSE";
	min_max();
	return 0;
}

short& Data::operator[](const int i) {
	assert(i >= 0 && i < (width* height* depth));
	return arr[i];
}
