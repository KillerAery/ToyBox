#pragma once
#include <functional>
#include <string>

//��ȡ����
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

//��װ������ֵ�ı�׼C����
template<class T, typename ...Args>
std::function<int(lua_State*)> packAsLuaFunc(T f(Args...)) {
	return [f](lua_State* l) {
		int index = -1;
		T v = f(getArgu<Args>(l, index--)...);
		lua_pushnumber(l, v);
		return 1;
	};
}
//��װ�޷���ֵ�ı�׼C����
template<typename ...Args>
std::function<int(lua_State*)> packAsLuaFunc(void f(Args...)) {
	return [f](lua_State* l) {
		int index = -1;
		f(getArgu<Args>(l, index--)...);
		return 0;
	};
}

//��װ������ֵ��C++��������
template<class T, typename ...Args>
std::function<int(lua_State*)> packAsLuaFunc(std::function<T(Args...)>& f) {
	return [&f](lua_State* l) {
		int index = -1;
		T v = f(getArgu<Args>(l, index--)...);
		lua_pushnumber(l, v);
		return 1;
	};
}

//��װ�շ���ֵ��C++��������
template<typename ...Args>
std::function<int(lua_State*)> packAsLuaFunc(std::function<void(Args...)>& f) {
	return [&f](lua_State* l) {
		int index = -1;
		f(getArgu<Args>(l, index--)...);
		return 0;
	};
}

//ͨ���궨���Ȱ�װ�ɾ�̬C������
#define RESISTER_LUA_FUNC(str,func)\
static int str(lua_State* s){\
	return  packAsLuaFunc(func)(s);\
}
