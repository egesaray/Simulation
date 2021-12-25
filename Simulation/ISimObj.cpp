#include <iostream>
#include <memory>
#include <vector>
#include <string>

struct Point {
	float X,Y,heading;
};

class ISimObj {
public:
	virtual void SetPosition();
	virtual void SetSpeed();
	virtual void Print();
};