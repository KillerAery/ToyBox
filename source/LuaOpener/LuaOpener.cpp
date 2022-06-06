#include "LuaOpener.h"


LuaOpener::LuaOpener():m_state(nullptr){
}

LuaOpener::~LuaOpener(){
	clear();
	closeFile();
}

bool LuaOpener::loadFile(const std::string & filename) {
	//先确保关闭状态
	closeFile();
	//开启新状态
	m_state = luaL_newstate();
	luaL_openlibs(m_state);
	//加载脚本并运行
	if (luaL_loadfile(m_state,filename.c_str()) || lua_pcall(m_state, 0, 0, 0)) {
		std::clog << "load Lua script failed: " << lua_tostring(m_state, -1) ;
		return false;
	}
	return true;
}

void LuaOpener::closeFile(){
	if (m_state) {
		clear();
		lua_close(m_state);
		m_state = nullptr;
	}
}

LuaValue LuaOpener::operator[](const std::string & key){
	auto itr = m_stateMap.find(key);
	//若找到，返还该值在虚拟栈的索引
	if (itr != m_stateMap.end() ) {
		return LuaValue(m_state,itr->second.getIndex(),key);
	}
	//若找不到，则在lua文件读取值放入虚拟栈，并获取索引
	else {
		int ret = lua_getglobal(m_state, key.c_str());
		//ret != 0时，lua文件中存在该值
		if (ret != 0){
			int index = lua_gettop(m_state);
			m_stateMap.insert(std::make_pair(key, LuaValue(m_state, index, key)));
			return LuaValue(m_state, index, key);
		}
	}
	return LuaValue(m_state, 0, key);
}

void LuaOpener::clear(){
	for (auto & value : m_stateMap)
		value.second.clear();
	m_stateMap.clear();
}

