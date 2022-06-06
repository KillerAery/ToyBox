# LuaOpener

## 介绍

使用 C++ 编写的对 Lua 库的简易封装库，方便 C/C++ 与Lua交互。

依赖 Lua 版本：5.3.6

目前功能：
- 支持 string, int, double 3个基本类型
- 访问 lua 变量
- 调用 lua 函数
- lua 调用 C/C++ 函数
- lua 调用 C/C++ 变量

> PS：并不包括 C++ 类与对象与成员函数

## 用法

**直接构造LuaOpener对象，然后使用loadFile读取lua文件：**

```cpp
LuaOpener opener;
opener.loadFile("test.lua");
```

**读取lua文件的某个变量：**

```cpp
double value =opener["number"].asDouble();
```

**读取lua文件的某个table里的某个变量（支持嵌套table）：**

```cpp
std::string value1 = opener["table"]["name"].asString();
std::string value2 = opener["table"]["table2"]["name2"].asString();
```

**读取lua文件的某个函数并调用之(支持任意数量不同类型的参数，包括无参数)：**

```cpp
opener["func1"].call(2, 3, "随便打", 4);
opener["func2"].call();
```

**调用lua文件的某个函数的返还结果：**

> 只支持一个返还值的 lua 函数

```cpp
int value1 = opener["func1"].call(2, 250, 9, 8).asInt();
double value2 = opener["func2"].call(1.0f,2.0f).asDouble();
```
**向lua注册C/C++变量：**

```cpp
opener["cnumber1"] = 2;
opener["cppstring1"] = "test";
```
**向lua注册C/C++函数：**

首先得在全局位置，利用宏定义RESISTER_LUA_FUNC注册C/C++函数
```cpp
//测试用C函数1
int func1(int a, int b,int c) {
	return a + b + c;
};

//func1_binding是你想要注册的名字，func是你想要注册的C/C++函数
RESISTER_LUA_FUNC(func1_binding, func1);
```
注册完毕，便可在代码区域，再在[]里填写想要让lua识别的名字，直接赋值注册到的函数名字（实际上是另一个C函数指针）

```cpp
opener["cfunc1"] = func1_binding;
```

**关闭LuaOpener：**

```cpp
opener.closeFile();
```

# Job System

## 介绍

## 用法
