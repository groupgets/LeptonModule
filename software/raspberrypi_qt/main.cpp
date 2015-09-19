#include <QApplication>

#include "mainwindow.h"

int main( int argc, char **argv )
{
    QApplication a(argc, argv);
	
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();

	return a.exec();
}

