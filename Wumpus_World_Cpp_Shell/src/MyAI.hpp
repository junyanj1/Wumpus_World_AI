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
#include <deque>
#include <set>
#include <algorithm>
using namespace std;
//

typedef pair<int,int> Position;
typedef vector<bool> Percept;


struct Tile
{
	bool pit    = false;
	bool wumpus = false;
	bool glitter  = false;

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

	int current_time = 0;
	deque<Position> safe;
	deque<Position> unsafe;
    deque<deque<Position>> not_unsafe;

    Position Wumpus;

    int bump_X = 10;
    int bump_Y = 10;
    int current_X = 0;
    int current_Y = 0;
	Action current_action;
    int current_Dir = 0;
    bool have_arrow = true;
    bool got_gold = false;

    //store the expanded tiles of the selected tile
    map<Position,vector<Position>> tiles_expansion;
    void store_tile_expansion(int current_X, int current_Y);


	//a sequence of actions
	deque<Action> plan;


	//knowledge base
    vector<vector<Tile> > KB;

    vector<vector<Position>> KB_;


	//Tell() update KB after getting percepts
	void update_percept(bool stench,bool breeze);
	void update_safe();
	void update_action(Action current_action, int t);

	//query KB for certain information
	void Ask(){}

	//generate an action sequence
	vector<Position> route_generator(Position current,deque<Position> goals, deque<Position> allowed_tiles );
    deque<Action> actions_generator(vector<Position> route);



private:




	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
};

#endif