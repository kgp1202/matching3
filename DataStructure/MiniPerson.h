//MiniPerson.h
#include "BasicModule/MMemoryPool.h"
#include "BasicModule/MLog.h"
#include "boost/shared_ptr.hpp"
#include <time.h>
#include <set>

class Person;

//이 객체는 MiniDataStructure's mutex에 의해 Thread safe하다.
//즉 MiniDataStructure 내부에서만 생성해야 한다.
class MiniPerson : public MMemoryPool<MiniPerson> {
public:
	typedef std::set<boost::shared_ptr<Person> >::iterator DSIterator;
	
	explicit MiniPerson();
	explicit MiniPerson(DSIterator iter);	
	inline DSIterator getIter(){ return _iter; }
	void setValue(DSIterator iter);		//_iter과 _startTime 초기화
						//초기화 전에 비어있는지 확인
	
	void clearValue();			//값을 초기화 시켜준다.

	bool isEmpty();				//비어있는지 확인

	bool timeCheck();			//Time Check 해준다
						//시간이 경과되어져 있으면 true
						//아니면 false를 리턴.

	//FOR DEBUG
	void print();
private:
	DSIterator _iter;
	time_t _startTime;
};
