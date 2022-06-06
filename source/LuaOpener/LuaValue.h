#pragma once
#include <map>
#include <memory>
#include <string>

extern "C"
{
#include "lua.h"  
#include "lauxlib.h"  
#include "lualib.h"  
}

class LuaValue
{
protected:
	int m_index;
	std::string m_name;
	std::shared_ptr<std::map<std::string, LuaValue>> m_childmap;
	lua_State* r_state;
public:
	LuaValue(lua_State* r_state, int index,const std::string& name);
	LuaValue(const LuaValue&) = default;
	LuaValue(LuaValue&&) = default;
	~LuaValue();
public:
	int getIndex();
	//ת����C/C++ intֵ
	int asInt();
	//ת����C/C++ doubleֵ
	double asDouble();
	//ת����C++ stringֵ
	std::string asString();
	//��������
	template <typename... Args>LuaValue call(Args... args);
	//��ȡLuaֵ
	LuaValue operator[](const std::string & key);
	//ע��C����
	template<class T,typename ...Args>
	LuaValue& operator =(T func(Args ...args)) {
		lua_register(r_state,m_name.c_str(),func);
		m_index = lua_getglobal(r_state, m_name.c_str());
		return *this;
	}
	//ע��C����
	template<class T>
	LuaValue operator =(T&& value) {
		pushArg(value);
		lua_setglobal(r_state, m_name.c_str());
		return *this;
	}
	//����
	void clear();
private:
	void createMap();
	void clearMap();
	void pushArg(int arg);
	void pushArg(double arg);
	void pushArg(const std::string& arg);
	void pushArg(const char* arg);
};

template<typename ...Args>
inline LuaValue LuaValue::call(Args ...args) {
	//��ѹ�뺯��
	lua_pushvalue(r_state, m_index);
	//��ѹ�����
	int arr[] = { (0),(pushArg(args),0)... };

	if (lua_pcall(r_state, sizeof...(args), 1, 0) != 0) {
		std::cerr << ("lua_pcall failed: %s\n", lua_tostring(r_state, -1));
		return LuaValue(r_state,0,"_Error");
	}

	int index = lua_gettop(r_state);
	this->createMap();
	return m_childmap->emplace("_FuncCall", LuaValue(r_state, index,"_FuncCall")).first->second;
}