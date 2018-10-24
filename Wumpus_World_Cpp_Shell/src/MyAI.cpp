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
        //store current(X,Y) and its expandable tiles
        tiles_position.push_back(Position(current_X,current_Y));
        store_tile_expansion(current_X,current_Y);
    }
    else if(KB[current_X].size()  < current_Y)   {
        Tile t;
        for(int i=0;i<current_Y-KB[current_X].size()-1;++i) {
            KB[current_X].push_back(t);
        }
        KB[current_X].push_back(temp_tile);
        //store current(X,Y) and its expandable tiles
        tiles_position.push_back(Position(current_X,current_Y));
        store_tile_expansion(current_X,current_Y);
    }
}

void MyAI::store_tile_expansion(int current_X, int current_Y) {
    vector<Position> expandable_tiles;
    //left: current_X-1,current_Y
    if( 0 <= current_X-1 && current_X-1 <= KB.size()-1){
        if(0 <= current_Y && current_Y <= KB[current_X-1].size()-1){
            //the two if statements determine there is a tile on the left side
            expandable_tiles.push_back(Position(current_X-1,current_Y));
        }
    }
    //right: current_X+1,current_Y
    if( 0 <= current_X+1 && current_X+1 <= KB.size()-1){
        if(0 <= current_Y && current_Y <= KB[current_X+1].size()-1){
            //the two if statements determine there is a tile on the right side
            expandable_tiles.push_back(Position(current_X+1,current_Y));
        }
    }
    //Top: current_X,current_Y+1
    if( 0 <= current_X && current_X <= KB.size()-1){
        if(0 <= current_Y+1 && current_Y+1 <= KB[current_X].size()-1){
            //the two if statements determine there is a tile on the top side
            expandable_tiles.push_back(Position(current_X,current_Y+1));
        }
    }
    //Bottom: current_X,current_Y-1
    if( 0 <= current_X && current_X <= KB.size()-1){
        if(0 <= current_Y-1 && current_Y-1 <= KB[current_X].size()-1){
            //the two if statements determine there is a tile on the bottom side
            expandable_tiles.push_back(Position(current_X,current_Y-1));
        }
    }

    //add to the tiles_expansion
    tiles_expansion[Position(current_X,current_Y)] = expandable_tiles;
}

void MyAI::update_safe(pair<int,int> position ){
    //to implement
}

void MyAI::update_action(Agent::Action current_action,int time){
    current_time = time;
    switch (current_action){
        case Agent:: FORWARD:
            //right
            if(current_Dir == 0)
                current_X++;
            else if(current_Dir == 1)
                current_Y++;
            else if(current_Dir == 2)
                current_X --;
            else if(current_Dir == 3)
                current_Y --;
            break;
        case Agent:: TURN_LEFT:
            if(current_Dir != 3)
                current_Dir ++;
            else
                current_Dir = 0;
            break;
        case Agent:: TURN_RIGHT:
            if(current_Dir != 0)
                current_Dir --;
            else
                current_Dir = 3;
            break;
        case Agent:: SHOOT:
            have_arrow = false;
            break;
        case Agent:: GRAB:
            got_gold = true;
            break;
        case Agent:: CLIMB:
            break;


    }
}
vector<Position> MyAI::route_generator(Position current,Position goal, deque<Position> allowed_pos ){
    deque<Agent::Action> result;


    map<Position, vector<Position>> all_solutions;

    all_solutions.insert(all_solutions.cbegin(),pair<Position, vector<Position>>(current, vector<Position>()));

    vector<Position> tiles_to_explore;
    vector<Position> next_tiles_to_explore;
    tiles_to_explore.push_back(current);


    vector<Position> path_found;

    //BFS
    while(path_found.empty()) {
        for (int k = 0; k < tiles_to_explore.size(); ++k) {
            Position temp_pos = tiles_to_explore[k];
            for (int i = 0; i < tiles_expansion[temp_pos].size(); ++i) {
                if (!all_solutions.count(tiles_expansion[temp_pos][i])) {
                    //all solutions does not contain the position [temp_pos][i], so add it, then copy the path of temp_pos
                    //make sure tiles_expansion[temp_pos] do not expand back
                    next_tiles_to_explore.push_back(tiles_expansion[temp_pos][i]);
                    vector<Position> temp;
                    if (!all_solutions[temp_pos].empty()) {
                        temp.push_back(temp_pos);
                        for (int j = 0; j < all_solutions[temp_pos].size(); ++j) {
                            temp.push_back(all_solutions[temp_pos][j]);
                        }
                    }
                    else{
                        temp.push_back(temp_pos);
                    }
                    all_solutions[tiles_expansion[temp_pos][i]] = temp;
                    //check if goal state found
                    if (tiles_expansion[temp_pos][i] == goal) {
                        path_found = all_solutions[tiles_expansion[temp_pos][i]];
                    }

                }
            }
        }
        tiles_to_explore = next_tiles_to_explore;
        next_tiles_to_explore.clear();
    }

    //debug path_found
    for(int i=0;i<path_found.size();++i){
        cout << "[" <<path_found[i].first << "," << path_found[i].second << "]" << endl;
    }

    return path_found;

}

deque<Agent::Action> MyAI::actions_generator(vector<Position> route) {
    //To implement

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
        vector<Position> route ;
        deque<Action> actions;
        route = route_generator(pair<int,int>(current_X,current_Y),pair<int,int>(0,0),allowed_tiles);
        actions = actions_generator(route);


        plan.insert(plan.cend(),actions.begin(),actions.end());

        plan.push_back(CLIMB);
	}

    ////////////////////////////////////////////////FOR DEBUG///////////////////////////////////////////////////
//    pair<int,int> start = pair<int,int>(0,0);
//    pair<int,int> end = pair<int,int>(2,3);
//    KB.clear();
//    for(int i=0; i<5; ++i) {
//        vector<Tile> temp;
//        KB.push_back(temp);
//        for(int k=0; k<5; ++k) {
//            Tile temp;
//            KB[i].push_back(temp);
//        }
//    }
//
//    tiles_position = vector<Position>{
//            pair<int,int>(0,0),pair<int,int>(0,1),pair<int,int>(0,2),pair<int,int>(0,3),pair<int,int>(0,4),
//            pair<int,int>(1,0),pair<int,int>(1,1),pair<int,int>(1,2),pair<int,int>(1,3),pair<int,int>(1,4),
//            pair<int,int>(2,0),pair<int,int>(2,1),pair<int,int>(2,2),pair<int,int>(2,3),pair<int,int>(2,4),
//            pair<int,int>(3,0),pair<int,int>(3,1),pair<int,int>(3,2),pair<int,int>(3,3),pair<int,int>(3,4),
//            pair<int,int>(4,0),pair<int,int>(4,1),pair<int,int>(4,2),pair<int,int>(4,3),pair<int,int>(4,4),
//    };
//    for(int i=0; i < tiles_position.size(); ++i){
//        store_tile_expansion(tiles_position[i].first,tiles_position[i].second);
//    }
//    //debug store_tile_expansion()
//    for(int i=0; i<5; ++i) {
//        for(int k=0; k<5; ++k) {
//            cout << "[" << i << "," << k << "]" << endl;//tiles_expansion[pair<int, int>(i, k)].size() << endl;
//            for(int j=0;j<tiles_expansion[pair<int, int>(i, k)].size();++j){
//                cout<<tiles_expansion[pair<int, int>(i, k)][j].first << ","<< tiles_expansion[pair<int, int>(i, k)][j].second << endl;
//            }
//
//
//        }
//    }
//    deque<Position> allowed_tiles{
//            pair<int,int>(0,0),pair<int,int>(0,1),pair<int,int>(0,2),pair<int,int>(0,3),pair<int,int>(0,4),
//            pair<int,int>(1,0),pair<int,int>(1,1),pair<int,int>(1,2),pair<int,int>(1,3),pair<int,int>(1,4),
//            pair<int,int>(2,0),pair<int,int>(2,1),pair<int,int>(2,2),pair<int,int>(2,3),pair<int,int>(2,4),
//            pair<int,int>(3,0),pair<int,int>(3,1),pair<int,int>(3,2),pair<int,int>(3,3),pair<int,int>(3,4),
//            pair<int,int>(4,0),pair<int,int>(4,1),pair<int,int>(4,2),pair<int,int>(4,3),pair<int,int>(4,4),
//    };
//    route_generator(start,end,allowed_tiles);

    ////////////////////////////////////////////////FOR DEBUG///////////////////////////////////////////////////


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
    if(!plan.empty()) {
        current_action = plan[0];
        plan.pop_front();
    }

    //update KB after action
    update_action(current_action,current_time);

    //update time after each action
	current_time++;

    //current_action
    return current_action;
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