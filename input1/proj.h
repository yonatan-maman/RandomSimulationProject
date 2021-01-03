#include <stdio.h>
#include <cstdlib>

int f(int r) {
	int s = 0;
	for(int i=0; i<r; i++) {
		s += (rand() % 100);
	}
	return s;
}

int g() {
	int b = 1;
	while(1) {
		b *= 2;
		if(b > 10000) break;
	}
	return b;
}

void h() {
	int g = (rand() % 100000) + 1;
	int p = (rand() % 100) + 1;
	printf("%d, %d\n", g, p);
	for(int i=0; i<g; i+=p) {
		continue;
	}
	return;
}
