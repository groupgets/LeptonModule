#include <iostream>

#include <QtCore/QEventLoop>
#include <QtCore/QTimer>

#include "LeptonThread.h"
#include "Lepton.h"
#include "LeptonAction.h"
#include "LeptonActionQt.h"
#include "Lepton_I2C.h"

LeptonThread::LeptonThread() : QThread()
{
}

LeptonThread::~LeptonThread() {
}

void LeptonThread::setLepton(Lepton *newLepton)
{
	myLepton = newLepton;
}

void LeptonThread::run()
{
	//create the initial image
	myImage = QImage(myLepton->getWidth(), myLepton->getHeight(), QImage::Format_RGB888);
	LeptonActionQt *leptonAction = new LeptonActionQt();
	leptonAction->setQImage(&myImage);

	//open Lepton port
	myLepton->open();

	while(true) {

		//read data packets from lepton over SPI
		int segment_number = myLepton->readFrameData(leptonAction);

		//lets emit the signal for update
		emit updateImage(myImage);
	}
	
	//finally, close Lepton just bcuz
	myLepton->close();
}

void LeptonThread::performFFC() {
	//perform FFC
	lepton_perform_ffc();
}

