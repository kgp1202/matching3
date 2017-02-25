//CheckMDSComamnd.h
#include "CheckMDSCommand.h"
#include "DataStructure/DataStructure.h"
#include "boost/shared_ptr.hpp"
#include <vector>

void CheckMDSCommand::execute(DataStructure* ds){
	if(ds == NULL)
		criticalLog("execute() in CheckMDSCommand\n ds is NULL\n");

	//MDS's check()
	MiniDataStructure* mds = ds->getMDS();
	std::vector<int> wakingList = mds->check();

	//DS's wake()
	if(!wakingList.empty()){
		wake(wakingList);
	}
}
