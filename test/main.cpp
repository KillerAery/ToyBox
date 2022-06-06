#include <Windows.h>
#include "testJobSystem.h"
#include "testLuaOpener.h"

using namespace std;

int main()
{
	TEST1::testLuaOpener();
	TEST2::testJobSystem();
	system("pause"); 
	return 0;
}
