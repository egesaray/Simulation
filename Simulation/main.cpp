#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <math.h>
#include <stdio.h>
#include <chrono>
#include <thread>

const int INVALID_INT = -99999;
#define PI 3.14159265

using namespace std;
struct Point {
	float X, Y; // meter
	float heading; //degrees
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

	void update(float deltaTime) {


		//dikey bileþen sin(heading)* current_speed
		float Vy = sin(position.heading* PI/180) * current_speed;
		float Vx = cos(position.heading* PI/180) * current_speed;
		
		float chageInX = (Vx * deltaTime) + (0.5*acceleration*deltaTime * deltaTime);
		position.X += chageInX;
		float chageInY = (Vy * deltaTime) + (0.5 * acceleration * deltaTime * deltaTime);
		position.Y += chageInY;
		


		current_speed = current_speed +(acceleration * deltaTime);
		if (current_speed >= speed_goal)
		{
			acceleration = 0.0f;
		}


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

	~Vehicle(){}

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
	
	~Pedestrian() {}

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
	vector<unique_ptr<TrafficParticipant>> *trafficParticipants ;
	double currentSimulationTime;

	void Start(vector<unique_ptr<TrafficParticipant>> &StrafficParticipants) {
		trafficParticipants = &StrafficParticipants;
		currentSimulationTime = 0.0f;
	}
	double GetSimSec() {
		return currentSimulationTime;
	}
	void Update(float deltaTime) {
		for (auto & ptr : *trafficParticipants)
		{
			ptr->update(deltaTime);
		}
		currentSimulationTime += deltaTime;
	}
	void Print() {
		for (auto& ptr : *trafficParticipants)
		{
			ptr->Print();
		}
		cout << "current simulation time: " << currentSimulationTime <<"\n";
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
	string isWalking;

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
			cout << "goal speed:  ";
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
			cout << "isWalking[Y/N]:  ";
			cin >> isWalking;
			if (isWalking =="Y")
			{
				participants.push_back(unique_ptr<TrafficParticipant>(new Pedestrian(Point{ x,y,heading }, true, name)));
			}else if(isWalking == "N") {
				participants.push_back(unique_ptr<TrafficParticipant>(new Pedestrian(Point{ x,y,heading }, false, name)));
			}
		}

	}
	
	
	S.Start(participants);
	cout << "\n Enter simulation time in seconds:  ";
	int simulationTime;
	cin >> simulationTime;


	float realSecond = 0;
	int roundedSecond = 0;
	while (realSecond < simulationTime) {

		this_thread::sleep_for(chrono::milliseconds(100));

		realSecond += 0.1;
		roundedSecond++;
		S.Update(0.1);
		if (roundedSecond%10 ==0)
		{
			S.Print();
		}
	}


	return 0;
}