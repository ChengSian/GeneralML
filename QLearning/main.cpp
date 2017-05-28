//
//  main.cpp
//  QLearn
//
//  Created by Hans on 2017/5/27.
//  Copyright © 2017年 Hans. All rights reserved.
//

#include <iostream>
#include "QLearn/QLearn.hpp"

#if defined __unix__ || defined __APPLE__
#define pause() {std::cout << "Press any key to continue..." << endl;system("read");}
#else
#if defined _WIN32
#define pause() {system("pause");}
#endif
#endif

QLearn<9, 9>::RewardType reward = {
    { 5,    3,  -3,  -5,  -7,  -9, -11, -13, -15}, // S = 0
    { 3,    5,   3,  -3,  -5,  -7,  -9, -11, -13}, // S = 1
    {-3,    3,   5,   3,  -3,  -5,  -7,  -9, -11}, // S = 2
    {-5,   -3,   3,   5,   3,  -3,  -5,  -7,  -9}, // S = 3
    {-7,   -5,  -3,   3,   5,   3,  -3,  -5,  -7}, // S = 4
    {-9,   -7,  -5,  -3,   3,   5,   3,  -3,  -5}, // S = 5
    {-11,  -9,  -7,  -5,  -3,   3,   5,   3,  -3}, // S = 6
    {-13, -11,  -9,  -7,  -5,  -3,   3,   5,   3}, // S = 7
    {-15, -13, -11,  -9,  -7,  -5,  -3,   3,   5}  // S = 8
};
QLearn<9, 9> qlearn(&reward);
using namespace std;

uint getPos(uint status, uint action) {
    static int pos = status-4;
    int act = action-4;
    pos -= act * 0.87;
    if (pos >  4) return 8;
    if (pos < -4) return 0;
    return pos+4;
}

int main(int argc, const char * argv[]) {
    uint stat, prev_stat = 4;
    qlearn.PrintMatrix();
    for (;;) {
        string s;
        cout << "Select action (l=Learn, r=Run): ";
        cin >> s;
        if (s == "l") {
            srand(time(NULL));
            for (uint t=0; t < 1000; t++) {
                uint act = rand() % 9;
                cout << "Action: " << act+1 << endl;
                stat = getPos(prev_stat, act);
                cout << "Status: " << stat+1 << endl;
                cout << "Reward: " << qlearn.Learn(prev_stat, stat, act, 0.89, 0.8) << endl;
                prev_stat = stat;
            }
            qlearn.PrintMatrix();
        } else if (s == "r") {
            uint act = qlearn.GetResult(prev_stat);
            stat = getPos(prev_stat, act);
            cout << "Status: " << prev_stat+1 << endl;
            cout << "Action: " << act+1 << endl;
            prev_stat = stat;
        }
    }
    return 0;
}
