//
//  main.cpp
//  ANN
//
//  Created by Hans on 2017/5/26.
//  Copyright © 2017年 Hans. All rights reserved.
//

#include <sys/resource.h>
#include <iostream>
#include <cmath>
#include "NN/ANN.hpp"

#define RANGE    1000
#define classify 5
#define LIMIT    1.0
#define regularize(x) (x*(LIMIT/RANGE))

using namespace std;

long getMemoryUsage()
{
    struct rusage usage;
    if(0 == getrusage(RUSAGE_SELF, &usage))
        return usage.ru_maxrss; // bytes
    else
        return 0;
}

template<typename T>
void pushToArray(T in, T* array, size_t size) {
    for (size_t i=1; i<size; i++) {
        array[i] = array[i-1];
    }
    array[0] = in;
}

template<typename T>
void printArray(T* array, size_t size) {
    cout << "[";
    for (size_t i=0; i<size; i++) {
        cout << array[i];
        if (i<size-1) cout << ", ";
    }
    cout << "]" << endl;
}

void myAnswer(ANNType* history, ANNClassType* ans, uint history_num) {
    for (int i=0; i<classify; i++) {
        ans[i] = 0;
    }
    float d = 0;
    float target = 0;
    for (int i=1; i<history_num; i++) {
        d += history[i] - history[i-1];
    }
    d /= history_num;
    target = (history[0]-d) / (LIMIT/classify) ;
    target = (-target+classify)/2;
    if (target >= classify) target = classify-1;
    if (target < 0) target = 0;
    cout << target << endl;
    if (target < 0.6) {
        ans[0] = 1;
    } else if (target < 0.8) {
        ans[0] = 1;
        ans[1] = 1;
    } else if (target < 1.6) {
        ans[1] = 1;
    } else if (target < 1.8) {
        ans[1] = 1;
        ans[2] = 1;
    } else if (target < 2.6) {
        ans[2] = 1;
    } else if (target < 2.8) {
        ans[2] = 1;
        ans[3] = 1;
    } else if (target < 3.6) {
        ans[3] = 1;
    } else if (target < 3.8) {
        ans[3] = 1;
        ans[4] = 1;
    } else {
        ans[4] = 1;
    }
}

int main(int argc, const char * argv[]) {
    long before = getMemoryUsage();
    ANN test(3,  5, 6, 5);
    ANNType history[5];
    ANNClassType train[5];
    cout << "Mem: " << getMemoryUsage()-before << " B" << endl;
    bool learning = true;
    test.printNet();
    cout << endl;
    for (;;) {
        string input;
        cout << "Please input position: ";
        cout.flush();
        cin.clear();
        cin >> input;
        if (input == "q") break;
        if (input == "l") {
            learning = !learning;
            continue;
        }
        ANNType pos = regularize(atof(input.c_str()));
        cout << pos << endl;
        pushToArray(pos, history, 5);
        myAnswer(history, train, 5);
        if (learning) test.Train(history, train, 0.8);
        test.printNet();
        cout << "ANN answer: ";
        printArray(test.Predict(history), 5);
        cout << "My answer: ";
        printArray(train, 5);
        cout << "Class: ";
        printArray(test.Classify(), 5);
        cout << endl;
    }
    cout << endl;
    test.printNet();
    return 0;
}
