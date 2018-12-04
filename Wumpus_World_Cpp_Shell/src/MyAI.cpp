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

    if(KB.size()<=0){
        for(int i=0;i<bump_X;++i){
            vector<Tile>t;
            for(int j=0;j<bump_Y;++j){
                t.push_back(temp_tile);
            }
            KB.push_back(t);
        }

    }

//    if(KB.size() < current_X+1){
//        vector<Tile> t;
//        temp_tile.stench = stench;
//        temp_tile.breeze = breeze;
//        t.push_back(temp_tile);
//        KB.push_back(t);
//
//    }
//    else if(KB[current_X].size()  < current_Y+1)   {
//        Tile t;
//        t.stench = stench;
//        t.breeze = breeze;
//        for(int i=0;i<current_Y+1 - KB[current_X].size();++i) {
//            KB[current_X].push_back(t);
//        }
//        //KB[current_X].push_back(temp_tile);
//
//
//    }
//    else{
//        //current position has a tile
//        KB[current_X][current_Y].breeze = breeze;
//        KB[current_X][current_Y].stench = stench;
//    }


}


void MyAI::store_tile_expansion(int current_X, int current_Y) {
    vector<Position> expandable_tiles;
    //left: current_X-1,current_Y
    if (0 <= current_X - 1 && current_X - 1 <= KB.size() - 1) {
        if (0 <= current_Y && current_Y <= KB[current_X - 1].size() - 1) {
            expandable_tiles.push_back(Position(current_X - 1, current_Y));
        }
    }
    //right: current_X+1,current_Y
    if (0 <= current_X + 1 && current_X + 1 <= KB.size() - 1) {
        if (0 <= current_Y && current_Y <= KB[current_X + 1].size() - 1) {
            expandable_tiles.push_back(Position(current_X + 1, current_Y));
        }
    }
    //Top: current_X,current_Y+1
    if (0 <= current_X && current_X <= KB.size() - 1) {
        if (0 <= current_Y + 1 && current_Y + 1 <= KB[current_X].size() - 1) {
            expandable_tiles.push_back(Position(current_X, current_Y + 1));
        }
    }
    //Bottom: current_X,current_Y-1
    if (0 <= current_X && current_X <= KB.size() - 1) {
        if (0 <= current_Y - 1 && current_Y - 1 <= KB[current_X].size() - 1) {
            expandable_tiles.push_back(Position(current_X, current_Y - 1));
        }

    }
    //add to the tiles_expansion
    tiles_expansion[Position(current_X, current_Y)] = expandable_tiles;

}

void MyAI::update_safe(bool stench,bool breeze){
    //to implement
    Position temp_pos(current_X,current_Y);

    if(find(safe.begin(),safe.end(),temp_pos) == safe.end())
        //check safe does not have current position
        safe.push_back(temp_pos);


    Tile temp_tile = KB[current_X][current_Y];
    temp_tile.breeze = breeze;
    temp_tile.stench = stench;

    //create left tile if can exist
    current_X--;
    if (current_X >= 0)
        update_percept(true, true);
    //create top tile if can exist
    current_X++;
    current_Y++;
    if (current_Y < bump_Y)
        update_percept(true, true);
    //create right tile if can exist
    current_Y--;
    current_X++;
    if (current_X < bump_X)
        update_percept(true, true);
    //create bottom tile if can exist
    current_X--;
    current_Y--;
    if (current_Y >= 0)
        update_percept(true, true);
    //recover current x,y
    current_Y++;
    store_tile_expansion(current_X, current_Y);

    //this tiles doesn't have breeze or stench, which means the other tiles around it are also safe
    if(!temp_tile.breeze && !temp_tile.stench){
        //update safe
        for(int i=0; i<tiles_expansion[temp_pos].size();++i) {
            //avoid duplicates in safe, avoid putting unsafe in safe
            if (find(safe.begin(), safe.end(), tiles_expansion[temp_pos][i]) == safe.end()
                &&find(unsafe.begin(),unsafe.end(),tiles_expansion[temp_pos][i]) == unsafe.end())
                safe.push_back(tiles_expansion[temp_pos][i]);
        }

        //update not_unsafe
        //if there is safe position in not_unsafe, pop it
        deque<deque<Position>> new_not_unsafe;
        vector<Position> safe_pos = tiles_expansion[temp_pos];
        for(int i = 0; i < not_unsafe.size(); ++i) {
            for(int j = 0; j<safe_pos.size();++j){
                deque<Position>::iterator it = find(not_unsafe[i].begin(),not_unsafe[i].end(),safe_pos[j]);
                if(it != not_unsafe[i].end())
                    not_unsafe[i].erase(it);
            }

            //update unsafe
            if(not_unsafe[i].size() == 1){
                if(find(unsafe.begin(),unsafe.end(),not_unsafe[i][0]) == unsafe.end())
                    unsafe.push_back(not_unsafe[i][0]);
            }
            else
                new_not_unsafe.push_back(not_unsafe[i]);
        }
        not_unsafe = new_not_unsafe;

        //update possible wumpus
        for(int j = 0; j<safe_pos.size();++j){
            vector<Position>::iterator it = find(possible_wumpus.begin(),possible_wumpus.end(),safe_pos[j]);
            if(it != possible_wumpus.end())
                possible_wumpus.erase(it);
        }



    }
        //this tile has breeze or stench
    else if(temp_tile.breeze || temp_tile.stench) {


        deque<Position> union_of_stench; //disjuctive stench childs
        //only stench
        if(temp_tile.stench && !temp_tile.breeze ){
            //do stench things
            if(!wumpus_dead) {
                vector<Position> stench_child = tiles_expansion[Position(current_X, current_Y)];
                vector<Position> wumpus_pos;
                //check wumpus pos is not already in safe
                for (int i = 0; i < stench_child.size(); ++i) {
                    deque<Position>::iterator it = find(safe.begin(), safe.end(), stench_child[i]);
                    if (it == safe.end()) {
                        wumpus_pos.push_back(stench_child[i]);
//                        cout << stench_child[i].first << "," << stench_child[i].second << endl;
                    }
//                    cout << stench_child[i].first << "," << stench_child[i].second << endl;
                }

                //first time percieve stench
                if (possible_wumpus.size() == 0) {
                    possible_wumpus = wumpus_pos;
                }
                else {
                    //not first time percieve stench

                    //get intersection
                    deque<Position> intersection;
//                    set_intersection(possible_wumpus.begin(),possible_wumpus.end(),
//                                     wumpus_pos.begin(),wumpus_pos.end(),intersection.begin());

                    deque<Position> handle_previous_possible_wumpus;

                    for(auto i:possible_wumpus){
                        for(auto j:wumpus_pos){
                            if(i==j)
                                intersection.push_back(j);
                        }

                        if(find(intersection.begin(),intersection.end(),i) == intersection.end())
                            handle_previous_possible_wumpus.push_back(i);
                    }

                    //handle previous wumpus tiles
                    for(auto i:handle_previous_possible_wumpus){
                        bool exist = false;
                        for(auto j:not_unsafe){
                            if(find(j.begin(),j.end(),i) != j.end())
                                exist = true;
                        }
                        if(!exist)
                            if(find(safe.begin(),safe.end(),i) == safe.end())
                                safe.push_back(i);
                    }



//                    //get union
//                    set_union(possible_wumpus.begin(),possible_wumpus.end(),
//                                     wumpus_pos.begin(),wumpus_pos.end(),
//                                     std::inserter(union_of_stench,union_of_stench.begin()));
//
//                    //union - intersection
//                    union_of_stench.erase(intersection.begin(),intersection.end());

                    possible_wumpus.clear();
                    possible_wumpus.insert(possible_wumpus.begin(),intersection.begin(),intersection.end());

                }

            }


            //do !breeze things
            vector<Position> save_pos;
            for(auto p:tiles_expansion[Position(current_X, current_Y)]){
                //tiles not already in possible_wumpus or safe or unsafe
                if(find(possible_wumpus.begin(),possible_wumpus.end(),p) == possible_wumpus.end()
                   &&find(safe.begin(), safe.end(),p) == safe.end()
                     &&find(unsafe.begin(),unsafe.end(),p) == unsafe.end())
                    save_pos.push_back(p);
            }

            //update safe
            for(int i=0; i<save_pos.size();++i) {
                if (find(safe.begin(), safe.end(), save_pos[i]) == safe.end())
                    safe.push_back(save_pos[i]);
            }

            //update not_unsafe
            //if there is safe position in not_unsafe, pop it
            deque<deque<Position>> new_not_unsafe;

            for(int i = 0; i < not_unsafe.size(); ++i) {
                for(int j = 0; j<save_pos.size();++j){
                    deque<Position>::iterator it = find(not_unsafe[i].begin(),not_unsafe[i].end(),save_pos[j]);
                    if(it != not_unsafe[i].end())
                        not_unsafe[i].erase(it);
                }

                //update unsafe
                if(not_unsafe[i].size() == 1){
                    if(find(unsafe.begin(),unsafe.end(),not_unsafe[i][0]) == unsafe.end())
                        unsafe.push_back(not_unsafe[i][0]);
                }
                else
                    new_not_unsafe.push_back(not_unsafe[i]);
            }
            not_unsafe = new_not_unsafe;


        }
            //only breeze
        else if (temp_tile.breeze && !temp_tile.stench) {
            //push tiles around into not_unsafe
            deque<Position> not_unsafe_pos;
            for (int i = 0; i < tiles_expansion[temp_pos].size(); ++i) {
                //check this tile is not already in safe or in possible wumpus tiles
                if (find(safe.begin(), safe.end(), tiles_expansion[temp_pos][i]) == safe.end())
                        //&& find(possible_wumpus.begin(), possible_wumpus.end(), tiles_expansion[temp_pos][i]) == possible_wumpus.end())
                    not_unsafe_pos.push_back(tiles_expansion[temp_pos][i]);
            }
            not_unsafe.push_back(not_unsafe_pos);
        }
            //both breeze && stench
        else {
            ///////////////////////////////do stench things///////////////////////////
            if(!wumpus_dead) {
                vector<Position> stench_child = tiles_expansion[Position(current_X, current_Y)];
                vector<Position> wumpus_pos;
                //check wupums pos is not already in safe
                for (int i = 0; i < stench_child.size(); ++i) {
                    deque<Position>::iterator it = find(safe.begin(), safe.end(), stench_child[i]);
                    if (it == safe.end()) {
                        wumpus_pos.push_back(stench_child[i]);
//                        cout << stench_child[i].first << "," << stench_child[i].second << endl;
                    }
//                    cout << stench_child[i].first << "," << stench_child[i].second << endl;
                }

                //first time percieve stench
                if (possible_wumpus.size() == 0) {
                    possible_wumpus = wumpus_pos;
                }
                else {
                    //not first time percieve stench

                    //get intersection
                    deque<Position> intersection;
                    deque<Position> handle_previous_possible_wumpus;

                    for(auto i:possible_wumpus){
                        for(auto j:wumpus_pos){
                            if(i==j)
                                intersection.push_back(j);
                        }

                        if(find(intersection.begin(),intersection.end(),i) == intersection.end())
                            handle_previous_possible_wumpus.push_back(i);
                    }

                    //handle previous wumpus tiles
                    for(auto i:handle_previous_possible_wumpus){
                        bool exist = false;
                        for(auto j:not_unsafe){
                            if(find(j.begin(),j.end(),i) != j.end())
                                exist = true;
                        }
                        if(!exist)
                            if(find(safe.begin(),safe.end(),i) == safe.end())
                                safe.push_back(i);
                    }



//                    set_intersection(possible_wumpus.begin(),possible_wumpus.end(),
//                                     wumpus_pos.begin(),wumpus_pos.end(),
//                                     back_inserter(intersection));
//                    //get union
//                    set_union(possible_wumpus.begin(),possible_wumpus.end(),
//                              wumpus_pos.begin(),wumpus_pos.end(),
//                              std::inserter(union_of_stench,union_of_stench.begin()));
//
//                    //union - intersection
//                    union_of_stench.erase(intersection.begin(),intersection.end());

                    possible_wumpus.clear();
                    possible_wumpus.insert(possible_wumpus.begin(),intersection.begin(),intersection.end());
                }
            }


            ///////////////////////////////do breeze things///////////////////////////

            //push tiles around into not_unsafe
            deque<Position> not_unsafe_pos;
            deque<Position> save_pos;
            for(auto p:tiles_expansion[Position(current_X, current_Y)]){
                //check this tile is not already in safe or possible wumpus or unsafe
                if(find(safe.begin(), safe.end(), p) == safe.end())
                   //&&find(possible_wumpus.begin(),possible_wumpus.end(),p) == possible_wumpus.end())
                    save_pos.push_back(p);
            }
            for (int i = 0; i < save_pos.size(); ++i) {
                not_unsafe_pos.push_back(save_pos[i]);
            }
            not_unsafe.push_back(not_unsafe_pos);
        }




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
            has_arrow = false;
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

deque<Agent::Action> MyAI::shoot_wumpus() {
    deque<Agent::Action> actions;
    if(possible_wumpus.size() == 0) {
        actions.push_back(SHOOT);
        return actions;
    }
    else {
        vector<Position> wumpus_pos;
        wumpus_pos.insert(wumpus_pos.begin(),possible_wumpus.begin(),possible_wumpus.end());

        Position des = Position(100,100);
        Position wumpus = wumpus_pos[wumpus_pos.size()-1];
        while(wumpus_pos.size() >0) {
            wumpus_pos.pop_back();

            for (int i = 0; i < safe.size(); ++i) {
                if (safe[i].first == wumpus.first
                    || safe[i].second == wumpus.second) {
                    des = safe[i];
                    break;
                }
            }

            //check if des is not null
            if (des.first != 100) {
                deque<Position> goals;
                goals.push_back(des);
                vector<Position> path = route_generator(Position(current_X, current_Y), goals, safe);
                actions = actions_generator(path);

                int des_dir;
                if (!path.empty()) {
                    Position before_des;
                    if (path.size() > 1)
                        before_des = path[1];
                    else
                        before_des = Position(current_X, current_Y);

                    if (des.first > before_des.first)
                        des_dir = 0;
                    else if (des.first < before_des.first)
                        des_dir = 2;
                    else if (des.second > before_des.second)
                        des_dir = 1;
                    else if (des.second < before_des.second)
                        des_dir = 3;
                    else
                        des_dir = current_Dir;
                } else
                    des_dir = current_Dir;

                int tar_dir;
                if (des.first == wumpus.first){
                    if(des.second < wumpus.second)
                        tar_dir = 1;
                    else
                        tar_dir = 3;
                }
                else if (des.second == des.second)
                {
                    if (des.first < wumpus.first)
                        tar_dir = 0;
                    else
                        tar_dir = 2;
                }



                //turn right, and shoot
                if (tar_dir - des_dir == 1 || tar_dir - des_dir == -3) {
                    actions.push_back(TURN_LEFT);
                    actions.push_back(SHOOT);
                }
                    //turn left, and shoot
                else if (tar_dir - des_dir == -1 || tar_dir - des_dir == 3) {
                    actions.push_back(TURN_RIGHT);
                    actions.push_back(SHOOT);
                }
                    //turn back, and shoot
                else if (tar_dir - des_dir == -2 || tar_dir - des_dir == 2) {
                    actions.push_back(TURN_LEFT);
                    actions.push_back(TURN_LEFT);
                    actions.push_back(SHOOT);
                }
                    //same dir, shoot
                else {
                    actions.push_back(SHOOT);
                }

//                cout << "Target dir:" << tar_dir << " Des_dir:" << des_dir << endl;
//                cout << "Path:";
//                for(auto i : path){
//                    cout<< "(" << i.first << "," << i.second << ")";
//                }
//                cout << endl;

                return actions;
            }
        }
        actions.push_back(SHOOT);
        return actions;


    }

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
        update_safe(stench,breeze);
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

            for(int i=0; i<safe.size();++i){
                //safe and not possible wumpus tile
                if(find(possible_wumpus.begin(),possible_wumpus.end(),safe[i]) == possible_wumpus.end())
                    allowed_tiles.push_back(safe[i]);
            }

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

            deque<Position>::iterator it = find(safe.begin(), safe.end(), Position(current_X, current_Y));
            if(it != safe.end()) {
                safe.erase(it);
                if(find(unsafe.begin(),unsafe.end(),Position(current_X,current_Y)) == unsafe.end())
                    unsafe.push_back(Position(current_X, current_Y));
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

            vector<Position> route;
            deque<Action> actions;
            deque<Position> goals;
            deque<Position> allowed_tiles;

            for (int i = 0; i < safe.size(); i++) {
                //safe and not possible wumpus tile
                if(find(possible_wumpus.begin(),possible_wumpus.end(),safe[i]) == possible_wumpus.end()) {

                    //allowed tiles contain all safe tiles
                    allowed_tiles.push_back(safe[i]);
                    //any safe tile which is not current tile or visited tile can be the goal tile
                    if (safe[i] != Position(current_X, current_Y)
                        && !KB[safe[i].first][safe[i].second].visited) {
                        goals.push_back(safe[i]);
                    }
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
                //try to shoot wumpus
            else if(has_arrow && possible_wumpus.size()!=0){
                actions = shoot_wumpus();
                plan.insert(plan.cend(), actions.begin(), actions.end());
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
        }
        else if(scream) {

            //debug
//            cout <<"Scream"<<endl;
//            safe.insert(safe.end(),possible_wumpus.begin(),possible_wumpus.end());
            possible_wumpus.clear();
            wumpus_dead = true;


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
                }

            }

            if(!goals.empty()) {

                route = route_generator(pair<int, int>(current_X, current_Y), goals, allowed_tiles);

                if (!route.empty())
                    actions = actions_generator(route);

                if (!actions.empty())
                    plan.insert(plan.cend(), actions.begin(), actions.end());
            }
                //try to shoot wumpus
            else if(has_arrow && possible_wumpus.size()!=0){
                actions = shoot_wumpus();
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


        else if (breeze || stench) {

            vector<Position> route;
            deque<Action> actions;
            deque<Position> allowed_tiles;
            deque<Position> goals;




            for (int i = 0; i < safe.size(); i++) {
                //safe and not possible wumpus tile
                if(find(possible_wumpus.begin(),possible_wumpus.end(),safe[i]) == possible_wumpus.end()) {
                    //allowed tiles contain all safe tiles
                    allowed_tiles.push_back(safe[i]);
                    //any safe tile which is not current tile or visited tile can be the goal tile
                    if (safe[i] != Position(current_X, current_Y) && !KB[safe[i].first][safe[i].second].visited) {
                        goals.push_back(safe[i]);
                    }
                }
            }

            //if there are no unviisted safe tiles
            if(goals.empty()){
                //try to shoot wumpus
                if(has_arrow && possible_wumpus.size()!=0){
                    actions = shoot_wumpus();
                    plan.insert(plan.cend(), actions.begin(), actions.end());
                }
                else {
                    //go back
                    goals.push_back(pair<int, int>(0, 0));
                    route = route_generator(pair<int, int>(current_X, current_Y), goals, allowed_tiles);
                    if (!route.empty()) {
                        actions = actions_generator(route);
                        if (!actions.empty())
                            plan.insert(plan.cend(), actions.begin(), actions.end());
                    }
                    plan.push_back(CLIMB);
                }
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

            for (int i = 0; i < safe.size(); i++) {
                //safe and not possible wumpus tile
                if(find(possible_wumpus.begin(),possible_wumpus.end(),safe[i]) == possible_wumpus.end()) {
                    //allowed tiles contain all safe tiles
                    allowed_tiles.push_back(safe[i]);
                    //any safe tile which is not current tile or visited tile can be the goal tile
                    if (safe[i] != Position(current_X, current_Y) && !KB[safe[i].first][safe[i].second].visited) {
                        goals.push_back(safe[i]);
                    }
                }
            }

            if(!goals.empty()) {

                route = route_generator(pair<int, int>(current_X, current_Y), goals, allowed_tiles);

                if (!route.empty())
                    actions = actions_generator(route);

                if (!actions.empty())
                    plan.insert(plan.cend(), actions.begin(), actions.end());
            }
                //try to shoot wumpus
            else if(has_arrow && possible_wumpus.size()!=0){
                actions = shoot_wumpus();
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

//    cout <<"Current pos:" << "[" << current_X << "," << current_Y << "]" << endl;
//    cout <<"Tile expansion:" ;
//    for(auto i:tiles_expansion[Position(current_X,current_Y)])
//        cout<< "[" << i.first << "," << i.second << "] ";
//    cout << endl;

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

//    debug
//    cout << "W:" <<possible_wumpus.size() << endl;
//    for(int i=0;i<possible_wumpus.size();++i)
//        cout << "[" << possible_wumpus[i].first << "," << possible_wumpus[i].second << "]" << endl;
//
//    cout << "Safe:" << endl;
//    for(int i=0;i<safe.size();++i)
//        cout << "[" << safe[i].first << "," << safe[i].second << "]" << endl;
//
//    cout << "Unsafe:" << endl;
//    for(int i=0;i<unsafe.size();++i)
//        cout << "[" << unsafe[i].first << "," << unsafe[i].second << "]" << endl;
//
//    cout << "Not_unsafe:" << endl;
//    for(int i=0;i<not_unsafe.size();++i) {
//        cout << "[";
//        for (int j = 0; j < not_unsafe[i].size(); ++j)
//             cout << "[" << not_unsafe[i][j].first << "," << not_unsafe[i][j].second << "]";
//        cout << "]" << endl;
//    }
//
//    if(unsafe.size()>100)
//        cout<<"?";



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