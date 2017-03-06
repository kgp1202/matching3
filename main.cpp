#include "Command/AddDSCommand.h"
#include "Command/CheckMDSCommand.h"
#include "Command/RecvCommand.h"
#include "Command/CommandQueue.h"
#include "DataStructure/DataStructure.h"
#include "DataStructure/Person.h"
#include <string.h>
#include <stdio.h>

int main(){

	CommandQueue* cqPtr = new CommandQueue();
	DataStructure ds = DataStructure(cqPtr);

	boost::shared_array<char> msg(new char[10]);
	int distance = 5;
	memcpy(msg.get(), &distance, sizeof(int));	

	ds.add(6, msg);
}
