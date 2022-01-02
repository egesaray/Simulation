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
		position.heading = INVALID_INT;
		position.X = INVALID_INT;
		position.Y = INVALID_INT;
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
		cout << "current_speed : " << current_speed <<"\n";
		cout << "speed_goal : " << speed_goal << "\n";
		cout << "acceleration : " << acceleration << "\n";
	};

	void update(double deltaTime) {

		//math

	}
protected:
	Point position;
	double current_speed;	//m/sec
	double speed_goal;		//m/sec
	double acceleration;	//m/sec^2

};


class Vehicle : public TrafficParticipant {

public:
	Vehicle(Point Sposition, double Scurrent_speed, double Sspeed_goal, double Sacceleration,string Svehicle_maker):
		TrafficParticipant(Sposition,Scurrent_speed,Sspeed_goal,Sacceleration),
		vehicle_maker {Svehicle_maker}
	{}

	~Vehicle(){
		TrafficParticipant::~TrafficParticipant();
	}

	void SetSpeedGoal(double Sspeed_goal,double Sacceleration) {
		speed_goal = Sspeed_goal;
		acceleration = Sacceleration;
	}

	void Print() override {
		cout << "***Vehicle class print***\n";
		cout << "vehicle_maker : " << vehicle_maker << "\n";
		TrafficParticipant::Print();
	}
private:
	std::string vehicle_maker;
};

class Pedestrian : public TrafficParticipant {
public:

	Pedestrian(Point Sposition, bool isWalking, string Sname) :
		person_name{ Sname },
		TrafficParticipant(Sposition, 0.0f, 0.0f, 0.0f)
	{
		if (isWalking == true){
			current_speed = walk_speed;
		}
	}
	
	~Pedestrian() {
		//TrafficParticipant::~TrafficParticipant();
	}

	void Print() override {
		cout << "***Pedestrian class print***\n";
		cout << "person_name : " << person_name << "\n";
		TrafficParticipant::Print();
	}

private:
	std::string person_name;
	const float walk_speed = 1.5f;
};


class Simulation {
public:
	vector<unique_ptr<TrafficParticipant>> trafficParticipants ;
	float currentSimulationTime;

	void Start(vector<unique_ptr<TrafficParticipant>> StrafficParticipants) {
		//trafficParticipants = StrafficParticipants;      // ??????????????????????
		currentSimulationTime = 0.0f;
	}
	double GetSimSec() {
		return currentSimulationTime;
	}
	void Update(double deltaTime) {
		for (auto & ptr : trafficParticipants)
		{
			ptr->update(deltaTime);
		}
		currentSimulationTime = deltaTime;
	}
	void Print() {
		for (auto& ptr : trafficParticipants)
		{
			ptr->Print();
		}
		cout << "current simulation time: " << currentSimulationTime;
	}
};


int main() {

	Simulation S;
	cout << "enter number of traffic particiants ...\n";
	int noOfTrafficParticipants;
	cin >> noOfTrafficParticipants;
	
	float x, y, heading;
	double current_speed, goal_speed,acceleration;
	string name;
	bool isWalking;

	vector<unique_ptr<TrafficParticipant>> participants;

	for (size_t i = 0; i <noOfTrafficParticipants; i++)
	{
		cout << "Enter Type vehicle or pedestrian [V,P] \n";
		char type;
		cin >> type;
		if (type == 'V')
		{
			cout << "Enter maker_name:  ";
			cin >> name;
			cout << "Position X:  ";
			cin >> x;
			cout << "Position Y:  ";
			cin >> y;
			cout << "Position heading:  ";
			cin >> heading;
			cout << "current speed:  ";
			cin >> current_speed;
			cout << "goal speed";
			cin >> goal_speed;
			cout << "acceleration  ";
			cin >> acceleration;
			participants.push_back(unique_ptr<TrafficParticipant>(new Vehicle(Point{ x,y,heading }, current_speed, goal_speed, acceleration, name)));
		}
		else if (type == 'P') {
			cout << "Enter person_name:  ";
			cin >> name;
			cout << "Position X:  ";
			cin >> x;
			cout << "Position Y:  ";
			cin >> y;
			cout << "Position heading:  ";
			cin >> heading;
			cout << "isWalking[true/false]:  ";
			cin >> isWalking;
			participants.push_back(unique_ptr<TrafficParticipant>( new Pedestrian(Point{ x,y,heading }, isWalking, name)));
		}

	}
	
	/*
	S.Start(participants);
	cout << "\n Enter simulation tmie in seconds...";
	double simulationTime;
	cin >> simulationTime;

	for (size_t i = 0; i < simulationTime; i++) // should iterate after 0.1 sec ? for now let's imagine i=10 means 1 second
	{
		S.Update(i/10);

		if (i%10 == 0)
		{
			S.Print();
		}
	}
	*/

	return 0;
}