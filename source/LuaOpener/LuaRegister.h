#pragma once
#include <functional>
#include <string>

//获取参数
template<class T>
inline T getArgu(lua_State* l, int index) {
	static_assert("ERROR ARGU TYPE");
	return T();
}

template<>
inline int getArgu<int>(lua_State* l,int index) {
	int num = lua_tonumber(l, index);
	return num;
}

template<>
inline double getArgu<double>(lua_State* l, int index) {
	double num = lua_tonumber(l, index);
	return num;
}

template<>
inline std::string getArgu<std::string>(lua_State* l, int index) {
	auto str = lua_tostring(l, index);
	return str;
}

//包装含返还值的标准C函数
template<class T, typename ...Args>
std::function<int(lua_State*)> packAsLuaFunc(T f(Args...)) {
	return [f](lua_State* l) {
		int index = -1;
		T v = f(getArgu<Args>(l, index--)...);
		lua_pushnumber(l, v);
		return 1;
	};
}
//包装无返还值的标准C函数
template<typename ...Args>
std::function<int(lua_State*)> packAsLuaFunc(void f(Args...)) {
	return [f](lua_State* l) {
		int index = -1;
		f(getArgu<Args>(l, index--)...);
		return 0;
	};
}

//包装含返还值的C++函数对象
template<class T, typename ...Args>
std::function<int(lua_State*)> packAsLuaFunc(std::function<T(Args...)>& f) {
	return [&f](lua_State* l) {
		int index = -1;
		T v = f(getArgu<Args>(l, index--)...);
		lua_pushnumber(l, v);
		return 1;
	};
}

//包装空返还值的C++函数对象
template<typename ...Args>
std::function<int(lua_State*)> packAsLuaFunc(std::function<void(Args...)>& f) {
	return [&f](lua_State* l) {
		int index = -1;
		f(getArgu<Args>(l, index--)...);
		return 0;
	};
}

//通过宏定义先包装成静态C函数。
#define RESISTER_LUA_FUNC(str,func)\
static int str(lua_State* s){\
	return  packAsLuaFunc(func)(s);\
}
