#include "c2rtl.h"
int main() {
unsigned char a;
long unsigned int sum;
C2RTL_INPUT (unsigned char, a);
sum = 0;
for (int i=0; i<a; i++)
        sum = sum + i;
//C2RTL_OUTPUT (long unsigned int, sum);
}
