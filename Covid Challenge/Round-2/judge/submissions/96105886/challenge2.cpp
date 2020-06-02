//
// Created by Mahsa Sheikhi on 4/30/20.
//
#include <iostream>
#include <random>
#include <algorithm>
#include <map>
#include <deque>

using namespace std;
#define SIZE 20


struct Edge {
    int u;
    int v;
};
deque<Edge> edges;


//void getPairs(int arr[], int n) {
//
//
//    Edge temp;
//    // Consider each and every Edge
//    for (int i = 0; i < n; i++) {
//
//        for (int j = i + 1; j < n; ) {
//
////            if (Edges[index].find(temp1) == Edges[index].end() ) {
////                j++;
////                Edges[index].insert({temp2,temp1});
////            }
//
//
//            temp.v = arr[i];
//            temp.u = arr[j];
//            edges.push_back(temp);
//
//        }
//    }
//
//    return;
//}


int main() {
    deque<string> result;
    int n, m;
    cin >> n;
    cin >> m;
    int temp1, temp2, temp3;
    int cities[101];
    Edge temp;
    int lowestCostCity;
    int minim = 2147483647;

    long long today = 1;
    Edge oddTemp;


    for (int i = 0; i < n; ++i) {
        cin >> temp1;
        if (temp1 < minim) {
            lowestCostCity = i + 1;
            minim = temp1;

        }


    }

    for (int j = 0; j < m; ++j) {
        cin >> temp1;
        cin >> temp2;
        cin >> temp3;
    }
    for (int k = 1; k < n; ++k) {
        cities[k - 1] = k + 1;
    }

    if (n % 2 == 1) {
        cities[n - 1] = -1;
        n++;
    }

    Edge first;
    first.v = 1;
    int counter = 0;
    int index;
    deque<int> leftCities;
    deque<int> rightCities;
    int familyLoc[n];

    for (int i1 = 0; i1 < n; ++i1) {
        familyLoc[i1] = i1;
    }


//    cout<< lowestCostCity<<endl;

    for (int k = 0; k < n - 1; ++k) {
        first.u = cities[k];
        edges.clear();
        leftCities.clear();
        rightCities.clear();

        edges.push_back(first);


        counter = 0;
        index = k + 1;

        while (counter < ((n - 2) / 2)) {
            if (index > n - 2)
                index = 0;

            rightCities.push_back(cities[index]);
            counter++;
            index++;


        }



//        if( n % 2 == 1 ){
////            index++;
//            if (index > n - 2)
//                index = 0;
//
//            if( k % 2 == 0){
//                oddTemp.v = cities[index];
//            }
//            else{
//                oddTemp.u = cities[index];
//                edges.push_back(oddTemp);
//            }
//        }





        counter = 0;
        index = k - 1;

        while (counter < ((n - 2) / 2)) {
            if (index < 0)
                index = n - 2;

            leftCities.push_back(cities[index]);

            counter++;
            index--;


        }

        for (int i = 0; i < leftCities.size(); ++i) {
            temp.v = leftCities[i];
            temp.u = rightCities[i];
            edges.push_back(temp);

        }

        for (int j = 0; j < edges.size(); ++j) {
            temp1 = edges[j].v;
            temp2 = edges[j].u;

            if (temp1 == -1 || temp2 == -1) {

                if (temp1 == -1 && familyLoc[temp2 - 1] != lowestCostCity - 1){
                    result.push_back(to_string(1) + " " + to_string(today) + " " + to_string(temp2) + " " +
                                     to_string(lowestCostCity));
                    familyLoc[temp2 - 1] = lowestCostCity - 1;

                }

                else if(temp2 == -1 && familyLoc[temp1 - 1] != lowestCostCity - 1){
                    result.push_back(to_string(1) + " " + to_string(today) + " " + to_string(temp1) + " " +
                                     to_string(lowestCostCity));
                    familyLoc[temp1 - 1] = lowestCostCity - 1;
                }


                continue;

            }


//            cout<<temp1<<" , "<<temp2<<endl;

            if (familyLoc[temp1 - 1] != temp1 - 1) {
                result.push_back(to_string(1) + " " + to_string(today) + " " + to_string(temp1) + " " +
                                 to_string(temp1));

                familyLoc[temp1 - 1] = temp1 - 1;
            }

            if (familyLoc[temp2 - 1] != temp1 - 1) {
                result.push_back(to_string(1) + " " + to_string(today) + " " + to_string(temp2) + " " +
                                 to_string(temp1));
                familyLoc[temp2 - 1] = temp1 - 1;
            }


            result.push_back(to_string(2) + " " + to_string(today) + " " + to_string(temp2) + " " +
                             to_string(temp1));


        }

        today++;

        for (int j = 0; j < edges.size(); ++j) {
            temp2 = edges[j].v;
            temp1 = edges[j].u;

            if (temp1 == -1 || temp2 == -1){
                continue;

            }

            if (familyLoc[temp1 - 1] != temp1 - 1) {
                result.push_back(to_string(1) + " " + to_string(today) + " " + to_string(temp1) + " " +
                                 to_string(temp1));
                familyLoc[temp1 - 1] = temp1 - 1;
            }

            if (familyLoc[temp2 - 1] != temp1 - 1) {
                result.push_back(to_string(1) + " " + to_string(today) + " " + to_string(temp2) + " " +
                                 to_string(temp1));
                familyLoc[temp2 - 1] = temp1 - 1;
            }


            result.push_back(to_string(2) + " " + to_string(today) + " " + to_string(temp2) + " " +
                             to_string(temp1));


        }

        today++;


    }

//    cout << leftCities.size() << endl;

//    cout << rightCities.size() << endl;



































//    for (int i = 0; i < n; i++) {
//
//        for (int j = i + 1; j < n; j++) {
//
//            temp.v = cities[i];
//            temp.u = cities[j];
//            edges.push_back(temp);
//
//        }
//    }
//
//
////    for (int k = 0; k < n* (n-1)/2; ++k) {
////        cout<<edges[k].v<<" "<<edges[k].u<<endl;
////    }
//
//    deque<Edge> oneDayWork;
//    bool isErased = false;
//
//    while (!edges.empty()) {
//        oneDayWork.clear();
//
//
//        for (auto it2 = edges.begin(); it2 != edges.end();) {
////            cout << (*it2).v << " " << (*it2).u << endl;
//            isErased = false;
//
//            for (int i = 0; i < oneDayWork.size(); ++i) {
//
//
//                if ((*it2).v == oneDayWork[i].v || (*it2).u == oneDayWork[i].v || (*it2).v == oneDayWork[i].u ||
//                      (*it2).u == oneDayWork[i].u)
//                    break;
//
//
//
//                if( i == oneDayWork.size()-1){
//                    oneDayWork.push_back(*it2);
////                    cout<<(*it2).v<<" "<<(*it2).u<<"  ,  "<<oneDayWork[i].v<<" "<<oneDayWork[i].u  <<endl;
//
//                    edges.erase(it2);
//                    isErased = true;
//                    break;
//
//                }
//
//
//
//
//
//
//            }
//
//
//
//
//            if (oneDayWork.size() == 0) {
//                oneDayWork.push_back(*it2);
//                edges.erase(it2);
//                isErased = true;
//
//
//            }
//            if( !isErased)
//                it2++;
//
//
//        }
//        cout<<"******"<<endl;
//
//
//        for (int i = 0; i < oneDayWork.size(); ++i) {
////result
//            cout << oneDayWork[i].v << " " << oneDayWork[i].u << endl;
//
//
//        }
//        cout << "--------" << endl;
//
//
//    }


    int num = result.size();

    cout << num << endl;
    for (int l = 0; l < num; ++l) {
        cout << result[l] << endl;
    }


    return 0;

//    for (int k = 0; k < 4; ++k) {
//
//
//    for(auto it=Edges[k].begin();it!=Edges[k].end();it++)
//    {
//        std::cout<<it->first<<" "<<it->second<<std::endl;
//    }
//    cout<<"---------"<<endl;
//    }


}


