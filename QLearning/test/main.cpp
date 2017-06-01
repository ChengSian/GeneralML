//
//  main.cpp
//  QLearn
//
//  Created by Hans on 2017/5/27.
//  Copyright © 2017年 Hans. All rights reserved.
//

#include <iostream>


extern int RewardTable();
extern int RewardLambda();


using namespace std;
int main(int argc, const char * argv[]) {
    if (RewardTable()) return 1;
    if (RewardLambda()) return 1;
    return 0;
}
