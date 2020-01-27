#include "road.hpp"
using namespace std;

Road::Road(int len, int wid, int dist)
{
        length = len;
        width = wid;
        signal_color = 0; //0 is RED and 1 is GREEN
        signal_distance = dist;
        this->road_map.resize(wid, vector<char>(len, ' '));
        for (int i = 0; i < length; i++)
        {
                road_map[signal_distance][i] = '|';
        }
}

int Road::get_length() { return length; }
int Road::get_width() { return width; }
int Road::get_sig_distance() { return signal_distance; }
int Road::get_signal_color() { return signal_color; }

void Road::set_sig_colour(int col) { signal_color = col; }

void Road::update(vector<Vehicle *> a)
{
        vector<vector<char>> refresh(width, vector<char>(length, ' '));
        road_map = refresh;
        for (int i = 0; i < a.size(); i++)
        {
                for (int j = 0; j < a[i]->get_width(); j++)
                {
                        for (int k = 0; k < a[i]->get_length(); k++)
                        {
                                int p = a[i]->get_pos()[0] - j;
                                int q = a[i]->get_pos()[1] + k;
                                if (p >= 0 && p < width && q >= 0 && q < length)
                                {
                                        road_map[p][q] = a[i]->get_display_char();
                                }
                        }
                }
        }

        if (signal_color == 0)
        {
                for (int i = 0; i < length; i++)
                {
                        road_map[signal_distance][i] = '|';
                }
        }
}

void Road::display()
{
        for (int i = 0; i < width; i++)
        {
                cout << '-';
        }
        cout << endl;
        for (int j = 0; j < length; j++)
        {
                for (int i = 0; i < width; i++)
                        cout << road_map[i][length - 1 - j];
                cout << endl;
        }
        for (int i = 0; i < width; i++)
        {
                cout << '-';
        }
}
//For vehicles and road.
//  width w
// ----------
//|         |
//|         |   length l
//|         |
// ----------
