## Pre-requisites:

- OpenGL
- GLUT (for OpenGL)
- g++ compiler
- RapidXML (Required to parse the XML Configuration file)

## Instructions for RapidXML:

- Download the RapidXML distribution to a folder in your computer.
- Copy the path of the "rapidxml.hpp" file.
- Paste the path in line 16 of opengl.cpp where I have provided a sample path.

## Compilation:

- To compile the code, `cd` to the root of the project directory and execute the following command (in the terminal),

        make

## Running the Simulator:

- To specify the number of vehicles, their speed, acceleration, etc. create an XML config file with the name "XML_config.xml" and store it in the root of the project directory. A sample file has already been provided for reference. If you wish to run the default simulation, leave the file unaltered.
- Execute the following binary (in the terminal),

        ./output.out

- The config file is in XML format and the user is allowed to specify the following parameters (see the sample file for reference):
  - Signal initial color (Red or green)
  - Time at which a signal is supposed to change color (Can specify multiple time instants. Eg. 25,30,50. If the signal is initially red, then it will turn green at 25s, red at 30s and green at 50s)
  - Vehicle type (Optional)
  - Vehicle color (RGB format with each value in the range 0-9. Eg. 595 gives light green.)
  - Display character for vehicle in console output.
  - Length, width and height of the vehicle
  - Max speed of the vehicle

## Simulator Features

- Ability to define any kind of vehicle with any length, width, height, and color (provided they fit the road)
- Randomised behaviour.
- Lane changing ability.
- Deceleration to prevent collisions in the event that a lane change is not possible.
- In the event that a collision is not probable, velocity is constantly increased i.e vehicle is accelerated till max speed.
- In the event that a vehicle is halfway through the signal, just like a real life Indian vehicle, it will not stop and will proceed to cross the signal.
- Bikes and auto-rickshaws can overtake even at a red light, so that they can get in front of all the other vehicles. (By bikes and auto-rickshaws, I mean vehicles of width = 1)

## Description:

The simulator has been made in such a way that all the vehicles' behaviour is dictated by a Finite State Machine like algorithm. The vehicles do not look ahead by more than one time-step to make a decision as to whether they should change lanes or decelerate or stop at a signal or run through a red signal. For any given vehicle on the road,

- If there is no vehicle in front of it, it will move ahead with increasing velocity till it reaches max speed
- If there is a vehicle, it will try to see whether a vehicle is there to it's right in its movable zone. If there isn't, it will try to change lanes to the right and overtake. (Higher prefernce is given to right because, in Indian roads, faster vehicles are suppose to overtake by moving to the lane on the right.)
- If it can't change lanes to the right, it will do the same analysis for overtaking on the left.
- If it can't change lanes and move forward to overtake, then it will change lanes to the right or left and stay put, in the hopes of overtaking in the future.
- If there is no scope for overtaking, it will decelerate, just enough to stop right in front of the vehicle.
- If there is a red light and the vehicle hasn't already crossed it, then it will come to a complete stop. (Bikes and auto-rickshaws are allowed to overtake)

The behaviour of the vehicles has been randomised in such a way that, they will enter the road at random intervals of 1-3 s and they will enter at different positions

In the videos, provided in the submission, the vehicles have the following properties:

- b = Bikes => Length = 2, Width = 1, Max Speed = 3, Acceleration = 1
- C = Car => Length = 2, Width = 2, Max Speed = 2, Acceleration = 1
- B = Bus => Length = 3, Width = 2, Max Speed = 1, Acceleration = 1
- T = Truck => Length = 4, Width = 2, Max Speed = 1, Acceleration = 1
