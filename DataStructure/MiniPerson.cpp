//MiniPerson.cpp
#include "DataStructure/MiniPerson.h"
#include "BasicModule/MLog.h"
#include "Constant.h"
#include "stdio.h"

MiniPerson::MiniPerson()
: _iter(), _startTime(-1)
{
}

MiniPerson::MiniPerson(MiniPerson::DSIterator iter)
: _iter(iter)
{
	//current time check;
	_startTime = time(NULL);
}

void MiniPerson::setValue(MiniPerson::DSIterator iter){
	boost::shared_ptr<Person> tempIter = *iter;
	if(tempIter.get() == NULL || _startTime == -1){
		MLog::writeLog("setValue() in MiniPerson.cpp\n _startTime is init\n");
	}

	_iter = iter;
	_startTime = time(NULL);
}

void MiniPerson::clearValue(){
	if(_iter->get() == NULL || _startTime == -1){
		MLog::writeLog("clearValue() in MiniPerson.cpp\n It is already clear\n");
	}

	boost::shared_ptr<Person> personPtr = *_iter;
	personPtr.reset();
	_startTime = time(NULL);
}

bool MiniPerson::isEmpty(){
	if(_iter->get() == NULL && _startTime == -1){
		return true;
	}else if(_iter->get() != NULL && _startTime != -1){
		return false;
	}else {
		MLog::writeLog("isEmpty() in MiniPerson.cpp\n");
		return false;
	}
}

bool MiniPerson::timeCheck(){
	double diffSec = difftime(time(NULL), _startTime);

	if(diffSec <= 0){
		MLog::criticalLog("timeCheck() in MiniPerson.cpp\n");
	}

	if(diffSec > WAKE_MINIMUM_TERM){
		return true;
	}else {
		return false;
	}
}
