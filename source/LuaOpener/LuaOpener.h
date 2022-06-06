#pragma once
#include <iostream>
#include <map>
#include <string>
#include "LuaValue.h"
#include "LuaRegister.h"

class LuaOpener
{
public:
	lua_State * m_state;
	std::map<std::string, LuaValue> m_stateMap;
public:
	LuaOpener();
	~LuaOpener();
	bool loadFile(const std::string & filename);
	void closeFile();
	LuaValue operator[](const std::string & key);
private:
	void clear();
};