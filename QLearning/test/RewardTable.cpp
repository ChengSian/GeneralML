//
//  main.cpp
//  QLearn
//
//  Created by Hans on 2017/5/27.
//  Copyright © 2017年 Hans. All rights reserved.
//

#include <iostream>
#include <QLearn/QLearn.hpp>
#include "Memory.h"

using namespace std;

int RewardTable() {
    cout << "Q-Learning with reward table." << endl;
    volatile size_t before = memory_used();
    QLearn<3, 3>::RewardType reward = {
        {  1,  0, -1}, // S = 0
        {  0,  1,  0}, // S = 1
        { -1,  0,  1}, // S = 2
    };
    QLearn<3, 3> qlearn(&reward);
    cout << "Memory used: " << memory_used()-before << "B" << endl;
    cout << "Learning test";
    uint stat, prev_stat = 0;
    for (uint t=0; t < 100; t++) {
        uint act = t % 3;
        stat = prev_stat/5.0 + act/5.0 + (t%3)/5.0 + ((act*act)%3)/5.0 + ((prev_stat*prev_stat*prev_stat)%3)/5.0;
        qlearn.Learn(prev_stat, stat, act, 0.89, 0.8);
        prev_stat = stat;
        if (!(t%15)) cout << ".";
    }
    cout << " OK" << endl;
    qlearn.PrintMatrix();
    return 0;
}
