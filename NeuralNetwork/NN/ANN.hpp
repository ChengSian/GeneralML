//
//  ANN.hpp
//  ANN
//
//  Created by Hans on 2017/5/26.
//  Copyright © 2017年 Hans. All rights reserved.
//

#ifndef ANN_hpp
#define ANN_hpp

#include <stdio.h>
#include <cstdarg>

#define RANDOM_WEIGHT

#if defined __unix__ || defined __APPLE__ || defined _WIN32
#include <iostream>
#include <iomanip>
#include <math.h>
#define debug_double(msg, n) {std::cout << std::setprecision(n) << msg;}
#define debug(msg)           {std::cout << msg;}
#define endl                 std::endl
#else
#include <Arduino.h>
#include <math.h>
#define debug_double(msg, n) {debug(msg, n);}
#define debug(msg)           {Serial.print(msg);}
#define endl                 "\n"
#endif

#ifndef uint
#define uint unsigned int
#endif

#ifdef RANDOM_WEIGHT
#define init_weight()  ((rand() % 100) / 60.0 - 1.0)
#else
#define init_weight()  0.01
#endif

#define sigmonid(z)    (1.0/(1.0+exp(-z)))

typedef float ANNType;
typedef uint  ANNClassType;

struct ANN_Node {
    ANNType *weights;
    ANNType error;
    unsigned int size;
};

struct ANN_Layer {
    ANNType *input;
    ANN_Node *nodes;
    uint size;
};

class ANN{
public:
    ANN(uint neural_size, ...);
    ~ANN();
    void InitWeights();
    void Training(ANNType *input, ANNClassType *target, ANNType learnRate, uint epochs=1);
    ANNType* Predict(ANNType *input);
    ANNType*** ExportWeights();
    void LoadWeights(ANNType*** weights);
    uint NetSize();
    void printNet();
    ANNType Error(ANNClassType *output);
    ANNClassType* Classify(ANNType boundary=0.5);
private:
    ANN_Layer* neural;
    uint size;
    uint input;
    uint output;
    ANNType* outputs;
};

#endif /* ANN_hpp */
