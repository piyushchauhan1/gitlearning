#include <GL/glut.h>    // Header File For The GLUT Library
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <unistd.h>     // needed to sleep
#include <string>
#include <iostream>
#include "road.hpp"
#include "vehicle.hpp"

////parser
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include "/home/h_bond/rapidxml-1.13/rapidxml.hpp"  //To the USER: Change this to the path of where the rapidXML hpp file is in your computer.

using namespace rapidxml;
using namespace std;

#define ESCAPE 27
#define SPACE 32
using namespace std;
// Helper functions
Road *r;
vector<Vehicle *> vehicles;
vector<int> sig_time;
bool road_empty;
int time_step;
int sig_time_counter;
int vehicle_counter;
int enter=0;



vector<int> check_movable_zone(Vehicle a,vector<vector<char>> rd)
{
	int x_pos=a.get_pos()[0];
	int y_pos=a.get_pos()[1];
	int length=a.get_length();
	int width=a.get_width();
	int x_velocity=a.get_velocity()[0];
	int rd_width=rd[0].size()-1;

	int x_new_pos=x_pos+x_velocity;
	int y_new_pos=y_pos;
	int lane_change=5; //0 not possible, decelerate. -1 right. 1 left. 5 move_forward. 4 signal_stop.
	bool is_signal=false;
	int m;
	// for(int i=x_new_pos; i>x_new_pos - width; i--)
	// cout << x_pos << " " << x_new_pos << endl;
	for(int i=x_pos+1; i<=x_new_pos; i++)
    {
      // for(int j=y_new_pos; j>y_new_pos - length; j--)
      for(int j=y_pos+length-1; j>=y_pos; j--)

        {
          if(i<rd.size() && i>=0 && j<rd[0].size() && j>=0)
            {
							if(rd[i][j]!=' ')
                {
                  m=i-1;
                  lane_change=0;
                  if(rd[i][j]=='|')
                    {
                      is_signal=true;
                      break;
                    }
                }
            }
        }
      if(lane_change!=5)
        {
          break;
        }
    }

	if(lane_change==5)
    {
      vector<int> a{0,5}; //return = new_x_pos, decision
      return a;
    }
	if(lane_change==0)
    {
      if(is_signal)
        {
          vector<int> a{m,4};
          return a;
        }
			else if(m==x_pos && (length>1 || rd[m+1][y_pos]=='b'))
			{
				vector<int> a{m,0};
				return a;
			}
    }

	//With my current velocity can I move right without hitting something else ?
	bool move_right=true;
	if(y_pos-length>=0) //can move right
    {
      for(int i=x_pos-width+1;i<=x_new_pos;i++)
        {
          for(int j=y_pos+length-2;j>=y_pos-1;j--)
            {
              if(i<rd.size() && i>=0)
                {
                  if(!((i >= x_pos-width+1) && (i<=x_pos) && (j<=y_pos+length-1) && (j>=y_pos)))
                    {
                      if(rd[i][j]!=' ')
                        {
                          move_right=false;
													break;
                        }
                    }
                }
            }
					if(move_right==false)
					{
						break;
					}
        }
    }
	else
    {
      move_right=false;
    }

	if(move_right)
    {
      vector<int> a{x_new_pos,-1};
      return a;
    }
	else
    {
      //With my current velocity, since I can't move right, can I move left without hitting something ?
      bool move_left=true;
      if(y_pos+1<=rd.size()-1)
        {
          for(int i=x_pos-width+1;i<=x_new_pos;i++)
            {
              for(int j=y_pos+1;j<=y_pos+length;j++)
                {
                  if(i<rd.size() && i>=0)
                    {
                      if(!((i >= x_pos-width+1) && (i<=x_pos) && (j<=y_pos+length-1) && (j>=y_pos)))
                        {
                          if(rd[i][j]!=' ')
                            {
                              move_left=false;
															break;
                            }
                        }
                    }
                }
								if(move_left==false)
								{
									break;
								}
            }
        }
      else
        {
          move_left=false;
        }

      if(move_left)
        {
          vector<int> a{x_new_pos,1};
          return a;
        }
      else
        {
          x_new_pos=m;
          //With my reduced velocity can I move right without hitting something else ?
          move_right=true;
          if(y_pos-1>=0) //can move right
            {
              for(int i=x_pos-width+1;i<=x_new_pos;i++)
                {
                  for(int j=y_pos+length-2;j>=y_pos-1;j--)
                    {
                      if(i<rd.size() && i>=0)
                        {
                          if(!((i >= x_pos-width+1) && (i<=x_pos) && (j<=y_pos+length-1) && (j>=y_pos)))
                            {
                              if(rd[i][j]!=' ')
                                {
                                  move_right=false;
																	break;
                                }
                            }
                        }
                    }
										if(move_right==false)
										{
											break;
										}
                }
            }
          else
            {
              move_right=false;
            }

          if(move_right)
            {
              vector<int> a{x_new_pos,-1};
              return a;
            }
          else
            {
              //With my current velocity, since I can't move right, can I move left without hitting something ?
              move_left=true;
              if(y_pos+1<=rd.size()-1) // Can I move left without falling off the road ?
                {
                  for(int i=x_pos-width+1;i<=x_new_pos;i++)
                    {
                      for(int j=y_pos+1;j<=y_pos+length;j++)
                        {
                          if(i<rd.size() && i>=0)
                            {
                              if(!((i >= x_pos-width+1) && (i<=x_pos) && (j<=y_pos+length-1) && (j>=y_pos)))
                                {
                                  if(rd[i][j]!=' ')
                                    {
                                      move_left=false;
																			break;
                                    }
                                }
                            }
                        }
											if(move_left==false)
											{
												break;
											}
                    }
                }
              else
                {
                  move_left=false;
                }

              if(move_left)
                {
                  vector<int> a{x_new_pos,1};
                  return a;
                }
              else
                {
                  //Since I can't move left or right with reduced or normal velocity,
                  // my only option is stop before the bumper of the vehicle.
                  vector<int> a{m,0};
                  return a;
                }
            }
        }
    }
}

vector<Vehicle *> interaction_update(Road *r, vector<Vehicle *> a, vector<int> sig_time)
{
	int road_length=r->get_length();
	int road_width=r->get_width();
	vector<vector<char>> *road_map = &r->road_map;

	vector<Vehicle> list;
	for(int i=0; i<a.size(); i++)
    {
      list.push_back(*a[i]);  //I'm making a copy of all the objects here cause I don't want to disturb the state of all the objects.
    }
	for(int i=0;i<a.size();i++)
    {
      if(a[i]->get_pos()[1]!=-1)  //If my vehicle is on the road, then update.
        {
					a[i]->opengl_diagonal=0;
          Vehicle cur = list[i];
          int signal_distance=r->get_sig_distance();
          int max=cur.get_max_speed();
          int x = cur.get_pos()[0];

          //Under normal circumstances, if the vehicle is not at max speed I want it to keep increasing speed till it reaches max.
          int temp_vx = cur.get_velocity()[0];
          temp_vx=temp_vx+cur.get_acceleration();
          if(temp_vx>max)
            {
              temp_vx=max;
            }
          cur.set_velocity(temp_vx,0);

          vector<int> option = check_movable_zone(cur, *road_map);
          // cout<<option[0]<<" "<<option[1]<<" "<<(char)cur.get_display_char()<<" "<<cur.get_velocity()[0]<<" "<<cur.get_pos()[0]<<" "<<cur.get_pos()[1]<<endl;
          if(option[1] == 5)
            {
              a[i]->set_velocity(cur.get_velocity());
              int px = cur.get_pos()[0]+cur.get_velocity()[0];
              int py = cur.get_pos()[1];
              a[i]->set_pos(px,py);
            }
          else if(option[1] == 4)
            {
              a[i]->set_velocity(0,0);
              int px = option[0];
              int py = cur.get_pos()[1];
              a[i]->set_pos(px,py);
            }
          else if (option[1] == 0)
            {
              a[i]->set_velocity(cur.get_velocity());
              int px = option[0];
              int py = cur.get_pos()[1];
              a[i]->set_pos(px,py);
            }
          else if(option[1] == -1 || option[1] == 1)
            {
							if(a[i]->get_length()==1)
							{
								a[i]->opengl_diagonal=option[1];
							}
              a[i]->set_velocity(cur.get_velocity());
              int px = option[0];
              int py = cur.get_pos()[1]+option[1];
              a[i]->set_pos(px,py);
            }
        }
    }
  r->update(a);
  return a;
}



void draw_cube(float px, float py, float length, float width, float height, string col, int diag=0)
{
	int rgb=stoi(col);
	float col_b = ((1.0)*(rgb%10))/10;
	rgb=rgb/10;
	float col_g = ((1.0)*(rgb%10))/10;
	rgb=rgb/10;
	float col_r = ((1.0)*(rgb%10))/10;

	glLoadIdentity();
	// glTranslatef(-10.0f, -10.0f, -40.0f);
	// glRotatef(45.0f,0.0f,0.0f,1.0f);
	glTranslatef(0.0f, -10.0f, -40.0f);
	glRotatef(-65.0f,1.0f,0.0f,0.0f);
	glRotatef(65.0f,0.0f,0.0f,1.0f);
  glBegin(GL_QUADS);				// start drawing the cube.
	//top of cube
  glColor3f(col_r*1.0f,col_g*1.0f,col_b*1.0f);
  glVertex3f(px*1.0f,(py-(length-1)+diag/5.0f)*1.0f, height*1.0f);		// Top Right Of The Quad (Top)
  glVertex3f(px*1.0f,(py+diag/5.0f)*1.0f, height*1.0f);		// Top Left Of The Quad (Top)
  glVertex3f((px - (width-1))*1.0f,py*1.0f,height*1.0f);		// Bottom Left Of The Quad (Top)
  glVertex3f( (px - (width-1))*1.0f,(py-(length-1))*1.0f,height*1.0f);		// Bottom Right Of The Quad (Top)

  // bottom of cube
  glColor3f(col_r*0.95f,col_g*0.95f,col_b*0.95f);
  glVertex3f(px*1.0f,(py-(length-1)+(diag/5.0f))*1.0f, 0.0f);		// Top Right Of The Quad (Bottom)
  glVertex3f(px*1.0f,(py+(diag/5.0f))*1.0f, 0.0f);		// Top Left Of The Quad (Bottom)
  glVertex3f((px - (width-1))*1.0f,py*1.0f,0.0f);		// Bottom Left Of The Quad (Bottom)
  glVertex3f( (px - (width-1))*1.0f,(py-(length-1))*1.0f,0.0f);		// Bottom Right Of The Quad (Bottom)

  // front of cube
  glColor3f(col_r*0.9f,col_g*0.9f,col_b*0.9f);
  glVertex3f( px*1.0f, (py-(length-1)+(diag/5.0f))*1.0f, height*1.0f);		// Top Right Of The Quad (Front)
  glVertex3f(px*1.0f, (py+(diag/5.0f))*1.0f, height*1.0f);		// Top Left Of The Quad (Front)
  glVertex3f(px*1.0f, (py+(diag/5.0f))*1.0f, 0.0f);		// Top Left Of The Quad (Front)
  glVertex3f( px*1.0f, ((py+diag/5.0f)-(length-1))*1.0f, 0.0f);		// Top Right Of The Quad (Front)

  //back of cube.
  glColor3f(col_r*0.85f,col_g*0.85f,col_b*0.85f);
  glVertex3f( (px - (width-1))*1.0f,(py-(length-1))*1.0f,height*1.0f);		// Top Right Of The Quad (Back)
  glVertex3f((px - (width-1))*1.0f,py*1.0f,height*1.0f);		// Top Left Of The Quad (Back)
  glVertex3f((px - (width-1))*1.0f, py*1.0f, 0.0f);		// Bottom Left Of The Quad (Back)
  glVertex3f( (px - (width-1))*1.0f, (py-(length-1))*1.0f,0.0f);		// Bottom Right Of The Quad (Back)

  // left of cube
  glColor3f(col_r*0.8f,col_g*0.8f,col_b*0.8f);
  glVertex3f(px*1.0f, (py+(diag/5.0f))*1.0f, height*1.0f);		// Top Right Of The Quad (Left)
  glVertex3f((px - (width-1))*1.0f, py*1.0f, height*1.0f);		// Top Left Of The Quad (Left)
  glVertex3f((px - (width-1))*1.0f,py*1.0f,0.0f);		// Bottom Left Of The Quad (Left)
  glVertex3f(px*1.0f,(py+(diag/5.0f))*1.0f, 0.0f);		// Bottom Right Of The Quad (Left)

  // Right of cube
  glColor3f(col_r*0.75f,col_g*0.75f,col_b*0.75f);
  glVertex3f( px*1.0f, (py-(length-1)+(diag/5.0f))*1.0f,height*1.0f);	        // Top Right Of The Quad (Right)
  glVertex3f( (px - (width-1))*1.0f, (py-(length-1))*1.0f, height*1.0f);		// Top Left Of The Quad (Right)
  glVertex3f( (px - (width-1))*1.0f,(py-(length-1))*1.0f, 0.0f);		// Bottom Left Of The Quad (Right)
  glVertex3f( px*1.0f,(py-(length-1)+(diag/5.0f))*1.0f,0.0f);		// Bottom Right Of The Quad (Right)
  glEnd();					// Done Drawing The Cube

}

void draw_cube_bike(float px, float py, float length, float width, float height, string col, int diag=0)
{
	int rgb=stoi(col);
	float col_b = ((1.0)*(rgb%10))/10;
	rgb=rgb/10;
	float col_g = ((1.0)*(rgb%10))/10;
	rgb=rgb/10;
	float col_r = ((1.0)*(rgb%10))/10;

	glLoadIdentity();
	// glTranslatef(-10.0f, -10.0f, -40.0f);
	// glRotatef(45.0f,0.0f,0.0f,1.0f);
	glTranslatef(0.0f, -10.0f, -40.0f);
	glRotatef(-65.0f,1.0f,0.0f,0.0f);
	glRotatef(65.0f,0.0f,0.0f,1.0f);
  glBegin(GL_QUADS);				// start drawing the cube.
	//top of cube
  glColor3f(col_r*1.0f,col_g*1.0f,col_b*1.0f);
  glVertex3f(px*1.0f,(py-(length-0.5f)+diag/5.0f)*1.0f, height*1.0f);		// Top Right Of The Quad (Top)
  glVertex3f(px*1.0f,(py+diag/5.0f)*1.0f, height*1.0f);		// Top Left Of The Quad (Top)
  glVertex3f((px - (width-1))*1.0f,py*1.0f,height*1.0f);		// Bottom Left Of The Quad (Top)
  glVertex3f( (px - (width-1))*1.0f,(py-(length-0.5f))*1.0f,height*1.0f);		// Bottom Right Of The Quad (Top)

  // bottom of cube
  glColor3f(col_r*0.95f,col_g*0.95f,col_b*0.95f);
  glVertex3f(px*1.0f,(py-(length-0.5f)+(diag/5.0f))*1.0f, 0.0f);		// Top Right Of The Quad (Bottom)
  glVertex3f(px*1.0f,(py+(diag/5.0f))*1.0f, 0.0f);		// Top Left Of The Quad (Bottom)
  glVertex3f((px - (width-1))*1.0f,py*1.0f,0.0f);		// Bottom Left Of The Quad (Bottom)
  glVertex3f( (px - (width-1))*1.0f,(py-(length-0.5f))*1.0f,0.0f);		// Bottom Right Of The Quad (Bottom)

  // front of cube
  glColor3f(col_r*0.9f,col_g*0.9f,col_b*0.9f);
  glVertex3f( px*1.0f, (py-(length-0.5f)+(diag/5.0f))*1.0f, height*1.0f);		// Top Right Of The Quad (Front)
  glVertex3f(px*1.0f, (py+(diag/5.0f))*1.0f, height*1.0f);		// Top Left Of The Quad (Front)
  glVertex3f(px*1.0f, (py+(diag/5.0f))*1.0f, 0.0f);		// Top Left Of The Quad (Front)
  glVertex3f( px*1.0f, ((py+diag/5.0f)-(length-0.5f))*1.0f, 0.0f);		// Top Right Of The Quad (Front)

  //back of cube.
  glColor3f(col_r*0.85f,col_g*0.85f,col_b*0.85f);
  glVertex3f( (px - (width-1))*1.0f,(py-(length-0.5f))*1.0f,height*1.0f);		// Top Right Of The Quad (Back)
  glVertex3f((px - (width-1))*1.0f,py*1.0f,height*1.0f);		// Top Left Of The Quad (Back)
  glVertex3f((px - (width-1))*1.0f, py*1.0f, 0.0f);		// Bottom Left Of The Quad (Back)
  glVertex3f( (px - (width-1))*1.0f, (py-(length-0.5f))*1.0f,0.0f);		// Bottom Right Of The Quad (Back)

  // left of cube
  glColor3f(col_r*0.8f,col_g*0.8f,col_b*0.8f);
  glVertex3f(px*1.0f, (py+(diag/5.0f))*1.0f, height*1.0f);		// Top Right Of The Quad (Left)
  glVertex3f((px - (width-1))*1.0f, py*1.0f, height*1.0f);		// Top Left Of The Quad (Left)
  glVertex3f((px - (width-1))*1.0f,py*1.0f,0.0f);		// Bottom Left Of The Quad (Left)
  glVertex3f(px*1.0f,(py+(diag/5.0f))*1.0f, 0.0f);		// Bottom Right Of The Quad (Left)

  // Right of cube
  glColor3f(col_r*0.75f,col_g*0.75f,col_b*0.75f);
  glVertex3f( px*1.0f, (py-(length-0.5f)+(diag/5.0f))*1.0f,height*1.0f);	        // Top Right Of The Quad (Right)
  glVertex3f( (px - (width-1))*1.0f, (py-(length-0.5f))*1.0f, height*1.0f);		// Top Left Of The Quad (Right)
  glVertex3f( (px - (width-1))*1.0f,(py-(length-0.5f))*1.0f, 0.0f);		// Bottom Left Of The Quad (Right)
  glVertex3f( px*1.0f,(py-(length-0.5f)+(diag/5.0f))*1.0f,0.0f);		// Bottom Right Of The Quad (Right)
  glEnd();					// Done Drawing The Cube

}

/* The number of our GLUT window */
int window;

/* rotation angle for the quadrilateral. */
float rquad = 5.0f;

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)	        // We call this right after our OpenGL window is created.
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);		// This Will Clear The Background Color To Black
  glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
  glDepthFunc(GL_LESS);			        // The Type Of Depth Test To Do
  glEnable(GL_DEPTH_TEST);		        // Enables Depth Testing
  glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();				// Reset The Projection Matrix

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window

  glMatrixMode(GL_MODELVIEW);
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height)
{
  if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
    Height=1;

  glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);
}

/* The main drawing function. */
void DrawGLScene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	// glTranslatef(-10.0f, -10.0f, -40.0f);
	// glRotatef(-45.0f,0.0f,0.0f,1.0f);
	// glRotatef(45.0f,0.0f,0.0f,1.0f);
	glTranslatef(0.0f, -10.0f, -40.0f);
	glRotatef(-65.0f,1.0f,0.0f,0.0f);
	glRotatef(65.0f,0.0f,0.0f,1.0f);
	glBegin(GL_QUADS);
  glColor3f(0.2f , 0.2f, 0.2f);
  glVertex3f(0.0f,-1.0f,0.0f);
  glVertex3f((r->get_width())*1.0f,-1.0f, 0.0f);
  glVertex3f((r->get_width())*1.0f,(r->get_length())*1.0f -1.0f, 0.0f);
  glVertex3f( 0.0f,(r->get_length())*1.0f -1.0f,0.0f);
	glEnd();
	int signal_dist=r->get_sig_distance();
  glBegin(GL_QUADS);
  if(r->get_signal_color()==0)
		{
			glColor3f(1.0f , 0.3f, 0.3f);
		}
  else
		{
			glColor3f(0.5f , 1.0f, 0.5f);
		}
  glVertex3f(signal_dist*1.0f,-2.0f,0.0f);
  glVertex3f(signal_dist*1.0f,-1.0f,0.0f);
  glVertex3f(signal_dist*1.0f,-1.0f,3.0f);
  glVertex3f(signal_dist*1.0f,-2.0f,3.0f);
  glEnd();

	usleep(300000);//sleep for 0.25 second

	////////////To make vehicles enter
	int max_no_vehicles;
	int max_width=0;

	srand(time(0));
	int rand_var = (rand()%4)+2;
	int road_length=r->get_length();
	int road_width=r->get_width();
	vector<vector<char>> *road_map = &r->road_map;

	if(enter>rand_var)
	{

		for(int i=0; i<vehicles.size(); i++)
			{
				if(max_width<vehicles[i]->get_length())
					{
						max_width=vehicles[i]->get_length();
					}
			}
		max_no_vehicles=r->get_length()/max_width; //What is the max no. of vehicles I can put on the road ?

		bool is_start_line_empty=true;
		for(int i=0;i<road_length;i++)
			{
				if((*road_map)[0][i]!=' ')
					{
						is_start_line_empty=false;
						break;
					}
			}
		if(is_start_line_empty && vehicle_counter>=0)
			{
				int start_point=road_length-1-vehicles[vehicle_counter]->get_length()+1;
				while(vehicle_counter>=0)
					{
						vehicles[vehicle_counter]->set_pos(0,start_point);
						if(start_point<0)
							{
								vehicles[vehicle_counter]->set_pos(-1,-1);
								break;
							}

						start_point=start_point - 1 - (3-(rand()%3));
						vehicle_counter--;
						road_empty=false;
					}
			}
		r->update(vehicles);
		enter=0;
	}
	else
	{
		enter++;
	}
	//////////////////

	if(!road_empty)
    {
      road_empty=true;
      vector<Vehicle *> a = interaction_update(r,vehicles,sig_time); //redundant. a is not necessary. but kept for readability
      r->display();
      for(int i=0;i<r->get_width();i++)
        {
          for(int j=0;j<r->get_length();j++)
            {
              if((r->road_map)[i][j]!=' ' && (r->road_map)[i][j]!='|')
                {
                  road_empty=false;
                  break;
                }
            }
        }
      time_step++;
      cout<<"Time step = "<<time_step<<endl<<endl;
      if(time_step==sig_time[sig_time_counter] && sig_time_counter<sig_time.size())
        {
          r->set_sig_colour(1-r->get_signal_color());
          sig_time_counter++;
        }

      for(int i=0;i<a.size();i++)
        {
          int px = a[i]->get_pos()[0];
          int py = a[i]->get_pos()[1];
          int length = a[i]->get_length();
          int width = a[i]->get_width();
          int height = a[i]->get_height();
          string col=a[i]->get_color();
          if(px>=0 && px<r->get_width() && length>1)
            {
							draw_cube(px,py,length,width,height,col,a[i]->opengl_diagonal);
            }
          else if(px>=0 && px<r->get_width() && length==1)
          {
							draw_cube_bike(px,py,length,width,height,col,a[i]->opengl_diagonal);
          }
        }
    }
	glutSwapBuffers();
}

/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y)
{
  /* avoid thrashing this call */
  usleep(100);

  /* If escape is pressed, kill everything. */
  if (key == ESCAPE)
    {
      /* shut down our window */
      glutDestroyWindow(window);

      /* exit the program...normal termination. */
      exit(0);
    }
}

int main(int argc, char **argv)
{
  //////////Parsing
	string config = "XML_config.xml";
	vector < vector < string> > ans;
	cout << "Start" << endl;
	xml_document<> doc;
	xml_node<> * root_node;
	// Read the xml file into a vector
	ifstream theFile (config);
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc
	doc.parse<0>(&buffer[0]);
	// Find our root node
	root_node = doc.first_node("road");

	int width = stoi(root_node->first_attribute("length")->value());
	int length = stoi(root_node->first_attribute("width")->value());
	int dist = stoi(root_node->first_attribute("sig_dist")->value());

	Road rd(length,width,dist);
	r=&rd;

	//sees the initial colour for the signal;
	string initial_color = ((root_node->first_node("signals"))->first_attribute("initial_color")->value());
	int t_signal; //0 for red, 1 for green

	if (initial_color.compare("green")==0)
    {
      t_signal = 1;
    }
	else
    {
      t_signal = 0;
    }

	r->set_sig_colour(t_signal);

	//to make a vector which stores the time after which you need to change the signal, taken care of last signal not being red
	xml_node<> * signals_node = root_node->first_node("signals");
	for (xml_node<> * signal_node = signals_node->first_node("signal"); signal_node; signal_node = signal_node->next_sibling())
    {
      sig_time.push_back(stoi(signal_node->first_attribute("time")->value()));
    }
	if ((sig_time.size() % 2 == 0 and t_signal == 1) or (sig_time.size() % 2 == 1 and t_signal == 0))
    {
      sig_time.push_back(1);
    }

	vector<int> pos(2,-1);

	//to make a vector of vehicles to be sent on road
	vector<Vehicle > sig_vehicles;
	xml_node<> * vehicles_node = root_node->first_node("Vehicles");
	for (xml_node<> * vehicle_node = vehicles_node->first_node("vehicle"); vehicle_node; vehicle_node = vehicle_node->next_sibling())
    {
      int wid = stoi(vehicle_node->first_node("length")->value());
      int len = stoi(vehicle_node->first_node("width")->value());
      int height = stoi(vehicle_node->first_node("height")->value());
      string col = vehicle_node->first_node("color")->value();
      int max_v = stoi(vehicle_node->first_node("max_v")->value());
      int acc = stoi(vehicle_node->first_node("acc")->value());
      char disp = vehicle_node->first_node("display")->value()[0];
      int id = sig_vehicles.size();

      Vehicle v(r, len, wid, height, col, max_v, acc, disp, id, pos);
      sig_vehicles.push_back(v);
    }
	for(int i=0;i<sig_vehicles.size();i++)
    {
	    vehicles.push_back(&sig_vehicles[i]);
    }

	road_empty=false;
	time_step=0;
	sig_time_counter=0;
	vehicle_counter=vehicles.size()-1;
  ////////////OpenGL
  /* Initialize GLUT state - glut will take any command line arguments that pertain to it or
     X Windows - look at its documentation at http://reality.sgi.com/mjk/spec3/spec3.html */
  glutInit(&argc, argv);

  /* Select type of Display mode:
     Double buffer
     RGBA color
     Alpha components supported
     Depth buffered for automatic clipping */
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

  /* get a 640 x 480 window */
  glutInitWindowSize(640, 480);

  /* the window starts at the upper left corner of the screen */
  glutInitWindowPosition(0, 0);

  /* Open a window */
  window = glutCreateWindow("Traffic Simulator");

  /* Register the function to do all our OpenGL drawing. */
  glutDisplayFunc(&DrawGLScene);

  /* Go fullscreen.  This is as soon as possible. */
  glutFullScreen();

  /* Even if there are no events, redraw our gl scene. */
  glutIdleFunc(&DrawGLScene);

  /* Register the function called when our window is resized. */
  glutReshapeFunc(&ReSizeGLScene);

  /* Register the function called when the keyboard is pressed. */
  glutKeyboardFunc(&keyPressed);

  /* Initialize our window. */
  InitGL(640, 480);

  /* Start Event Processing Engine */
  glutMainLoop();

  return 1;
}
