#ifndef _IMANAGER_HG_
#define _IMANAGER_HG_

class iManager
{
public:
	virtual ~iManager() {}

	virtual void Update(float deltaTime) = 0;
	//will this get complicated? :o
private:

};



#endif // !_IMANAGER_HG_
