#include <iostream.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

const int RUNCOUNT = 100000;

// X = X(lambda) is our random variable
double X(double lambda) {
    double u;
    do {
        u = double(rand()) / RAND_MAX;
    } while (u == 0);
    double val = -log(u) * lambda;
    return val;
    
}

double run(double lambda)
{
    int i = 0;
    double time = 0;
    double prevtime = -1;
    double nexttime = 0;
    time = X(lambda);
    nexttime = time + X(lambda);
    
    //while collision: adjacent times within +/- 1 slot
    while (time - prevtime < 1 || nexttime - time < 1) {
        prevtime = time;
        time = nexttime;
        nexttime += X(lambda);
    }
    return time;

}

void main (int argc, char * argv[]) {
    int i;
    double sum, lambda;
    for (lambda = 1.0; lambda <= 3.01; lambda += 0.1) {
        sum = 0;
        for (i = 0; i < RUNCOUNT; i++) {
            sum += run(lambda);
            cout << lambda << "   " << sum/RUNCOUNT << endl;
        }
    }
}