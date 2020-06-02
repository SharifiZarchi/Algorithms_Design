//
// Created by Mahsa Sheikhi on 5/18/20.
//
#include <bits/stdc++.h>
#include <iostream>

using namespace std;
deque<string> result;
multimap<int,string> res;
//int today =4;
//int bias;
deque<int> daysArr;


void bruteForce(deque<int> arr, int leftIndex, int rightIndex) {
    if (rightIndex - leftIndex + 1 == 2) {
        result.push_back(to_string(1) + " " + to_string(1) + " " + to_string(arr[leftIndex]) + " " +
                         to_string(arr[rightIndex]));
        res.insert({1,to_string(1) + " " + to_string(1) + " " + to_string(arr[leftIndex]) + " " +
                      to_string(arr[rightIndex])});


        result.push_back(to_string(2) + " " + to_string(1) + " " + to_string(arr[leftIndex]) + " " +
                         to_string(arr[rightIndex]));
        res.insert({1,to_string(2) + " " + to_string(1) + " " + to_string(arr[leftIndex]) + " " +
                      to_string(arr[rightIndex])});

        result.push_back(to_string(1) + " " + to_string(2) + " " + to_string(arr[rightIndex]) + " " +
                         to_string(arr[leftIndex]));
        res.insert({2,to_string(1) + " " + to_string(2) + " " + to_string(arr[rightIndex]) + " " +
                      to_string(arr[leftIndex])});

        result.push_back(to_string(1) + " " + to_string(2) + " " + to_string(arr[leftIndex]) + " " +
                         to_string(arr[leftIndex]));
        res.insert({2,to_string(1) + " " + to_string(2) + " " + to_string(arr[leftIndex]) + " " +
                      to_string(arr[leftIndex])});

        result.push_back(to_string(2) + " " + to_string(2) + " " + to_string(arr[rightIndex]) + " " +
                         to_string(arr[leftIndex]));
        res.insert({2,to_string(2) + " " + to_string(2) + " " + to_string(arr[rightIndex]) + " " +
                      to_string(arr[leftIndex])});


        result.push_back(to_string(1) + " " + to_string(3) + " " + to_string(arr[rightIndex]) + " " +
                         to_string(arr[rightIndex]));
        res.insert({3,to_string(1) + " " + to_string(3) + " " + to_string(arr[rightIndex]) + " " +
                      to_string(arr[rightIndex])});

    } else if (rightIndex - leftIndex + 1 == 1) {
        return;
    }
    return;
}


void divideAndConquer(deque<int> arr, int leftIndex, int rightIndex) {
    if (rightIndex - leftIndex + 1 <= 2) {
        bruteForce(arr, leftIndex, rightIndex);
        return;
    }

    int middle = leftIndex + (rightIndex - leftIndex) / 2;


    divideAndConquer(arr, leftIndex, middle);
    divideAndConquer(arr, middle + 1, rightIndex);


    int maxDay = -1;


    for (int i = leftIndex; i <= rightIndex; ++i) {
        daysArr[i]++;
        if (daysArr[i] > maxDay)
            maxDay = daysArr[i];
    }
    for (int i = leftIndex; i <= rightIndex; ++i)
        daysArr[i] = maxDay;


    int counter;
    int today = daysArr[leftIndex];
    for (int j = middle + 1, counter = 1; j <= rightIndex; ++j, ++counter) {
        for (int i = leftIndex; i <= middle; ++i) {

            result.push_back(to_string(1) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                             to_string(arr[i + counter]));
            res.insert({today,to_string(1) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                              to_string(arr[i + counter])});
            if (i + counter >= middle + 1){
                result.push_back(to_string(2) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                                 to_string(arr[i + counter]));

                res.insert({today,to_string(2) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                                  to_string(arr[i + counter])});
            }


        }
        today++;
    }


    for (int j = rightIndex, counter = rightIndex - middle + 1; j > middle; --j, ++counter) {
        for (int i = leftIndex; i <= middle; ++i) {

            if (i + counter > rightIndex) {
                int temp = i + counter - rightIndex;
                result.push_back(to_string(1) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                                 to_string(arr[i + counter - 2*temp]));

                res.insert({today,to_string(1) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                                  to_string(arr[i + counter - 2*temp])});


            } else {

                result.push_back(to_string(1) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                                 to_string(arr[i + counter]));
                res.insert({today,to_string(1) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                                  to_string(arr[i + counter])});

                result.push_back(to_string(2) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                                 to_string(arr[i + counter]));
                res.insert({today,to_string(2) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                                  to_string(arr[i + counter])});
            }


        }
        today++;
    }


    float tmp = 0;
    int tmp2 = 0;
    for (int j = 2*middle-leftIndex; j > leftIndex; --j,tmp+=0.5) {
        tmp2=1;
        for (int i = leftIndex; i < middle-tmp; ++i,tmp2++) {
            result.push_back(to_string(1) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                             to_string(arr[j-tmp2]));
            res.insert({today,to_string(1) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                              to_string(arr[j-tmp2])});
        }
        today++;
    }



    for (int j = middle , counter = 1; j >= leftIndex; --j, ++counter) {
        for (int i = middle+1; i <= rightIndex; ++i) {

            result.push_back(to_string(1) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                             to_string(arr[i - counter]));
            res.insert({today,to_string(1) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                              to_string(arr[i - counter])});
            if (i - counter <= middle ){
                result.push_back(to_string(2) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                                 to_string(arr[i - counter]));
                res.insert({today,to_string(2) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                                  to_string(arr[i - counter])});
            }


        }
        today++;
    }


    for (int j = leftIndex, counter = middle+1 - leftIndex + 1; j < middle+1; ++j, ++counter) {
        for (int i = middle+1; i <= rightIndex; ++i) {
//            cout<<i+1<<"  "<<i-counter<<" "<<leftIndex+1<<endl;

            if (i - counter < leftIndex) {
                int temp = i - counter - leftIndex;
                result.push_back(to_string(1) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                                 to_string(arr[i - counter - 2*temp]));

                res.insert({today,to_string(1) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                                  to_string(arr[i - counter - 2*temp])});


            } else {

                result.push_back(to_string(1) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                                 to_string(arr[i - counter]));
                res.insert({today,to_string(1) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                                  to_string(arr[i - counter])});

                result.push_back(to_string(2) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                                 to_string(arr[i - counter]));
                res.insert({today,to_string(2) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                                  to_string(arr[i - counter])});
            }


        }
        today++;
    }


    tmp = 0;
    tmp2 = 0;
    for (int j = middle+1 -(rightIndex-middle-1); j < rightIndex; ++j,tmp+=0.5) {
        tmp2=1;
        for (int i = rightIndex; i > middle+1+tmp; --i,tmp2++) {
            result.push_back(to_string(1) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                             to_string(arr[j+tmp2]));

            res.insert({today,to_string(1) + " " + to_string(today) + " " + to_string(arr[i]) + " " +
                              to_string(arr[j+tmp2])});
        }
        today++;
    }









    today--;

    for (int i = leftIndex; i <= rightIndex; ++i)
        daysArr[i] = today;





//    for (int j = 0; j < 18; ++j) {
//        cout<<daysArr[j]<<" , ";
//    }
//    cout<<endl;
//    cout << "left : " << leftIndex << " , middle : " << middle << " , right  : " << rightIndex << endl;
//    cout << "----------" << endl;


    return;
}


int main() {

    int n, m;
    cin >> n;
    cin >> m;
    int temp1, temp2, temp3;
    deque<int> arr;


    for (int i = 0; i < n; ++i) {
        cin >> temp1;
        daysArr.push_back(3);
    }


    for (int j = 0; j < m; ++j) {
        cin >> temp1;
        arr.push_back(temp1);

        cin >> temp2;
        cin >> temp3;
    }
    arr.push_back(temp2);


    divideAndConquer(arr, 0, arr.size() - 1);

    long long num = res.size();

//    cout << num << endl;
    cout<<res.size()<<endl;
//    for (int l = 0; l < num; ++l) {
//        cout << result[l] << endl;
//    }

    for(auto it=res.begin(); it != res.end();it++){
        cout<<(*it).second<<endl;
    }




}
