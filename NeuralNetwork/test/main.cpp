//
//  main.cpp
//  QLearn
//
//  Created by Hans on 2017/5/27.
//  Copyright © 2017年 Hans. All rights reserved.
//

#include <iostream>
#include "Memory.h"
#include <NN/ANN.hpp>


using namespace std;
int main(int argc, const char * argv[]) {
	srand(0);
	volatile size_t before = memory_used();
    ANN test(3,  2, 2, 1);
    ANNType input[2] = {0, 0};
    ANNClassType training[1];
    test.InitWeights();
    cout << "Memory used: " << memory_used()-before << "B" << endl;
    test.printNet();
    size_t t = 30000;
    cout << "XOR tranning test ";
    while (t--) {
        training[0] = (bool)(input[0]) ^ (bool)(input[1]);
        test.Training(input, training, 0.1);
        input[t % 2] = !input[t % 2];
        if (!(t%1000)) cout << ".";
    }
    cout << " Done." << endl;
    test.printNet();
    cout << "XOR predict test:" << endl;
    t = 4;
    while (t--) {
    	training[0] = (bool)(input[0]) ^ (bool)(input[1]);
        cout << (bool)(input[0]) << " " << (bool)(input[1])
        	 << " [" << training[0] << "] : ";
        cout << (test.Predict(input)[0]) << "\tError: " << test.Error(training)*100.0 << "%" << endl;
        input[t % 2] = !input[t % 2];
    }
    cout << "Export and load weights tests .";
    ANNType*** w = test.ExportWeights();
    test.LoadWeights(w);
    cout << ".. Done" << endl;
    return 0;
}
