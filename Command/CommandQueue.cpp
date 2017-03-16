//CommandQueue.cpp
#include "Command/CommandQueue.h"
#include "BasicModule/MLock.h"

#include <stdio.h>

CommandQueue::CommandQueue()
: _queue(), _mutex(new pthread_mutex_t())
{
	pthread_mutex_init(_mutex, NULL);
}

CommandQueue::~CommandQueue()
{
	pthread_mutex_destroy(_mutex);
}

boost::shared_ptr<Command> CommandQueue::pop()
{
	MLock m(_mutex);
		
	boost::shared_ptr<Command> tempPtr = _queue.front();
	if(tempPtr == NULL)	//if _queue is empty
	{
		return tempPtr;
	}
	else
	{
		_queue.pop();
		return tempPtr;
	}
}

void CommandQueue::push(boost::shared_ptr<Command> commandPtr)
{
	MLock m(_mutex);	

	_queue.push(commandPtr);
}

std::size_t CommandQueue::size()
{
	MLock m(_mutex);

	return _queue.size();
}	
