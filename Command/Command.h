#pragma once

class DataStructure;

class Command{
public:
	virtual void execute(DataStructure*) = 0;
	virtual ~Command() = 0;
};
