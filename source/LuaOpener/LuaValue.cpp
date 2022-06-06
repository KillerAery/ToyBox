#include "LuaValue.h"


LuaValue::LuaValue(lua_State* state,int index,const std::string& name):r_state(state),m_index(index),m_name(name){
}

LuaValue::~LuaValue(){
}

int LuaValue::getIndex(){
	return m_index;
}

int LuaValue::asInt() {
	return static_cast<int>(lua_tointeger(r_state, m_index)); 
}

double LuaValue::asDouble() { 
	return lua_tonumber(r_state, m_index); 
}

std::string LuaValue::asString() { 
	return lua_tostring(r_state, m_index); 
}

LuaValue LuaValue::operator[](const std::string & key) {
	if(m_childmap){
		auto itr = m_childmap->find(key);
		//若找到，返还该值在虚拟栈的索引
		if (itr != m_childmap->end() ) {
			return LuaValue(r_state,itr->second.m_index,key);
		}
	}
	//若找不到，则在lua文件读取值放入虚拟栈，并获取索引
	int ret = lua_getfield(r_state, m_index ,key.c_str());
	//ret != 0时，lua文件中存在该值
	if (ret != 0) {
		int index = lua_gettop(r_state);
		createMap();
		m_childmap->insert(std::make_pair(key, LuaValue(r_state, index,key)));
		return LuaValue(r_state, index,key);
	}
	return LuaValue(r_state, 0, key);
}


void LuaValue::pushArg(int arg) {
	lua_pushnumber(r_state, static_cast<double>(arg));
}

void LuaValue::pushArg(double arg) {
	lua_pushnumber(r_state, arg);
}

void LuaValue::pushArg(const std::string& arg) {
	lua_pushstring(r_state, arg.c_str());
}

void LuaValue::pushArg(const char* arg) {
	lua_pushstring(r_state, arg);
}

void LuaValue::clear(){
	m_index = 0;
	clearMap();
}

void LuaValue::createMap() {
	if (m_childmap == nullptr) {
		m_childmap = std::make_unique<std::map<std::string, LuaValue>>();
	}
}

void LuaValue::clearMap() {
	//智能指针置空清理
	m_childmap = nullptr;
}
