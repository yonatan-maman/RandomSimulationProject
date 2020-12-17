#include <iostream>
int main()
{
	INPUT(char,x)
	INPUT(long , y);
	
	std::cout << y << "  "<< int(x) <<std::endl;
	y = 10000000000;
	while((--y) != 0)
	{
		//std::cout << y << "  ";
		//	std::cout <<std::endl;

	}
	std::cout <<std::endl;
}
