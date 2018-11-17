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

    //generate a new tile of current position
    //check x coordinate
    if(KB.size() < current_X+1){
        vector<Tile> t;
        t.push_back(temp_tile);
        KB.push_back(t);

    }
    else if(KB[current_X].size()  < current_Y+1)   {
        Tile t;
        for(int i=0;i<current_Y-KB[current_X].size();++i) {
            KB[current_X].push_back(t);
        }
        KB[current_X].push_back(temp_tile);


    }
    else{
        //current position has a tile
        KB[current_X][current_Y].breeze = breeze;
        KB[current_X][current_Y].stench = stench;
    }


}


void MyAI::store_tile_expansion(int current_X, int current_Y) {
    vector<Position> expandable_tiles;
    //left: current_X-1,current_Y
    if( 0 <= current_X-1 && current_X-1 <= KB.size()-1){
        if(0 <= current_Y && current_Y <= KB[current_X-1].size()-1){
            //the two if statements determine there is a tile on the left side
            expandable_tiles.push_back(Position(current_X-1,current_Y));
            //also, this tile itself is a expandable tile of the left tile
            if(tiles_expansion.count(Position(current_X+1,current_Y))){
                tiles_expansion[Position(current_X-1,current_Y)].push_back(Position(current_X,current_Y));
            }
        }
    }
    //right: current_X+1,current_Y
    if( 0 <= current_X+1 && current_X+1 <= KB.size()-1){
        if(0 <= current_Y && current_Y <= KB[current_X+1].size()-1){
            //the two if statements above determine there is a tile on the right side
            expandable_tiles.push_back(Position(current_X+1,current_Y));
            //also, this tile itself is a expandable tile of the right tile
            if(tiles_expansion.count(Position(current_X+1,current_Y))){
                tiles_expansion[Position(current_X+1,current_Y)].push_back(Position(current_X,current_Y));
            }

        }
    }
    //Top: current_X,current_Y+1
    if( 0 <= current_X && current_X <= KB.size()-1){
        if(0 <= current_Y+1 && current_Y+1 <= KB[current_X].size()-1){
            //the two if statements above determine there is a tile on the top side
            expandable_tiles.push_back(Position(current_X,current_Y+1));
            //also, this tile itself is a expandable tile of the top tile
            if(tiles_expansion.count(Position(current_X+1,current_Y))){
                tiles_expansion[Position(current_X,current_Y+1)].push_back(Position(current_X,current_Y));
            }
        }
    }
    //Bottom: current_X,current_Y-1
    if( 0 <= current_X && current_X <= KB.size()-1){
        if(0 <= current_Y-1 && current_Y-1 <= KB[current_X].size()-1){
            //the two if statements above determine there is a tile on the bottom side
            expandable_tiles.push_back(Position(current_X,current_Y-1));
            //also, this tile itself is a expandable tile of the bottom tile
            if(tiles_expansion.count(Position(current_X+1,current_Y))){
                tiles_expansion[Position(current_X,current_Y-1)].push_back(Position(current_X,current_Y));
            }
        }
    }

    //add to the tiles_expansion
    tiles_expansion[Position(current_X,current_Y)] = expandable_tiles;
}

void MyAI::update_safe(){
    //to implement
    Position temp_pos(current_X,current_Y);

    if(find(safe.begin(),safe.end(),temp_pos) == safe.end())
        //check safe does not have current position
        safe.push_back(temp_pos);


    Tile temp_tile = KB[current_X][current_Y];

    //this tiles doesn't have breeze or stench, which means the other tiles around it are also safe
    if(!temp_tile.breeze && !temp_tile.stench){

        //create left tile if can exist
        current_X--;
        if(current_X >=0)
            update_percept(true,true);
        //create top tile if can exist
        current_X++;
        current_Y++;
        if(current_Y < bump_Y)
            update_percept(true, true);
        //create right tile if can exist
        current_Y--;
        current_X++;
        if(current_X < bump_X)
            update_percept(true,true);
        //create bottom tile if can exist
        current_X--;
        current_Y--;
        if(current_Y >= 0)
            update_percept(true,true);
        //recover current x,y
        current_Y++;
        store_tile_expansion(current_X,current_Y);

        deque<deque<Position>> new_not_unsafe;
        for(int i=0; i<tiles_expansion[temp_pos].size();++i){
            //avoid duplicates in safe, avoid put unsafe in safe
            if(find(safe.begin(),safe.end(),tiles_expansion[temp_pos][i]) == safe.end() &&
            find(unsafe.begin(),unsafe.end(),tiles_expansion[temp_pos][i]) == unsafe.end())
                safe.push_back(tiles_expansion[temp_pos][i]);
            //

            //if there is safe position in not_unsafe, pop it
            for(int j=0; j < not_unsafe.size(); ++j){
                auto it = find(not_unsafe[j].begin(),not_unsafe[j].end(),tiles_expansion[temp_pos][i]);
                if(it != not_unsafe[j].end() ){
                    not_unsafe[j].erase(it);
                }
                //if there is a list of not_unsafe with size 1, it means the remaining one is unsafe
                if(not_unsafe[j].size() == 1)
                    unsafe.push_back(not_unsafe[j][0]);
                else
                    new_not_unsafe.push_back(not_unsafe[j]);
            }

            not_unsafe = new_not_unsafe;
            new_not_unsafe.clear();


        }

    }
        //this tile has breeze or stench
    else if(temp_tile.breeze || temp_tile.stench){

        //create left tile if can exist
        current_X--;
        if(current_X >=0)
            update_percept(true,true);
        //create top tile if can exist
        current_X++;
        current_Y++;
        if(current_Y < bump_Y)
            update_percept(true, true);
        //create right tile if can exist
        current_Y--;
        current_X++;
        if(current_X < bump_X)
            update_percept(true,true);
        //create bottom tile if can exist
        current_X--;
        current_Y--;
        if(current_Y >= 0)
            update_percept(true,true);
        //recover current x,y
        current_Y++;
        store_tile_expansion(current_X,current_Y);

        //push tiles around into not_unsafe
        deque<Position> temp;
        for(int i=0; i<tiles_expansion[temp_pos].size();++i){
            //check this tile is not already in safe
            if(find(safe.begin(),safe.end(),tiles_expansion[temp_pos][i]) == safe.end())
                temp.push_back(tiles_expansion[temp_pos][i]);
        }
        not_unsafe.push_back(temp);



    }
    else {
        //store all expandable tiles around the tile of current
        store_tile_expansion(current_X,current_Y);
    }



}

void MyAI::update_action(Agent::Action current_action,int time){
    current_time = time;
    KB[current_X][current_Y].visited = true;
    switch (current_action){
        case Agent:: FORWARD:
            if(current_Dir == 0)
                current_X++;
            else if(current_Dir == 1)
                current_Y++;
            else if(current_Dir == 2)
                current_X--;
            else if(current_Dir == 3)
                current_Y--;
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

vector<Position> MyAI::route_generator(Position current,deque<Position> goals, deque<Position> allowed_pos ){
    deque<Agent::Action> result;


    map<Position, vector<Position>> all_solutions;

    all_solutions.insert(all_solutions.cbegin(),pair<Position, vector<Position>>(current, vector<Position>()));

    vector<Position> tiles_to_explore;
    vector<Position> next_tiles_to_explore;
    tiles_to_explore.push_back(current);

    Position goal;
    vector<Position> path_found;
    bool goal_found = false;

    //current position is already the goal position
    if(find(goals.begin(),goals.end(),current)!= goals.end()){
        goal_found = true;
    }


    //BFS
    while(!goal_found) {
        for (int k = 0; k < tiles_to_explore.size(); ++k) {
            Position temp_pos = tiles_to_explore[k];
            for (int i = 0; i < tiles_expansion[temp_pos].size(); ++i) {
                //all_solution does not have the key, and the tile of temp_pos to expand is in allowed_pos
                if (!all_solutions.count(tiles_expansion[temp_pos][i])
                    && find(allowed_pos.begin(),allowed_pos.end(),tiles_expansion[temp_pos][i]) != allowed_pos.end()) {
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
                    if (find(goals.begin(),goals.end(),tiles_expansion[temp_pos][i]) != goals.end() ) {
                        goal = tiles_expansion[temp_pos][i];
                        path_found = all_solutions[tiles_expansion[temp_pos][i]];
                        goal_found = true;
                    }

                }
            }
        }
        tiles_to_explore = next_tiles_to_explore;
        next_tiles_to_explore.clear();
    }

    //format path_found
    //path_found.pop_back();
    path_found.insert(path_found.begin(),goal);

    //debug path_found
//    for(int i=0;i<path_found.size();++i){
//        cout << "[" <<path_found[i].first << "," << path_found[i].second << "]" << endl;
//    }

    return path_found;

}

deque<Agent::Action> MyAI::actions_generator(vector<Position> route) {
    deque<Action> actions;
    vector<Position> route_get = route;
    int temp_x = route_get[route_get.size() - 1].first;
    int temp_y = route_get[route_get.size() - 1].second;
    int temp_dir = current_Dir;
    route_get.pop_back();


    while (!route_get.empty()) {
        int target_dir;
        Position next_tile = route_get[route_get.size() - 1];
        route_get.pop_back();

        if (next_tile.first > temp_x)
            target_dir = 0;
        else if (next_tile.first < temp_x)
            target_dir = 2;
        else if (next_tile.second > temp_y)
            target_dir = 1;
        else if (next_tile.second < temp_y)
            target_dir = 3;
        else{
            //current tile is next tile, which means current position is goal
            return actions;
        }

        //turn right, and move forward
        if (temp_dir - target_dir == 1 || temp_dir - target_dir == -3) {
            actions.push_back(TURN_RIGHT);
            actions.push_back(FORWARD);
        }
        //turn left, and move forward
        else if (temp_dir - target_dir == -1 || temp_dir - target_dir == 3) {
            actions.push_back(TURN_LEFT);
            actions.push_back(FORWARD);
        }
        //turn back, and move forward
        else if (temp_dir - target_dir == -2 || temp_dir - target_dir == 2){
            actions.push_back(TURN_LEFT);
            actions.push_back(TURN_LEFT);
            actions.push_back(FORWARD);
        }
        //same dir, move forward
        else{
            actions.push_back(FORWARD);
        }

        temp_x = next_tile.first;
        temp_y = next_tile.second;
        temp_dir = target_dir;

    }
    return actions;

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

    if(!bump) {
        //update percepts of current tile
        update_percept(stench, breeze);
        //update all safe tiles after last action
        update_safe();
    }


    /////////////for minimal AI
    if(plan.empty()) {
        //found gold?
        if(glitter){
            plan.push_back(GRAB);

            //generate a sequence of actions to go back
            vector<Position> route ;
            deque<Action> actions;
            deque<Position> goals;
            deque<Position> allowed_tiles;

            allowed_tiles = safe;
            goals.push_back(pair<int,int>(0,0));


            route = route_generator(pair<int,int>(current_X,current_Y),goals,allowed_tiles);
            if(!route.empty()) {
                actions = actions_generator(route);
                if(!actions.empty())
                    plan.insert(plan.cend(), actions.begin(), actions.end());
            }

            plan.push_back(CLIMB);
        }
        else if (bump) {
            //erase current position(in the wall) from safe

//            debug
//            cout << "!!!!!!!!!!!!!!!";


            deque<Position>::iterator it = find(safe.begin(), safe.end(), Position(current_X, current_Y));
            if(it != safe.end()) {
                safe.erase(it);
                unsafe.push_back(Position(current_X, current_Y));
//                cout << "bump_pos: " << current_X << "," << current_Y << endl;
            }

            //reverse position
            if (current_Dir == 0){
                bump_X = current_X;
                //KB.pop_back();
                current_X--;
            }
            if (current_Dir == 1) {
                bump_Y = current_Y;
                //KB[current_X].pop_back();
                current_Y--;
            }
            if (current_Dir == 2)
                current_X++;
            if (current_Dir == 3)
                current_Y++;



            //debug
//            cout << "current:" << current_X  << "," << current_Y << endl;

            vector<Position> route;
            deque<Action> actions;
            deque<Position> goals;
            deque<Position> allowed_tiles;

            for (int i = 0; i < safe.size(); i++) {
                //allowed tiles contain all safe tiles
                allowed_tiles.push_back(safe[i]);
                //any safe tile which is not current tile or visited tile can be the goal tile
                if (safe[i] != Position(current_X, current_Y) && !KB[safe[i].first][safe[i].second].visited) {
                    goals.push_back(safe[i]);
                    //debug
//                    cout<<"[" << safe[i].first << "," << safe[i].second <<"]" << endl;
                }
            }

            //go to any unvisited safe tiles
            if(!goals.empty()){
                route = route_generator(pair<int,int>(current_X,current_Y),goals,allowed_tiles);
                if(!route.empty()) {
                    actions = actions_generator(route);
                    if (!actions.empty())
                        plan.insert(plan.cend(), actions.begin(), actions.end());
                }
            }
                //no goal tile, just go home
            else{
                goals.push_back(pair<int,int>(0,0));
                route = route_generator(pair<int,int>(current_X,current_Y),goals,allowed_tiles);
                if(!route.empty()) {
                    actions = actions_generator(route);
                    if(!actions.empty())
                        plan.insert(plan.cend(), actions.begin(), actions.end());
                }
                plan.push_back(CLIMB);

            }





        } else if (breeze || stench) {
            vector<Position> route;
            deque<Action> actions;
            deque<Position> allowed_tiles;
            deque<Position> goals;


            ///////////////////////minimal AI///////////////////
//            allowed_tiles = safe;
//            deque<Position> goals;
//
//
//            goals.push_back(pair<int, int>(0, 0));
//            route = route_generator(pair<int, int>(current_X, current_Y), goals, allowed_tiles);
//
//            if (!route.empty()) {
//                actions = actions_generator(route);
//                if (!actions.empty())
//                    plan.insert(plan.cend(), actions.begin(), actions.end());
//            }
//
//            plan.push_back(CLIMB);
            ///////////////////////minimal AI///////////////////

            for (int i = 0; i < safe.size(); i++) {
                //allowed tiles contain all safe tiles
                allowed_tiles.push_back(safe[i]);
                //any safe tile which is not current tile or visited tile can be the goal tile
                if (safe[i] != Position(current_X, current_Y) && !KB[safe[i].first][safe[i].second].visited) {
                    goals.push_back(safe[i]);
                }
            }

            //if there are no unviisted safe tiles
            if(goals.empty()){
                //go back
                goals.push_back(pair<int,int>(0,0));
                route = route_generator(pair<int,int>(current_X,current_Y),goals,allowed_tiles);
                if(!route.empty()) {
                    actions = actions_generator(route);
                    if(!actions.empty())
                        plan.insert(plan.cend(), actions.begin(), actions.end());
                }
                plan.push_back(CLIMB);
            }
                //go to any saft unvisited tiles
            else{
                route = route_generator(pair<int,int>(current_X,current_Y),goals,allowed_tiles);
                if(!route.empty()) {
                    actions = actions_generator(route);
                    if(!actions.empty())
                        plan.insert(plan.cend(), actions.begin(), actions.end());
                }
            }


        }
        else {
            vector<Position> route;
            deque<Action> actions;
            deque<Position> goals;
            deque<Position> allowed_tiles;

            //debug
//            cout << "current:" << current_X  << "," << current_Y << endl;

            for (int i = 0; i < safe.size(); i++) {
                //allowed tiles contain all safe tiles
                allowed_tiles.push_back(safe[i]);
                //any safe tile which is not current tile or visited tile can be the goal tile
                if (safe[i] != Position(current_X, current_Y) && !KB[safe[i].first][safe[i].second].visited ) {
                    goals.push_back(safe[i]);

                    //debug
//                    cout<<"[" << safe[i].first << "," << safe[i].second <<"]" << endl;
                }
            }

            if(!goals.empty()) {

                //debug
//            for (int i = 0; i < goals.size(); ++i) {
//                cout << "[" << goals[i].first << "," << goals[i].second << "]" << endl;
//            }

                route = route_generator(pair<int, int>(current_X, current_Y), goals, allowed_tiles);

                if (!route.empty())
                    actions = actions_generator(route);

                //debug
//            for (int i = 0; i < actions.size(); ++i) {
//                if (actions[i] == TURN_RIGHT) {
//                    cout << "R" << endl;
//                }
//                if (actions[i] == TURN_LEFT) {
//                    cout << "L" << endl;
//                }
//                if (actions[i] == FORWARD) {
//                    cout << "F" << endl;
//                }
//                if (actions[i] == SHOOT) {
//                    cout << "S" << endl;
//                }
//                if (actions[i] == GRAB) {
//                    cout << "G" << endl;
//                }
//                if (actions[i] == CLIMB) {
//                    cout << "C" << endl;
//                }
//            }

                if (!actions.empty())
                    plan.insert(plan.cend(), actions.begin(), actions.end());
            }
                //no goal, just go home
            else{
                goals.push_back(pair<int,int>(0,0));
                route = route_generator(pair<int,int>(current_X,current_Y),goals,allowed_tiles);
                if(!route.empty()) {
                    actions = actions_generator(route);
                    if(!actions.empty())
                        plan.insert(plan.cend(), actions.begin(), actions.end());
                }
                plan.push_back(CLIMB);
            }

        }
    }

    /////////////



    ////////////////////////////////////////////////FOR DEBUG///////////////////////////////////////////////////
//    pair<int,int> start = pair<int,int>(0,0);
//    deque<Position> end = deque<Position>{Position(1,0),Position(0,1)};
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
////    for(int i=0; i<5; ++i) {
////        for(int k=0; k<5; ++k) {
////            cout << "[" << i << "," << k << "]" << endl;//tiles_expansion[pair<int, int>(i, k)].size() << endl;
////            for(int j=0;j<tiles_expansion[pair<int, int>(i, k)].size();++j){
////                cout<<tiles_expansion[pair<int, int>(i, k)][j].first << ","<< tiles_expansion[pair<int, int>(i, k)][j].second << endl;
////            }
////
////
////        }
////    }
//    deque<Position> allowed_tiles{
//            pair<int,int>(0,0),pair<int,int>(0,1),pair<int,int>(0,2),pair<int,int>(0,3),pair<int,int>(0,4),
//            pair<int,int>(1,0),pair<int,int>(1,1),pair<int,int>(1,2),pair<int,int>(1,3),pair<int,int>(1,4),
//            pair<int,int>(2,0),pair<int,int>(2,1),pair<int,int>(2,2),pair<int,int>(2,3),pair<int,int>(2,4),
//            pair<int,int>(3,0),pair<int,int>(3,1),pair<int,int>(3,2),pair<int,int>(3,3),pair<int,int>(3,4),
//            pair<int,int>(4,0),pair<int,int>(4,1),pair<int,int>(4,2),pair<int,int>(4,3),pair<int,int>(4,4),
//    };
//    vector<Position> route;
//    route = route_generator(start,end,allowed_tiles);
//    deque<Agent::Action> actions;
//    actions = actions_generator(route);
//    for(int i=0;i<actions.size();++i){
//        if(actions[i] == TURN_RIGHT){
//            cout<< "R";
//        }
//        if(actions[i] == TURN_LEFT){
//            cout<< "L";
//        }
//        if(actions[i] == FORWARD){
//            cout<< "F";
//        }
//        if(actions[i] == SHOOT){
//            cout<< "S";
//        }
//        if(actions[i] == GRAB){
//            cout<< "G";
//        }
//        if(actions[i] == CLIMB){
//            cout<< "C";
//        }
//    }

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

    //debug
//    if(current_action == TURN_RIGHT){
//        cout<< "R";
//    }
//    if(current_action == TURN_LEFT){
//        cout<< "L";
//    }
//    if(current_action == FORWARD){
//        cout<< "F";
//    }
//    if(current_action == SHOOT){
//        cout<< "S";
//    }
//    if(current_action == GRAB){
//        cout<< "G";
//    }
//    if(current_action == CLIMB){
//        cout<< "C";
//    }


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