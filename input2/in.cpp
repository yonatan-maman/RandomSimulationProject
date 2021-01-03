int main() {
	int x = 1000;
	int y = 2000;
	int z = 3000;
	for(int i=0; i<x; i++) {
		i++;
	}
	int a = 0;
	for(int k=0; k<5000; k+=a) {
		a = rand() % 100;
	}
	int b = 0;
	for(int t=1; t<1000000; t*=b) {
		b = (rand() % 4) + 1;
	}
	return 0;
}
