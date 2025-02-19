/**
* \class Room
*
* \brief Declaration of Room interface. This class is basically a container of walls
* \date	July 2021
*
* \authors F. Arebola-P�rez and A. Reyes-Lecuona, members of the 3DI-DIANA Research Group (University of Malaga)
* \b Contact: A. Reyes-Lecuona as head of 3DI-DIANA Research Group (University of Malaga): areyes@uma.es
*
* \b Contributions: (additional authors/contributors can be added here)
*
* \b Project: SAVLab (Spatial Audio Virtual Laboratory) ||
* \b Website:
*
* \b Copyright: University of Malaga - 2021
*
* \b Licence: GPLv3
*
* \b Acknowledgement: This project has received funding from Spanish Ministerio de Ciencia e Innovaci�n under the SAVLab project (PID2019-107854GB-I00)
*
*/#pragma once
#include <vector>
#include "Wall.h"

class Room
{
public:
	////////////
	// Methods
	////////////

	/** \brief Initializes the object with a shoebox room
	*	\details creates six walls conforming a shoebox room with 0,0,0 at the center. It must be used right after 
				 creating the empty object.
	*	\param [in] width: extension of the room along the Y axis.
	*	\param [in] length: extension of the room along the X axis.
	*	\param [in] height: extension of the room along the Z axis
	*/
	void setup(float width, float length, float height);

	/** \brief insert a new wall in the room
	*	\details Instead of using the setup method, this method can be used to create any arbitrary room. It should be 
				 called once per wall to be inserted, after creating a new empty room.
	*/
	void insertWall(Wall newWall);

	/** \brief Returns a vector of walls containing all the walls of the room.
	*	\param [out] Walls: vector of walls with all the walls of the room.
	*/
	std::vector<Wall> getWalls();

	/** \brief Returns a vector of image rooms
	*	\details creates an image (specular) room for each wall of this room and returns a vector contoining them.
	*	\param [out] ImageRooms: vector containing all image rooms of this room.
	*/
	std::vector<Room> getImageRooms();

	/** \brief Draws the room.
	*	\details draws the room with a line for each edge of each wall and the normal vector to each wall in their centers.
	*/
	void draw();

private:
	////////////
	// Attributes
	////////////

	std::vector<Wall> walls;            //Vector with all the walls of the room
};

