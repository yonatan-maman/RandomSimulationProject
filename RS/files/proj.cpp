#include "../../libFiles/gStatistics.h"
#include "c2rtl.h"
int _main_original(unsigned char  _a_rs) {
unsigned char a;
long unsigned int sum;
C2RTL_INPUT (unsigned char, a);
sum = 0;
g_statistics::addRunningLoop(__FILE__ ,7,1,"for");
for (int i=0; i<a; i++){
if(g_statistics::plusplus(__FILE__ ,7,1))
break;
sum = sum + i;
}
}
