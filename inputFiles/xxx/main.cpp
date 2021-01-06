#include <iostream>
int main()
{
	INPUT(char,x)
	INPUT(unsigned short , y);
	INPUT(unsigned int , z);

	std::cout << y << "  "<< int(x) <<std::endl;
	for(int i=0;i<10;i++){
		while((--y) != 0)
		{
			//std::cout << y << "  ";
			//	std::cout <<std::endl;

		}
	}
	while((--y)%30 != 0)
	{
		//std::cout << y << "  ";
		//	std::cout <<std::endl;

	}
	while((--z)%20 != 0)
	{
		//std::cout << y << "  ";
		//	std::cout <<std::endl;

	}
	std::cout <<std::endl;
}
