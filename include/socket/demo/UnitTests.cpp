#include <iostream>
#include <boost/static_assert.hpp>
#include "Socket.hpp"
using namespace std;
using namespace Network;

void Test(Socket& socket)
{
	vector<int> v;

	socket.Send<int>(v.begin(),v.end());

}
int main(int, char**)
{
	return 0;
}
