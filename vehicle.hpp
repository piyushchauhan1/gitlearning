#ifndef vehicle_h
#define vehicle_h

class Road;
#include <vector>
#include <string>
#include "road.hpp"
using namespace std;

class Vehicle
{
private:
        Road *rd;
        int length;
        int width;
	int height;
        int id;
        string color;
        int max_speed;
        int acceleration; //Make sure acceleration is not greater than max_speed.
        vector<int> pos;
        vector<int> velocity;
        char display;
        // In xml file when multiple vehicles are defined simultaneously, in the simulation they will
        //side by side. If no of vehicles exceeds road width, it will throw an error.

        //All vehicles will be spawned at the starting line.
public:
				int opengl_diagonal;

        Vehicle(Road *rd, int len, int wid, int h, string col, int max_sp, int acc, char disp, int i, vector<int> position);

        Vehicle(const Vehicle &obj);

        int get_length();
        int get_width();
				int get_height();
        int get_id();
        string get_color();
        int get_max_speed();
        int get_acceleration();
        vector<int> get_pos();
        vector<int> get_velocity();
        int get_display_char();
        int get_accident_truth();

        void set_acceleration(int acc);
        void set_pos(int x, int y);
        void set_velocity(vector<int> v);
	void set_velocity(int x, int y);

	bool can_go_left();
	bool can_go_right();
        void update();

};
#endif
