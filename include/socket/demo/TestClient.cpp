#include <iostream>

#include "Socket.hpp"

using namespace std;
using namespace Network;

int main(int, char**)
{
	try
	{
		ClientSocket client("localhost",5555);
		string s="hello world";
 		TestSendString(client,s);
		
		s=TestGetString(client);
		cout << s << endl;
	}
	catch(std::exception& e)
	{
		cout << e.what() << endl;
	}
	return 0;
}

