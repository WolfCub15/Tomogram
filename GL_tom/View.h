#include <QKeyEvent>
#include <QOpenGLWidget>
#include <qopengl.h>
#include <QtWidgets/QWidget>
#include <qopengltexture.h>
#include <qopenglfunctions.h>
#include <QtOpenGL/qgl.h>
#include <QWidget>
#include <qimage.h>
#include <algorithm>
#include <QString>
#include "Data.h"
#define QT_TEXTURE
//QT_BEGIN_NAMESPACE
//QT_END_NAMESPACE

class View : public QOpenGLWidget , protected QOpenGLFunctions{
	Q_OBJECT
private:
	int xyz;
	int predel;
	int MA;
	int MI;
	Data data;
	int layer;//слой
	void up();
	void down();
	void up10();
	void down10();
	static const int MAX_SIZE = 3000;//размеры окна
	static const int MIN_SIZE = 750;
	int visualization = 0;
	static const int VISUALIZATION_QUADS = 0;
	static const int VISUALIZATION_QUADSTRIP = 1;
	static const int VISUALIZATION_TEXTURE = 2;
	static const int VISUALIZATION_QUADS_Y = 3;
	static const int VISUALIZATION_QUADS_X = 4;
	void visualization_quads();
	void visualization_quad_strip();
	void visualization_texture();
	void visualization_quads_Y();
	void visualization_quads_X();
	float Transfer_Function(short value);
	void gen_texture_image();
	QImage texture_image;
#ifdef QT_TEXTURE
	QOpenGLTexture* texture;
#else
	GLuint VBO_texture;//хранит номер текстуры в памяти выдеокарты
	void load_2D_texture();
#endif // QT_TEXTURE
protected:
	void initializeGL()override;//Устанавливает контекст рендеринга OpenGL, определяет списки отображения
	void resizeGL(int n_width, int n_height)override;//устанавливает область просмотра OpenGL, проекцию и т. д. 
	void paintGL()override;//визуализия сцены OpenGL
public:
	View(QWidget* parent = nullptr);
	void load_Data(QString file_name);
	~View();
public slots:
	void keyPressEvent(QKeyEvent* event) override;
};
