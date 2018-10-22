// ======================================================================
// FILE:        MyAI.cpp
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

#include "MyAI.hpp"

MyAI::MyAI() : Agent()
{
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================

	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
}

void MyAI::update_percept(bool stench,bool breeze){
    Tile temp_tile;
    temp_tile.stench = stench;
    temp_tile.breeze = breeze;
    temp_tile.visited = true;

    //check x coordinate
    if(KB.size() == 0 || KB.size()< current_X){
        vector<Tile> t;
        t.push_back(temp_tile);
        KB.push_back(t);
    }
    else if(KB[current_X].size() < current_Y)   {
        Tile t;
        for(int i=0;i<current_Y-KB[current_X].size()-1;++i) {
            KB[current_X].push_back(t);
        }
        KB[current_X].push_back(temp_tile);

    }
}

void MyAI::update_safe(pair<int,int> position ){
    //to implement
}

void MyAI::update_action(Agent::Action current_action,int time){
    //to implement
}
deque<Agent::Action> MyAI::route_generator(Position current,Position goal, deque<Tile> allowed_pos ){
    //to implement
}

Agent::Action MyAI::getAction
		(
				bool stench,
				bool breeze,
				bool glitter,
				bool bump,
				bool scream
		)
{
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================

    //update percepts of current tile
    update_percept(stench,breeze);
    //update all safe tiles after last action
    update_safe((pair<int,int>(current_X,current_Y)));

    //found gold?
	if(glitter){
		plan.push_back(GRAB);

        //generate a sequence of actions to go back
        deque<Action> route ;
        route = route_generator(pair<int,int>(current_X,current_Y),pair<int,int>(0,0),allowed_tiles);
        plan.insert(plan.cend(),route.begin(),route.end());

        plan.push_back(CLIMB);
	}
    //found a route to unvisited & safe tiles?
    if(plan.empty()){

    }

    //try to shoot the arrow if no unvisited & safe tiles
    if(plan.empty()){

    }

    //no choice but to take a risk to go to a tile unvisited & not unsafe
    if(plan.empty()){

    }

    //run out of choices, so just go back and climb
    if(plan.empty()){

    }

    //pop action
//    current_action = plan[0];
//    plan.pop_front();

    //update KB after action
//    update_action(current_action,t);

    //update time after each action
	//t++;

    //current_action
    return CLIMB;
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
}

// ======================================================================
// YOUR CODE BEGINS
// ======================================================================


// ======================================================================
// YOUR CODE ENDS
// ======================================================================