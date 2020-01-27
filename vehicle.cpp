#include "vehicle.hpp"
using namespace std;

        Vehicle::Vehicle(Road *r, int len, int wid, int h, string col, int max_sp, int acc, char disp, int i, vector<int> position)
        {
                rd = r;
                length = len;
                width = wid;
								height=h;
                id = i;
                color=col;
                max_speed=max_sp;
                acceleration=acc;
                pos = position;
                velocity.push_back(max_sp);
                velocity.push_back(0);
                display = disp;
        }

        Vehicle::Vehicle(const Vehicle &obj)
        {
                rd=obj.rd;
                length = obj.length;
                width = obj.width;
								height = obj.height;
                id = obj.id;
                color = obj.color;
                max_speed = obj.max_speed;
                acceleration = obj.acceleration;
                pos = obj.pos;
                velocity = obj.velocity;
                display = obj.display;
        }
        int Vehicle::get_length() {return length;}
        int Vehicle::get_width() {return width;}
				int Vehicle::get_height() {return height;}
        int Vehicle::get_id() {return id;}
        string Vehicle::get_color() {return color;}
        int Vehicle::get_max_speed() {return max_speed;}
        int Vehicle::get_acceleration() {return acceleration;}
        vector<int> Vehicle::get_pos() {return pos;}
        vector<int> Vehicle::get_velocity() {return velocity;}
        int Vehicle::get_display_char() {return display;}

        void Vehicle::set_acceleration(int acc) {acceleration=acc;}
        void Vehicle::set_pos(int x, int y) {pos[0]=x; pos[1]=y;}
        void Vehicle::set_velocity(vector<int> v) {velocity[0]=v[0]; velocity[1]=v[1];}
				void Vehicle::set_velocity(int x, int y) {velocity[0]=x; velocity[1]=y;}
