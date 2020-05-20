#include "View.h"
#include "Dialog.h"
#include <qfiledialog.h>

int clamp(int min, int max, int val) {
	if (max < val)return max;
	else if (min > val) return min;
	else return val;
}
View::~View() {}
//конструктор
View::View(QWidget* parent): QOpenGLWidget(parent),QOpenGLFunctions(){
	layer = 0;
	xyz = 0;
	MA = 0;
	MI = 0;
	setFocus();
}


#ifndef QT_TEXTURE
void View::load_2D_texture() {
	glBindTexture(GL_TEXTURE_2D, VBO_texture);//привязывает текстуру к определенному текстурному типу
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_image.width(), texture_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_image.bits());//загружает текстуру в память видеокарты
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//устанавливает параметры для текущей текстуры, привязанной к текстурному блоку
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
#endif

void View::initializeGL() {
	initializeOpenGLFunctions();
	glClearColor(1,1,1,1);//устанавливаем заполняющий цвет
	glShadeModel(GL_SMOOTH);//устанавливаем режим сглаживания
	glMatrixMode(GL_MODELVIEW);//задаем модельно-видовую матрицу
	glLoadIdentity();//загрузка единичной матриц
#ifndef QT_TEXTURE
	glGenTextures(1, &VBO_texture);
#endif // QT_TEXTURE
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);//устанавливаем способ наложения текстуры(цвет текстуры перекрывает цвет пикселя)
}
//изменение размеров окна, вызывается при изменении размеров окна
void View::resizeGL(int n_width, int n_height) {
	glMatrixMode(GL_PROJECTION);//установка режима матрицы проекции
	glLoadIdentity();//заменяем на единичную
	if(xyz==0)glOrtho(0.0f, data.get_width() - 1, 0.0f, data.get_height() - 1, -1.0f, 1.0f);
	else if(xyz==1) glOrtho(0.0f, data.get_width() - 1, 0.0f, data.get_depth() - 1, -1.0f, 1.0f);
	else if (xyz == 2) glOrtho(0.0f, data.get_height() - 1, 0.0f, data.get_depth() - 1, -1.0f, 1.0f);//установка ортогонального преобразования
	glViewport(0, 0, n_width, n_height);//установка окна обзора
	update();
}

//визуализация томограммы
void View::paintGL() {
	qDebug() << "REPAINT" << visualization;//перекрашивание
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//очистка экрана
	switch (visualization) {
	case VISUALIZATION_QUADS: {
		predel = data.get_depth();
		visualization_quads();
		break;
	}
	case VISUALIZATION_QUADSTRIP: {
		predel = data.get_depth();
		visualization_quad_strip();
		break;
	}
	case VISUALIZATION_TEXTURE: {
		predel = data.get_depth();
		visualization_texture();
		break;
	}
	case VISUALIZATION_QUADS_Y: {
		predel = data.get_height();
		visualization_quads_Y();
		break;
	}
	case VISUALIZATION_QUADS_X: {
		predel = data.get_width();
		visualization_quads_X();
		break;
	}
	}
}

void View::load_Data(QString file_name) {
	data.read_file(file_name);
	if(xyz==0) resize(clamp(MIN_SIZE, MAX_SIZE, data.get_width()), clamp(MIN_SIZE, MAX_SIZE, data.get_height()));//установка размера окна
	else if(xyz==1) resize(clamp(MIN_SIZE, MAX_SIZE, data.get_width()), clamp(MIN_SIZE, MAX_SIZE, data.get_depth()));//установка размера окна
	else if (xyz == 2) resize(clamp(MIN_SIZE, MAX_SIZE, data.get_height()), clamp(MIN_SIZE, MAX_SIZE, data.get_depth()));//установка размера окна
	predel = data.get_depth();
}

void View::up() {
	if ((layer + 1) < predel) {
		layer += 1;
	}
	qDebug() << "layer : " << layer + 1;
	update();
}
void View::down() {
	if ((layer - 1) >= 0) {
		layer -= 1;
	}
	qDebug() << "layer : " << layer + 1;
	update();
}
void View::up10() {
	if ((layer + 10) < predel) {
		layer += 10;
	}
	qDebug() << "layer : " << layer + 1;
	update();
}
void View::down10() {
	if ((layer - 10) >= 0) {
		layer -= 10;
	}
	qDebug() << "layer : " << layer + 1;
	update();
}

//функция перевода значения плотностей томограммы в цвет
float View::Transfer_Function(short value) {
	float col;
	if (MA == 0 && MI == 0)	col = (value - data.get_min()) * 1.f / (data.get_max() - data.get_min());
	else col = (value -MI) * 1.f / (MA - MI);
	return col;
}

//Отрисовка томограммы четырехугольниками
void View::visualization_quads() {
	glDisable(GL_TEXTURE_2D);
	float col;
	int wi = data.get_width();
	int hi = data.get_height();
	for (int y = 0; y < (hi - 1); y++) {
		for (int x = 0; x < (wi - 1); x++) {
			glBegin(GL_QUADS);
			//1 вершина
			col = Transfer_Function(data[layer * wi * hi + y * wi + x]);
			glColor3f(col,col,col);
			glVertex2i(x, y);
			//2 вершина
			col = Transfer_Function(data[layer * wi * hi + (y+1) * wi + x]);
			glColor3f(col,col,col);
			glVertex2i(x, (y + 1));
			//3 вершина
			col = Transfer_Function(data[layer * wi * hi + (y+1)* wi + x + 1]);
			glColor3f(col, col, col);
			glVertex2i((x + 1), (y + 1));
			//4 вершина
			col = Transfer_Function(data[layer * wi * hi + y * wi + x + 1]);
			glColor3f(col, col, col);
			glVertex2i((x + 1), y);
			glEnd();
		}
	}
}
void View::visualization_quad_strip() {
	glDisable(GL_TEXTURE_2D);
	float col;
	int wi = data.get_width();
	int hi = data.get_height();
	for (int y = 0; y < (hi-1); y++) {
		glBegin(GL_QUAD_STRIP);
		//2 вершина
		col = Transfer_Function(data[layer * wi * hi + (y + 1) * wi ]);
		glColor3f(col,col,col);
		glVertex2i(0, (y + 1));
		//1 вершина
		col = Transfer_Function(data[layer * wi * hi + y * wi ]);
		glColor3f(col, col, col);
		glVertex2i(0, y);
		for (int x = 0; x < (wi-1); x++) {
			//3 вершина
			col = Transfer_Function(data[layer * wi * hi + (y + 1) * wi + x + 1]);
			glColor3f(col, col, col);
			glVertex2i((x + 1), (y + 1));
			//4 вершина
			col = Transfer_Function(data[layer * wi * hi + y * wi + x + 1]);
			glColor3f(col, col, col);
			glVertex2i((x + 1), y);
		}
		glEnd();
	}
}
void View::visualization_quads_Y(){
	glDisable(GL_TEXTURE_2D);
	float col;
	int wi = data.get_width();
	int hi = data.get_height();
	int d = data.get_depth();
	for (int z = 0; z < (d - 1); z++) {
		for (int x = 0; x < (wi - 1); x++) {
			glBegin(GL_QUADS);
			//1 вершина
			col = Transfer_Function(data[z * wi * hi + layer * wi + x]);
			glColor3f(col, col, col);
			glVertex2i(x, z);
			//2 вершина
			col = Transfer_Function(data[(z + 1) * wi * hi + layer * wi + x]);
			glColor3f(col, col, col);
			glVertex2i(x, (z + 1));
			//3 вершина
			col = Transfer_Function(data[(z + 1) * wi * hi + layer * wi + x + 1]);
			glColor3f(col, col, col);
			glVertex2i((x + 1), (z + 1));
			//4 вершина
			col = Transfer_Function(data[z * wi * hi + layer * wi + x + 1]);
			glColor3f(col, col, col);
			glVertex2i((x + 1), z);
			glEnd();
		}
	}
}
void View::visualization_quads_X(){
	glDisable(GL_TEXTURE_2D);
	float col;
	int wi = data.get_width();
	int hi = data.get_height();
	int d = data.get_depth();
	for (int z = 0; z < (d - 1); z++) {
		for (int y = 0; y < (hi - 1); y++) {
			glBegin(GL_QUADS);
			//1 вершина
			col = Transfer_Function(data[z * wi * hi + y * wi + layer]);
			glColor3f(col, col, col);
			glVertex2i(y, z);
			//2 вершина
			col = Transfer_Function(data[(z + 1) * wi * hi + y * wi + layer]);
			glColor3f(col, col, col);
			glVertex2i(y, (z + 1));
			//3 вершина
			col = Transfer_Function(data[(z + 1) * wi * hi + (y+1) * wi + layer ]);
			glColor3f(col, col, col);
			glVertex2i((y + 1), (z + 1));
			//4 вершина
			col = Transfer_Function(data[z * wi * hi + (y + 1) * wi + layer]);
			glColor3f(col, col, col);
			glVertex2i((y + 1), z);
			glEnd();
		}
	}
}

//рисует один прямоугольник с наложенной текстурой
void View::visualization_texture() {
	glEnable(GL_TEXTURE_2D);
#ifdef QT_TEXTURE
	texture->bind();
#else
	glBindTexture(GL_TEXTURE_2D, VBO_texture);
#endif // QT_TEXTURE
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2i(0, 0);
	glTexCoord2f(0, 1);
	glVertex2i(0, data.get_height());
	glTexCoord2f(1, 1);
	glVertex2i(data.get_width(), data.get_height());
	glTexCoord2f(1, 0);
	glVertex2i(data.get_width(), 0);
	glEnd();
}

//Отрисовка текстурированного прямоугольника
void View::gen_texture_image() {
	int wi = data.get_width();
	int hi = data.get_height();
	texture_image = QImage(wi, hi, QImage::Format_RGB32);
	for (int y = 0; y < (hi); y++) {
		for (int x = 0; x < (wi); x++) {
			int col = (int)(255*Transfer_Function(data[layer * wi * hi + wi * y + x]));
			QColor c(col, col, col);
			texture_image.setPixelColor(x, y, c);
		}
	}
#ifdef QT_TEXTURE
	texture = new QOpenGLTexture(texture_image);
	texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	texture->setMagnificationFilter(QOpenGLTexture::Linear);
#else 
	load_2D_texture();
#endif // QT_TEXTURE
}

void View::keyPressEvent(QKeyEvent* event) {
	if (event->nativeVirtualKey() == Qt::Key_U) up();// Подняться на слой выше
	else if (event->nativeVirtualKey() == Qt::Key_D) down();// Опуститься на слой ниже
	else if (event->nativeVirtualKey() == Qt::Key_I) up10();//подняться на 10 слоев
	else if (event->nativeVirtualKey() == Qt::Key_F) down10();//опуститься на 10 слоев
	else if (event->nativeVirtualKey() == Qt::Key_N) {
		visualization = (visualization + 1) % 5;// Переключиться на следующий тип рендеринга
		layer = 0;
	}
	else if (event->nativeVirtualKey() == Qt::Key_W) {
		Dialog* d = new Dialog;
		if (d->exec() == QDialog::Accepted) {
			int min = d->get_mi().toInt();
			int max = d->get_ma().toInt();
			MI=min;
			MA=max;
		}
		
	}
	else if (event->nativeVirtualKey() == Qt::Key_X) {
		xyz = 2;
	}
	else if (event->nativeVirtualKey() == Qt::Key_Y) {
		xyz = 1;
	}
	else if (event->nativeVirtualKey() == Qt::Key_Z) {
		xyz = 0;
	}
	else if (event->nativeVirtualKey() == Qt::Key_S) {
		QString k = QFileDialog::getOpenFileName(0, "Open File", "/E:/*.bin");
		load_Data(k);
		layer = 0;
		setFocus();
	}
	if (visualization == VISUALIZATION_TEXTURE) {
		gen_texture_image();
	}
	update();// Подняться на слой выше
}
