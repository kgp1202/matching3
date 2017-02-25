//CheckMDSComamnd.h
#include "CheckMDSCommand.h"
#include "DataStructure/DataStructure.h"
#include "DataStructure/MiniDataStructure.h"
#include "boost/shared_ptr.hpp"
#include <vector>

void CheckMDSCommand::execute(DataStructure* ds){
	//MDS's check()
	MiniDataStructure* mds = ds->getMDS();
	std::vector<int> wakingList = mds->check();

	//DS's wake()
	if(!wakingList.empty()){
		wake(wakingList);
	}
}
