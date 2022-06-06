#include <iostream>
#include <string> 
#include <LuaOpener.h>

namespace TEST1 {
	using namespace std;

	//测试用C函数1
	int func1(int a, int b, int c) {
		return -a - b - c;
	};

	//测试用C函数2
	void func2(int a, int b) {
		std::cout << a + b << std::endl;
	}

	//测试用C++函数对象3
	std::function<int(int, int, int)> func3 = [](int a, int b, int c) {
		return a + b + c;
	};

	//利用宏定义注册C函数
	RESISTER_LUA_FUNC(func1_binding, func1);
	RESISTER_LUA_FUNC(func2_binding, func2);
	RESISTER_LUA_FUNC(func3_binding, func3);

	void testLuaOpener()
	{
		std::cout << "|-------------------------- LuaOpener Unit Test------------------------------------|" << std::endl;

		LuaOpener opener;
		opener.loadFile("./test.lua");

		cout << opener["number"].asDouble() << endl;
		cout << endl;

		cout << opener["table"]["name"].asString() << endl;
		cout << endl;

		cout << opener["table"]["table2"]["name2"].asString() << endl;
		cout << endl;

		cout << "测试func1：（2, 3, '随便打', 4 ）" << endl;
		opener["func1"].call(2, 3, "随便打", 4);
		cout << endl;

		cout << "测试func2函数结果：（2, 250, 9, 8）" << endl;
		int result = opener["func2"].call(2, 250, 9, 8).asInt();
		cout << "函数结果：" << result << endl;
		cout << endl;

		cout << "测试cnumber1：2" << endl;
		opener["cnumber1"] = 2;
		cout << opener["cnumber1"].asDouble();
		cout << endl << endl;

		cout << "测试cfunc1：（1,3,5）" << endl;
		opener["cfunc1"] = func1_binding;
		cout << opener["cfunc1"].call(1, 3, 5).asDouble();
		cout << endl << endl;

		cout << "测试cfunc2：（2,4）" << endl;
		opener["cfunc2"] = func2_binding;
		opener["cfunc2"].call(2, 4);
		cout << endl << endl;

		cout << "测试cfunc3：（1,3,5）" << endl;
		opener["cfunc3"] = func3_binding;
		cout << opener["cfunc3"].call(1, 3, 5).asDouble();
		cout << endl << endl;

		cout << "测试func3：（）" << endl;
		opener["func3"].call();
		cout << endl;

		opener.closeFile();

		std::cout << std::endl << std::endl;
	}

}