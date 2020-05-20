#pragma once
//����� ��� ������ ������ �� �����
#include <iostream>
#include <locale>
#include <fstream>
#include <stdio.h>
#include "qdebug.h"
class Data {
private:
	float X, Y, Z;//���������� �� x,y � z
	short* arr;//������ ����������
	int width, height, depth;//������� ����������
	short min, max;//����������� � ������������ �������� ���������
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
	int read_file(QString file_name);//��������� ���� � ��������� ������� � �������� ������
	short& operator[](const int i);
};

