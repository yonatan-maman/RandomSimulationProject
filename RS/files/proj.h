#include "../../libFiles/gStatistics.h"
#include <cstdlib>
#include <stdio.h>

int f(int r) {
int s = 0;
g_statistics::addRunningLoop(__FILE__ ,6,2,"for");
for(int i=0; i<r; i++) {
if(g_statistics::plusplus(__FILE__ ,6,2))
break;
s += (rand() % 100);
}
return s;
}

int g() {
int b = 1;
g_statistics::addRunningLoop(__FILE__ ,14,2,"while");
while(1) {
if(g_statistics::plusplus(__FILE__ ,14,2))
break;
b *= 2;
if(b > 10000) break;
}
return b;
}

void h() {
int g = (rand() % 100000) + 1;
int p = (rand() % 100) + 1;
printf("%d, %d\n", g, p);
g_statistics::addRunningLoop(__FILE__ ,25,2,"for");
for(int i=0; i<g; i+=p) {
if(g_statistics::plusplus(__FILE__ ,25,2))
break;
continue;
}
return;
}