//CommandQueue.cpp
#include "Command/CommandQueue.h"
#include "BasicModule/MLock.h"

#include <stdio.h>

CommandQueue::CommandQueue()
: _queue(), _mutex(new pthread_mutex_t()), _mutex_pop_possible(new pthread_mutex_t())
{
	pthread_mutex_init(_mutex, NULL);
	pthread_mutex_init(_mutex_pop_possible, NULL);

	pthread_mutex_lock(_mutex_pop_possible);
}

CommandQueue::~CommandQueue()
{
	pthread_mutex_destroy(_mutex);
	pthread_mutex_destroy(_mutex_pop_possible);
}

boost::shared_ptr<Command> CommandQueue::pop()
{
	MLock* m = new MLock(_mutex);
		
	boost::shared_ptr<Command> tempPtr = _queue.front();
	if(tempPtr == NULL)	//if _queue is empty
	{
		delete m;

		pthread_mutex_lock(_mutex_pop_possible);	//explicit lock!!

		return pop();
	}
	else
	{
		_queue.pop();

		if(_queue.size() == 1){
			//make pop possible
			pthread_mutex_unlock(_mutex_pop_possible);
		}

		delete m;		

		return tempPtr;
	}
}

void CommandQueue::push(boost::shared_ptr<Command> commandPtr)
{
	MLock m(_mutex);	

	_queue.push(commandPtr);

	if(_queue.size() == 1){
		//make pop possible
		pthread_mutex_unlock(_mutex_pop_possible);
	}
}

std::size_t CommandQueue::size()
{
	MLock m(_mutex);

	return _queue.size();
}	
