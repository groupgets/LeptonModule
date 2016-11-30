#include <QApplication>
#include <QThread>
#include <QMutex>
#include <QMessageBox>

#include <QColor>
#include <QLabel>
#include <QtDebug>
#include <QString>
#include <QPushButton>

#include "LeptonThread.h"
#include "MyLabel.h"

int main( int argc, char **argv )
{
	//create the app
	QApplication a( argc, argv );
	
	QWidget *myWidget = new QWidget;
	myWidget->setGeometry(400, 300, 760, 480);

	//create an image placeholder for myLabel
	//fill the top left corner with red, just bcuz
	QImage myImage;
	myImage = QImage(320, 240, QImage::Format_RGB888);
	QRgb red = qRgb(255,0,0);
	for(int i=0;i<80;i++) {
		for(int j=0;j<60;j++) {
			myImage.setPixel(i, j, red);
		}
	}

	//create a label, and set it's image to the placeholder
	MyLabel myLabel(myWidget);
	myLabel.setGeometry(10, 10, 640, 480);
	myLabel.setPixmap(QPixmap::fromImage(myImage));

	//create a FFC button
	QPushButton *button1 = new QPushButton("Perform FFC", myWidget);
	button1->setGeometry(660, 480/6, 100, 30);

	//create a Snapshot button
	QPushButton *button2 = new QPushButton("Snapshot", myWidget);
	button2->setGeometry(660, 480/6*2, 100, 30);

	//create a GrayScale button
	QPushButton *button3 = new QPushButton("GrayScale", myWidget);
	button3->setGeometry(660, 480/6*3, 100, 30);

	//create a Rainbow button
	QPushButton *button4 = new QPushButton("Rainbow", myWidget);
	button4->setGeometry(660, 480/6*4, 100, 30);

	//create a IronBlack button
	QPushButton *button5 = new QPushButton("IronBlack", myWidget);
	button5->setGeometry(660, 480/6*5, 100, 30);

	//create a thread to gather SPI data
	//when the thread emits updateImage, the label should update its image accordingly
	LeptonThread *thread = new LeptonThread();
	QObject::connect(thread, SIGNAL(updateImage(QImage)), &myLabel, SLOT(setImage(QImage)));
	
	//connect ffc button to the thread's ffc action
	QObject::connect(button1, SIGNAL(clicked()), thread, SLOT(performFFC()));
	thread->start();

	//connect snapshot button to the thread's snapshot action
	QObject::connect(button2, SIGNAL(clicked()), thread, SLOT(snapImage()));
	thread->start();
	
	//connect GrayScale button to the thread's snapshot action
	QObject::connect(button3, SIGNAL(clicked()), thread, SLOT(greyMap()));
	thread->start();

	//connect Rainbow button to the thread's snapshot action
	QObject::connect(button4, SIGNAL(clicked()), thread, SLOT(rainMap()));
	thread->start();

	//connect IronBlack button to the thread's snapshot action
	QObject::connect(button5, SIGNAL(clicked()), thread, SLOT(ironMap()));
	thread->start();

	myWidget->show();

	return a.exec();
}
