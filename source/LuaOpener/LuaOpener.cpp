#include "LuaOpener.h"


LuaOpener::LuaOpener():m_state(nullptr){
}

LuaOpener::~LuaOpener(){
	clear();
	closeFile();
}

bool LuaOpener::loadFile(const std::string & filename) {
	//��ȷ���ر�״̬
	closeFile();
	//������״̬
	m_state = luaL_newstate();
	luaL_openlibs(m_state);
	//���ؽű�������
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
	//���ҵ���������ֵ������ջ������
	if (itr != m_stateMap.end() ) {
		return LuaValue(m_state,itr->second.getIndex(),key);
	}
	//���Ҳ���������lua�ļ���ȡֵ��������ջ������ȡ����
	else {
		int ret = lua_getglobal(m_state, key.c_str());
		//ret != 0ʱ��lua�ļ��д��ڸ�ֵ
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

