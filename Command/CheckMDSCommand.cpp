//CheckMDSComamnd.h
#include "CheckMDSCommand.h"
#include "DataStructure/DataStructure.h"
#include "boost/shared_ptr.hpp"
#include <vector>

CheckMDSCommand::~CheckMDSCommand(){}

void CheckMDSCommand::execute(DataStructure* ds){
	if(ds == NULL)
		MLog::criticalLog("execute() in CheckMDSCommand\n ds is NULL\n");


	ds->checkMDS();
}
