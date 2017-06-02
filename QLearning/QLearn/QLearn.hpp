//
//  QLearn.hpp
//  QLearn
//
//  Created by Hans on 2017/5/27.
//  Copyright © 2017年 Hans. All rights reserved.
//

#ifndef QLearn_hpp
#define QLearn_hpp

#if defined __unix__ || defined __APPLE__ || defined _WIN32
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <math.h>
#define debug_double(msg, n) {std::cout << std::setprecision(n) << msg;}
#define debug(msg)           {std::cout << msg;}
#define endl                 std::endl
#else
#include <Arduino.h>
#include <math.h>
#define debug_double(msg, n) {Serial.print(msg, n);}
#define debug(msg)           {Serial.print(msg);}
#define endl                 "\n"
#endif

#ifndef uint
#define uint unsigned int
#endif

typedef float LearnType;
typedef LearnType QLearnRewardFunc(uint, uint);

template<typename T>
T rangeMax(T* array, uint start, uint count) {
    T max = array[start];
    for (uint i = start+1; i < start+count; ++i) {
        if ( max < array[i] ) max = array[i];
    }
    return max;
}

template<uint S, uint A>
struct QLearnReward {
    typedef const LearnType RewardType[S][A];
    enum {function, real} type;
    union {
        QLearnRewardFunc* func;
        RewardType* real;
    } e;
    LearnType operator() (uint s, uint a) {
        if (this->type == function) {
            return this->e.func(s, a);
        } else {
            return (*this->e.real)[s][a];
        }
    }
};

template<uint S, uint A>
class QLearn {
public:
    typedef const LearnType RewardType[S][A];
    typedef LearnType QType[S][A];
    QLearn(QLearnRewardFunc* reward) {
        for (uint i = 0; i < S; i++) {
            for (uint j = 0; j < A; j++) {
                this->Q[i][j] = 0;
            }
        }
        this->reward.type = QLearnReward<S, A>::function;
        this->reward.e.func = reward;
    }
    QLearn(RewardType* reward) {
        for (uint i = 0; i < S; i++) {
            for (uint j = 0; j < A; j++) {
                this->Q[i][j] = 0;
            }
        }
        this->reward.type = QLearnReward<S, A>::real;
        this->reward.e.real = reward;
    }
    QLearn(LearnType* Q, QLearnRewardFunc* reward) {
        for (uint i = 0; i < S; i++) {
            for (uint j = 0; j < A; j++) {
                this->Q[i][j] = Q[i*S+j];
            }
        }
        this->reward.type = QLearnReward<S, A>::function;
        this->reward.e.func = reward;
    }
    QLearn(LearnType* Q, RewardType* reward) {
        for (uint i = 0; i < S; i++) {
            for (uint j = 0; j < A; j++) {
                this->Q[i][j] = Q[i*S+j];
            }
        }
        this->reward.type = QLearnReward<S, A>::real;
        this->reward.e.real = reward;
    }
    uint MatrixSize() {
        return S*A;
    }
    QType& getMatrix(){
        return &(this->Q);
    }
    void PrintMatrix(){
        debug("Q :");
        debug("\t");
        for (uint j = 0; j < A; j++) {
            debug("\t");
            debug(j+1);
        }
        debug(endl);
        debug(endl);
        for (uint i = 0; i < S; i++) {
            debug(i+1);
            debug("\t\t");
            for (uint j = 0; j < A; j++) {
                debug_double(this->Q[i][j], 2);
                if (j<A-1) debug("\t");
            }
            debug(endl);
        }
    }
    LearnType Learn(uint status, uint nextStat, uint action, LearnType learnRate, LearnType discount) {
        LearnType reward = this->reward(status, action);
        this->Q[status][action] += learnRate * (reward +
                                                 discount * rangeMax(this->Q[nextStat], 0, A) -
                                                 this->Q[nextStat][action] );
        LearnType sum = 0;
        for (uint i = 0; i < A; i++) {
            if (this->Q[status][i] > 0) {
                sum += this->Q[status][i];
            }
        }
        for (uint i = 0; i < A; i++) {
            if (this->Q[status][i] > 0) {
                this->Q[status][i] /= sum;
            }
        }
        return reward;
    }
    uint GetResult(uint status) {
        LearnType _max = -0.1;
        uint stat = 0;
        for (uint i = 0; i < A; i++) {
            if (_max < this->Q[status][i]) {
                _max = this->Q[status][i];
                stat = i;
            }
        }
        return stat;
    }
private:
    QType Q;
    QLearnReward<S, A> reward;
};

#endif /* QLearn_hpp */
