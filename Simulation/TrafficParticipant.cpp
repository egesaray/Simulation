#include "ISimObj.cpp"
#include <iostream>
#include <memory>
#include <vector>
#include <string>


class TrafficParticipant: public ISimObj {
public:

private:
	Point position;
	float current_speed;
	float speed_goal;
	float acceleration;
};


class Vehicle : public TrafficParticipant {

private:
	std::string vehicle_maker;
};

class Pedestrian : public TrafficParticipant {

private:
	std::string person_name;
};
