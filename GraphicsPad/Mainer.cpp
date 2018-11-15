#include <Qt/qapplication.h>
#include <MeGlWindow.h>
#include <MeWidget.h>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	//QWidget meWidg;
	//meWidg.show();

	MeGlWindow meWindow;
	//meWindow.setFixedSize(1024, 1024);
	meWindow.show();

	return app.exec();
	
}
