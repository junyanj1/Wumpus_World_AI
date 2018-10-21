// ======================================================================
// FILE:        MyAI.hpp
//
// AUTHOR:      Abdullah Younis
//
// DESCRIPTION: This file contains your agent class, which you will
//              implement. You are responsible for implementing the
//              'getAction' function and any helper methods you feel you
//              need.
//
// NOTES:       - If you are having trouble understanding how the shell
//                works, look at the other parts of the code, as well as
//                the documentation.
//
//              - You are only allowed to make changes to this portion of
//                the code. Any changes to other portions of the code will
//                be lost when the tournament runs your code.
// ======================================================================

#ifndef MYAI_LOCK
#define MYAI_LOCK

#include "Agent.hpp"

//include other libraries
#include <vector>
#include <iostream>
#include <map>
using namespace std;
//

typedef pair<int,int> Position;
typedef vector<bool> Percept;


struct Tile
{
	bool pit    = false;
	bool wumpus = false;
	bool gold   = false;

	bool breeze = false;
	bool stench = false;

	bool visited = false;
};


class MyAI :
		public Agent
{


public:
	MyAI ( void );

	Action getAction
			(
					bool stench,
					bool breeze,
					bool glitter,
					bool bump,
					bool scream
			);

	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================


	int time;

	vector<Position> safe_pos;
	vector<Position> unsafe_pos;
	vector<Action> plan;


	//knowledge base
	vector<vector<Tile>> KB;


	//update KB after getting percepts
	void Tell(){}
	//query KB for certain information
	void Ask(){}
	//generate an action sequence
	vector<Action> route_generator(Position current,Position goal, vector<Position> allowed_pos ){}

private:
	vector<int,int> current_location;
	bool have_arrow;

	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
};

#endif