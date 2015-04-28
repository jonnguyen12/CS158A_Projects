#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

const int RUNCOUNT = 50;

//X = X(lamba) is our random variable
double X(double lambda) {
	double u;
	do {
		u = double(rand())/RAND_MAX;
	} while (u==0);
	double val = - log(u)*lambda;
	return val;
}

double run (double lambda) {
	int i = 0;
	double time = 0;
	double prevtime = -1;
	double nexttime = 0;
	time = X(lambda);
	nexttime = time + X(lambda);
	//while collision : adjacent times within +/- 1 slot
	while (time - prevtime < 1 || nexttime - time < 1) {
		prevtime = time;
		time = nexttime;
		nexttime += X(lambda);
	}
	return time;
}

using namespace std;
int main (int argc, char * argv[]) {
	int i, N;
	double sum, lambda;
	assert(argc == 2);
	N=atoi(argv[1]);
	for (lambda = 1.0; lambda <= N+0.1; lambda += 0.1) {
		sum = 0;
		for (i=0; i<RUNCOUNT; i++) sum += run(lambda);
		cout << lambda << " " << sum/RUNCOUNT << endl;
	}
	return 0;
}
