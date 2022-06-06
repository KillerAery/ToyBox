# Toy Box

个人的 C++ 玩具箱，存放一些平时造的玩具轮子：

- Lua Opener：使用 C++ 编写的对 Lua 进一步封装的 Lua Binding 库，方便 C/C++ 与Lua交互。
- Job System：使用 C++ 编写的 Job System。

项目采用 CMake 管理。

# Lua Opener

Lua 版本：5.3.6

C++ 语言：C++ 17

目前功能：
- 支持 string, int, double 3个基本类型
- 访问 lua 变量
- 调用 lua 函数
- lua 调用 C/C++ 函数
- lua 调用 C/C++ 变量

> PS：并不包括 C++ 类与对象与成员函数

## 用法

**直接构造 LuaOpener 对象，然后使用 `loadFile` 读取 lua 文件：**

```cpp
LuaOpener opener;
opener.loadFile("test.lua");
```

**读取 lua 文件的某个变量：**

```cpp
double value =opener["number"].asDouble();
```

**读取 lua 文件的某个 table 里的某个变量（支持嵌套 table）：**

```cpp
std::string value1 = opener["table"]["name"].asString();
std::string value2 = opener["table"]["table2"]["name2"].asString();
```

**读取 lua 文件的某个函数并调用之(支持任意数量不同类型的参数，包括无参数)：**

```cpp
opener["func1"].call(2, 3, "随便打", 4);
opener["func2"].call();
```

**调用 lua 文件的某个函数的返还结果：**

> 只支持一个返还值的 lua 函数

```cpp
int value1 = opener["func1"].call(2, 250, 9, 8).asInt();
double value2 = opener["func2"].call(1.0f,2.0f).asDouble();
```
**向 lua 注册 C/C++ 变量：**

```cpp
opener["cnumber1"] = 2;
opener["cppstring1"] = "test";
```
**向 lua 注册 C/C++ 函数：**

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

**关闭 LuaOpener：**

```cpp
opener.closeFile();
```

# Job System

C++ 语言：C++ 17

目前功能：

- Job 支持依赖关系，即某些 Job 的执行需要依赖另一些 Job 的完成
- （一般用于每帧处理）批量提交 Job，构建 Job 依赖关系
- 支持同步
- 无锁设计

## 用法

**创建 Job System 并初始化：**

```cpp
JobSystem sys(4);	// 开启包含4个线程的 Job System
sys.Init();
```

**提交 Job（但不立即执行）：**

- 第1个参数：`cosnt std::function<void()>& func`
- 第2个参数：`std::list<int>&& ID_of_Parents`

提交 Job 后，会得到本 Job 的 ID，这个 ID 可以用来作为其它 Job 的依赖 ID

```cpp
int job1ID = sys.Sumbit(func1, {});
int job2ID = sys.Sumbit(func2, {});
int job3ID = sys.Sumbit(func3, {job1ID, job2ID});
```

**执行本批次所有提交 Jobs：**

```cpp
sys.StartJobs();
```

**阻塞等待至本批次提交的 Jobs 的完成：**

```cpp
sys.WaitJobs();
```

