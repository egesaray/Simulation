#include <iostream>
#include <memory>
#include <vector>
#include <string>

const int INVALID_INT = -99999;

using namespace std;
struct Point {
	float X, Y, heading;
};

class ISimObj {
public:
	virtual void SetPosition(double x, double y, double Sheading) {};
	virtual void SetSpeed(double Sspeed_goal) {};
	virtual void Print() {};
};

class TrafficParticipant : public ISimObj {
public:
	TrafficParticipant(Point Sposition, double Scurrent_speed, double Sspeed_goal, double Sacceleration) :
		position{ Sposition },
		current_speed{Scurrent_speed},
		speed_goal{Sspeed_goal},
		acceleration{Sacceleration}
	{}

	~TrafficParticipant() {
		current_speed = INVALID_INT;
		speed_goal = INVALID_INT;
		acceleration = INVALID_INT;
	}

	void SetPosition(double x, double y, double Sheading) override {
		position.X= x;
		position.Y = y;
		position.heading = Sheading;
	};

	void SetSpeed( double Sspeed_goal) override {
		speed_goal = Sspeed_goal;
	};
	void Print() override {
		cout << "Position:  X: " << position.X << "  Y: " << position.Y << "  heading: " << position.heading << "\n";
		cout << "current_speed : " << current_speed;
		cout << "speed_goal : " << speed_goal;
		cout << "acceleration : " << acceleration;
	};

	void update(double deltaTime) {

		//math

	}


private:
	Point position;
	double current_speed;	//m/sec
	double speed_goal;		//m/sec
	double acceleration;	//m/sec^2
};


class Vehicle : public TrafficParticipant {

private:
	std::string vehicle_maker;
};

class Pedestrian : public TrafficParticipant {

private:
	std::string person_name;
};


class Simulation {
public:

private:

};


int main() {
	Point point{ 3,5,6 };

	TrafficParticipant S(point, 5,5,8);
	S.Print();

	return 0;
}