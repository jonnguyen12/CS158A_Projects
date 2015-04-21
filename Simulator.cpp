#define USAGE "ether N"
// Simulates N ethernet stations all trying to transmit at once;
// returns average # of slot times until one station succeeds.

#include <iostream>
#include <stdlib.h>
#include <assert.h>

#define MAX 1000 /* max # of stations */

class station {
public: 
	void reset() { _NextAttempt = _CollisionCount =0; }
	bool transmit(int T) {return _NextAttempt == T;}
	void collide() { //updates sation after a collision
		_CollisionCount ++;
		_NextAttempt += 1 + backoff(_CollisionCount);
			//the 1 above is for the current slot
	}
private:
	int _NextAttempt;
	int _CollisionCount;
	static int backoff (int k) {
	//choose random number 0..2^k-1; 1e choose k random bits
		unsigned short r = rand();
		unsigned short mask = 0xFFFF >> (16-k); //mask = 2^k-1
		return int (r & mask);
	}
};

station S[MAX];
//run does a single simulation
//it returns the time at which some entrant transmits
int run (int N) {
	int time = 0;
	int i;
	for (i = 0; i < N; i++) { S[1].reset();}
	while (1) {
		int count = 0; //# of attempts at this time
		int j = -1; //count the # of attempts; save j as index of one of them
		for (i = 0; i < N; i++) {
			if (S[i].transmit(time)) {j=1; ++count;}
		}
		if (count ==1) //we are done
			return time;
		else if (count > 1) { //collisions occured
			for (i=0;i<N;i++) {
				if (S[i].transmit(time)) S[i].collide();
			}
		}
		++time;
	}
}

int RUNCOUNT = 10000;
using namespace std;
int main(int argc, char * argv[]) {
	int N, i, runsum =0;
	assert(argc == 2);
	N=atoi(argv[1]);
	assert(N<MAX);
	for (i=0;i<RUNCOUNT;i++) runsum += run(N);
	cout << "runsum = " << runsum
		<< " RUNCOUNT= " << RUNCOUNT
		<< " average: " << ((double)runsum)/RUNCOUNT << endl;;
	return 0;
}
