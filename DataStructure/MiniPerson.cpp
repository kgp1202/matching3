//MiniPerson.cpp
#include "DataStructure/MiniPerson.h"
#include "BasicModule/MLog.h"
#include "Constant.h"
#include "stdio.h"

MiniPerson::MiniPerson()
: _startTime(-1)
{
}

MiniPerson::MiniPerson(MiniPerson::DSIterator iter)
: _iter(iter)
{
	//current time check;
	_startTime = time(NULL);
}

void MiniPerson::setValue(MiniPerson::DSIterator iter){
	if(_startTime != -1){
		MLog::writeLog("setValue() in MiniPerson.cpp\n input iter is NULL\n");
	}

	_iter = iter;
	_startTime = time(NULL);
}

void MiniPerson::clearValue(){
	_startTime = -1;

	//_iter = NULL;
	//boost::shared_ptr<Person> personPtr = *_iter;
	//personPtr.reset();
	//_startTime = time(NULL);
}

bool MiniPerson::isEmpty(){
	if(_startTime == -1){
		return true;
	}else {
		return false;
	}
}

bool MiniPerson::timeCheck(){
	if(isEmpty()){
		MLog::writeLog("timeCheck() in MiniPerson.cpp\n");
	}

	double diffSec = difftime(time(NULL), _startTime);

	if(diffSec < 0){
		MLog::criticalLog("timeCheck() in MiniPerson.cpp\n");
	}

	if(diffSec >= WAKE_MINIMUM_TERM){
		return true;
	}else {
		return false;
	}
}
