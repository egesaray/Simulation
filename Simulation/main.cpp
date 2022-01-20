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

class ISimObj {     //changed (*Interface classes have pure virtual member functions.)
public:
	virtual void SetPosition() = 0;
	virtual void SetSpeed()=0;
	virtual void Print()=0;
};

class TrafficParticipant : public ISimObj {
public:
	TrafficParticipant(Point Sposition, double Scurrent_speed, double Sspeed_goal, double Sacceleration) :
		position{ Sposition },
		current_speed{Scurrent_speed},
		speed_goal{Sspeed_goal},
		acceleration{Sacceleration}
	{}

	virtual ~TrafficParticipant() = default; // changed (*Destructor should be virtual.)

	void SetPosition(double x, double y, double Sheading) {
		position.X= x;
		position.Y = y;
		position.heading = Sheading;
	};

	void SetSpeed( double Sspeed_goal) {
		speed_goal = Sspeed_goal;
	};
	void Print() override {
		std::cout << "Position:  X: " << position.X << "  Y: " << position.Y << "  heading: " << position.heading << "\n";
		std::cout << "current_speed : " << current_speed <<"\n";
		std::cout << "speed_goal : " << speed_goal << "\n";
		std::cout << "acceleration : " << acceleration << "\n";
	};

	void update(float deltaTime) {

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
		std::cout << "***Vehicle class print***\n";
		std::cout << "vehicle_maker : " << vehicle_maker << "\n";
		TrafficParticipant::Print();
	}
private:
	std::string vehicle_maker;
};

class Pedestrian : public TrafficParticipant {
public:

	Pedestrian(Point Sposition, bool isWalking, string Sname) :
		person_name{ Sname },
		//TrafficParticipant(Sposition, 0.0f, 0.0f, 0.0f)
		TrafficParticipant(Sposition, (isWalking ? current_speed: walk_speed),0,0) // changed
	{}
	
	~Pedestrian() {}

	void Print() override {
		cout << "***Pedestrian class print***\n";
		cout << "person_name : " << person_name << "\n";
		TrafficParticipant::Print();
	}

private:
	std::string person_name;
	static constexpr double walk_speed = 1.5; // changed (*Use static const expr to define walking speed)
};


class Simulation { // changed (	*Start method shall get a vector, and move it to the member attribute vector.
	// * Using a vector raw pointer is not sensible, then you would need to use a destructor)
public:
	vector<unique_ptr<TrafficParticipant>> trafficParticipants ;
	double currentSimulationTime;

	void Start(vector<unique_ptr<TrafficParticipant>> StrafficParticipants) {
		trafficParticipants = move(StrafficParticipants);
		currentSimulationTime = 0.0f;
	}
	double GetSimSec() {
		return currentSimulationTime;
	}
	void Update(float deltaTime) {
		for (auto & ptr : trafficParticipants)
		{
			ptr->update(deltaTime);
		}
		currentSimulationTime += deltaTime;
	}
	void Print() {
		for (auto& ptr : trafficParticipants)
		{
			ptr->Print();
		}
		cout << "current simulation time: " << currentSimulationTime <<"\n";
	}
};


int main() {

	Simulation S;
	std::cout << "enter number of traffic particiants ...\n";
	int noOfTrafficParticipants;
	cin >> noOfTrafficParticipants;
	
	float x, y, heading;
	double current_speed, goal_speed,acceleration;
	string name;
	string isWalking;

	vector<unique_ptr<TrafficParticipant>> participants{ noOfTrafficParticipants }; //changed (* Set the size of vector at the creation)

	for (size_t i = 0; i <5; ++i) //changed (* Prefer pre-increment rather than post-increment)
	{
		std::cout << "Enter Type vehicle or pedestrian [V,P] \n";
		char type;
		std::cin >> type;
		if (type == 'V')
		{
			std::cout << "Enter maker_name:  ";
			std::cin >> name;
			std::cout << "Position X:  ";
			std::cin >> x;
			std::cout << "Position Y:  ";
			std::cin >> y;
			std::cout << "Position heading:  ";
			std::cin >> heading;
			std::cout << "current speed:  ";
			std::cin >> current_speed;
			std::cout << "goal speed:  ";
			std::cin >> goal_speed;
			std::cout << "acceleration  ";
			std::cin >> acceleration;
			participants[i] = make_unique<Vehicle>(Point{ x,y,heading }, current_speed, goal_speed, acceleration, name); // changed (* The vector unique_ptr type shall be TrafficParicipant, but its element shall be created as Vehicle or Pedestrian to avoid object slicing.
			// * Prefer make_unique to create unique pointer)
		}
		else if (type == 'P') {
			std::cout << "Enter person_name:  ";
			std::cin >> name;
			std::cout << "Position X:  ";
			std::cin >> x;
			std::cout << "Position Y:  ";
			std::cin >> y;
			std::cout << "Position heading:  ";
			std::cin >> heading;
			std::cout << "isWalking[Y/N]:  ";
			std::cin >> isWalking;
			if (isWalking =="Y"){
				participants[i] = make_unique<Pedestrian>(Point{ x,y,heading }, true, name); // changed (* The vector unique_ptr type shall be TrafficParicipant, but its element shall be created as Vehicle or Pedestrian to avoid object slicing.
			// * Prefer make_unique to create unique pointer)
			}else if(isWalking == "N") {
				participants[i] = make_unique<Pedestrian>(Point{ x,y,heading }, false, name); // changed (* The vector unique_ptr type shall be TrafficParicipant, but its element shall be created as Vehicle or Pedestrian to avoid object slicing.
			// * Prefer make_unique to create unique pointer)
			}
		}

	}
	
	
	S.Start(participants);
	cout << "\n Enter simulation time in seconds:  ";
	int simulationTime;
	std::cin >> simulationTime;


	int loopCounter = 0;
	while (S.GetSimSec() < simulationTime) { // changed (*manage simulation time in the simulation class)

		this_thread::sleep_for(chrono::milliseconds(100));

		S.Update(0.1);
		if (loopCounter %10 ==0){
			S.Print();
		}
		loopCounter++;
	}


	return 0;
}