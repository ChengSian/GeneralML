//
//  ANN.cpp
//  ANN
//
//  Created by Hans on 2017/5/26.
//  Copyright © 2017年 Hans. All rights reserved.
//

#include "ANN.hpp"

ANN::ANN(uint neural_size, ...) {
    this->neural = new ANN_Layer[neural_size-1];
    this->size = neural_size;
    va_list num_list;
    va_start(num_list, neural_size);
    uint prev = va_arg(num_list, uint);
    this->input = prev;
    for (uint i = 0; i < neural_size-1; i++) {
        uint now = va_arg(num_list, uint);
        this->neural[i].size = now;
        if (i>0) this->neural[i].input = new ANNType[prev];
        this->neural[i].nodes = new ANN_Node[now];
        for (uint j=0; j < now; j++) {
            this->neural[i].nodes[j].size = prev+1;
            this->neural[i].nodes[j].weights = new ANNType[prev+1];
            for (uint k=0; k < prev+1; k++) {
                this->neural[i].nodes[j].weights[k] = init_weight();
            }
        }
        if (i == neural_size-2) this->output = now;
        prev = now;
    }
    va_end(num_list);
    this->outputs = new ANNType[this->output];
}

ANN::~ANN() {
    for (uint i = 0; i < this->size-1; i++) {
        for (uint j=0; j < this->neural[i].size; j++) {
            delete[] this->neural[i].nodes[j].weights;
        }
        delete[] this->neural[i].nodes;
        if (i>0) delete[] this->neural[i].input;
    }
    delete[] this->neural;
    delete[] this->outputs;
}

uint ANN::NetSize() {
    uint summary = 0;
    for (uint i = 0; i < this->size-1; i++) {
        for (uint j=0; j < this->neural[i].size; j++) {
            summary += this->neural[i].nodes[j].size;
        }
    }
    return summary;
}

void ANN::printNet() {
    for (uint i = 0; i < this->size-1; i++) {
        debug("Layer[");
        debug(i+1);
        debug("]:");
        debug(endl);
        debug("\t");
        for (uint k=0; k < this->neural[i].nodes[0].size; k++) {
            debug(k);
            debug("\t\t");
        }
        debug(endl);
        for (uint j=0; j < this->neural[i].size; j++) {
            debug(j+1);
            debug("\t");
            for (uint k=0; k < this->neural[i].nodes[j].size; k++) {
                debug_double(this->neural[i].nodes[j].weights[k], 2);
                debug("\t\t");
            }
            debug(endl);
        }
    }
}

void ANN::Train(ANNType *input, ANNClassType *target, ANNType learnRate, ANNType lambda) {
    this->Predict(input);
    for (uint j=0; j < this->output; j++) {
        this->neural[this->size-2].nodes[j].error = ((ANNType)target[j] - this->outputs[j]) * this->outputs[j] * (1.0 - this->outputs[j]);
        this->neural[this->size-2].nodes[j].weights[0] += learnRate * this->neural[this->size-2].nodes[j].error + lambda * this->neural[this->size-2].nodes[j].weights[0];
        for (uint k=1; k < this->neural[this->size-2].nodes[j].size; k++) {
            this->neural[this->size-2].nodes[j].weights[k] += learnRate * this->neural[this->size-2].nodes[j].error * this->neural[this->size-2].input[k-1] + lambda * this->neural[this->size-2].nodes[j].weights[k];
        }
    }
    for (int i = this->size-3; i >= 0; i--) {
        for (uint j=0; j < this->neural[i].size; j++) {
            this->neural[i].nodes[j].error = 0;
            for (uint k=0; k < this->neural[i].nodes[j].size; k++) {
                this->neural[i].nodes[j].error += (this->neural[this->size-2].nodes[k].error * this->neural[i].nodes[j].weights[k]);
            }
            this->neural[i].nodes[j].error *= (this->neural[i+1].input[j] * (1 - this->neural[i+1].input[j]));
            this->neural[i].nodes[j].weights[0] += learnRate * this->neural[i].nodes[j].error + lambda * this->neural[i].nodes[j].weights[0];
            for (uint k=1; k < this->neural[i].nodes[j].size; k++) {
                this->neural[i].nodes[j].weights[k] += learnRate * this->neural[i].nodes[j].error * this->neural[i].input[k-1] + lambda * this->neural[i].nodes[j].weights[k];
            }
        }
    }
}

ANNClassType* ANN::Classify() {
    ANNClassType* classify = new ANNClassType[this->output];
    for (uint j=0; j < this->output; j++) {
        if (this->outputs[j] >= 0.5) {
            classify[j] = 1;
        } else {
            classify[j] = 0;
        }
    }
    return classify;
}

ANNType ANN::Error(ANNClassType *output) {
    ANNType e = 0;
    for (uint i=0; i<this->output; i++) {
        e += pow(output[i] - this->outputs[i], 2)/2;
    }
    return e;
}

ANNType ANN::sigmonid(ANNType z) {
    return 1/(1+exp(-z));
}

ANNType* ANN::Predict(ANNType *input) {
    this->neural[0].input = input;
    for (uint i = 0; i < this->size-2; i++) {
        for (uint j=0; j < this->neural[i].size; j++) {
            this->neural[i+1].input[j] = this->neural[i].nodes[j].weights[0];
            for (uint k=1; k < this->neural[i].nodes[j].size; k++) {
                this->neural[i+1].input[j] += this->neural[i].input[k-1] * this->neural[i].nodes[j].weights[k];
            }
            this->neural[i+1].input[j] = this->sigmonid(this->neural[i+1].input[j]);
        }
    }
    for (uint j=0; j < this->output; j++) {
        this->outputs[j] = this->neural[this->size-2].nodes[j].weights[0];
        for (uint k=1; k < this->neural[this->size-2].nodes[j].size; k++) {
            this->outputs[j] += this->neural[this->size-2].input[k-1] * this->neural[this->size-2].nodes[j].weights[k];
        }
        this->outputs[j] = this->sigmonid(this->neural[this->size-2].input[j]);
    }
    return this->outputs;
}
