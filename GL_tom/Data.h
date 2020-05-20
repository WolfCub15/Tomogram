#pragma once
//класс для чтения данных из файла
#include <iostream>
#include <locale>
#include <fstream>
#include <stdio.h>
#include "qdebug.h"
class Data {
private:
	float X, Y, Z;//расстояние по x,y и z
	short* arr;//массив плотностей
	int width, height, depth;//размеры томограммы
	short min, max;//минимальное и максимальное значение плотности
public:
	short get_min();
	short get_max();
	int get_width();
	int get_height();
	int get_depth();
	Data() : width(0), height(0), depth(0), min(0), max(0), X(0), Y(0), Z(0) {
		arr = NULL;
	}
	~Data();
	void min_max();
	int read_file(QString file_name);//прочитать файл и вычислить минимум и максимум данных
	short& operator[](const int i);
};

