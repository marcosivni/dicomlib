#include <iostream>
#include "Socket.hpp"
using namespace std;
using namespace Network;

int main(int,char**)
{

	try
	{
		ServerSocket server(5555);
		AcceptedSocket a(server);

		cout << TestGetString(a);
		
		TestSendString(a,"hello back");
	}
	catch(std::exception& e)
	{
		cout << e.what() << endl;
	}
	return 0;//will everything clean up nicely?  I hope so...
}
