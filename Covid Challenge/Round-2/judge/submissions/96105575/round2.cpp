#include<iostream>
#include <bits/stdc++.h> 
#include<vector>

using namespace std;

int *p;


bool compare(int x,int y){
    return p[x] > p[y];
}

bool sort_moves(vector<int> x,vector<int> y){
    if(x[1] == y[1])
        return  x[0] < y[0];
    return x[1] < y[1];
}



//finds who the person should visit
int find_target(vector<int> remaining_travels, vector<int> moves){
    for(auto target = remaining_travels.begin();target!=remaining_travels.end();++target){
        if(moves[*target] == -1){
            return *target;
            }
        }

    return -1;
};

//updates a person location and tells if it is his own
bool update_locations(vector<vector<int>> *peoples_locations, int person, int* locations, int move){

    for (auto all_people = (*peoples_locations)[locations[person]].begin(); all_people != (*peoples_locations)[locations[person]].end(); all_people++)
        if (*all_people == person){
            (*peoples_locations)[locations[person]].erase(all_people);
            break;
        }
    
    (*peoples_locations)[move].push_back(person);
    locations[person] = move;
    
    return person == move;
}

void update_remaining_travels(vector<vector<int>> *remaining_travels, int person, int remove){

    for (auto travel = (*remaining_travels)[person].begin(); travel != (*remaining_travels)[person].end(); travel++)
        if (*travel == remove){
            (*remaining_travels)[person].erase(travel);
            return;
        }


}

int main(){

    int m, n, total_travels = 0, cost = 0;
    vector<vector<int>> remaining_travels = {};
    vector<vector<int>> people_in_city = {};
    vector<vector<int>> print_moves={};
    

    cin >> n >> m;

    p = new int[n];
    int expensive_cities[n];
    int location[n];

    for(auto i = 0; i < n; i++){
        
        cin >> p[i];
        expensive_cities[i] = i;
        people_in_city.push_back(vector<int>());
        people_in_city[i].push_back(i);
        location[i] = i;
    };
    
    for(int i = 0; i<m; i++){
        int x;
        cin >> x >>x >> x;
    };

    for (int i = 0; i < n; i++){
        vector<int> travles = {};
        for (int j = 0; j < n; j++)
            if(i!=j)
                travles.push_back(j);
        remaining_travels.push_back({travles});
        
    }

    sort(expensive_cities, expensive_cities + n, compare);
  
    //logic starts here


    
    for (int day = 1; total_travels < (n * (n-1)); ++day){
        vector<int> moves(n, -1);

      //finding moves in each turn
        for (int i = 0; i < n; i++){
            vector<int> peaple = people_in_city[expensive_cities[i]];
            for(auto person=peaple.begin();person!=peaple.end();++person){
                if(moves[*person] == -2 || moves[*person] >=0)
                    continue;
                int target = find_target(remaining_travels[*person], moves);
                if(target >= 0){
                        moves[*person] = target;
                
                    if(location[target] != target)
                        moves[target] = target;
                    else
                        moves[target] = -2;

                }else{
                    
                     if(location[*person] != expensive_cities[n-1])
                        moves[*person] = -3;

                }
            }
        }

/*
    cout << "Moves: ";
    for (int i = 0; i < n; i++){
        cout << moves[i] << ", ";
    }
    cout << endl;
*/

    //applying moves and printing them 
    for (int i = 0; i < n; i++){
        if(moves[i] >= 0){
            bool host = update_locations(&people_in_city, i, location, moves[i]);
            if(host)
                print_moves.push_back({1, day, i, moves[i]});
            
            else {
                print_moves.push_back({1, day, i, moves[i]});
                print_moves.push_back({2, day, i, moves[i]});
                update_remaining_travels(&remaining_travels, i, moves[i]);
                total_travels ++;
            }
        }else if(moves[i] == -2){
            bool host = update_locations(&people_in_city, i, location, location[i]);
            if(!host){
                print_moves.push_back({2, day, i, location[i]});
                update_remaining_travels(&remaining_travels, i, location[i]);
                total_travels ++;

            }
           
        }else if(moves[i] == -3){
            update_locations(& people_in_city, i, location, expensive_cities[n - 1]);
            print_moves.push_back({1, day, i, expensive_cities[n - 1]});

        }
        
    }


    //calculate cost

    for(int i = 0; i < n; i++)
        cost += p[location[i]];
    

        

    // cout << "Day: "<< day << ", Travels: "<< total_travels<<endl;
    
    }
    sort(print_moves.begin(), print_moves.end(), sort_moves);

    // cout << cost<< endl;
     cout << print_moves.size()<< endl;

    
    for (auto i = print_moves.begin(); i != print_moves.end(); i++)
    {
        cout<< (*i)[0]<< " " << (*i)[1]<< " " <<(*i)[2] + 1 << " " << (*i)[3] + 1<< endl;
    }
    
}

