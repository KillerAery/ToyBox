#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <thread>
#include <string>
#include <future>
#include <condition_variable>
#include <mutex>
#include <Windows.h>
#include <JobSystem.h>

using namespace std;


// ¶îÍâÓÃÓÚ¿ØÖÆÌ¨Êä³öµÄ»¥³âËø
std::mutex printMtx;

void func1() {
	Sleep(700);
	printMtx.lock();
	cout << "¡¾JOB1¡¿Update Translation" << endl;
	std::cout << "work thread:[" << std::this_thread::get_id() << "]" << std::endl << std::endl;
	printMtx.unlock();
}
void func2() {
	Sleep(700);
	printMtx.lock();
	cout << "¡¾JOB2¡¿Update Rotation" << endl;
	std::cout << "work thread:[" << std::this_thread::get_id() << "]" << std::endl << std::endl;
	printMtx.unlock();
}
void func3() {
	Sleep(700);
	printMtx.lock();
	cout << "¡¾JOB3¡¿Update Scale" << endl;
	std::cout << "work thread:[" << std::this_thread::get_id() << "]" << std::endl << std::endl;
	printMtx.unlock();
}
void func4() {
	Sleep(700);
	printMtx.lock();
	cout << "¡¾JOB4¡¿Transform \t ÒÀÀµ ¡¾JOB1¡¿ ¡¾JOB2¡¿ ¡¾JOB3¡¿" << endl;
	std::cout << "work thread:[" << std::this_thread::get_id() << "]" << std::endl << std::endl;
	printMtx.unlock();
}
void func5() {
	Sleep(700);
	printMtx.lock();
	cout << "¡¾JOB5¡¿Physic \t \t ÒÀÀµ ¡¾JOB4¡¿" << endl;
	std::cout << "work thread:[" << std::this_thread::get_id() << "]" << std::endl << std::endl;
	printMtx.unlock();
}
void func6() {
	Sleep(700);
	printMtx.lock();
	cout << "¡¾JOB6¡¿Render \t \t ÒÀÀµ ¡¾JOB4¡¿ ¡¾JOB5¡¿" << endl;
	std::cout << "work thread:[" << std::this_thread::get_id() << "]" << std::endl << std::endl;
	printMtx.unlock();
}
void func7() {
	Sleep(700);
	printMtx.lock();
	cout << "¡¾JOB7¡¿Particle \t ÒÀÀµ ¡¾JOB4¡¿" << endl;
	std::cout << "work thread:[" << std::this_thread::get_id() << "]" << std::endl << std::endl;
	printMtx.unlock();
}
void func8() {
	Sleep(700);
	printMtx.lock();
	cout << "¡¾JOB8¡¿Post \t \t ÒÀÀµ ¡¾JOB4¡¿" << endl;
	std::cout << "work thread:[" << std::this_thread::get_id() << "]" << std::endl << std::endl;
	printMtx.unlock();
}



int main()
{
	std::atomic<unsigned int> head = 0xffffffffff;

	JobSystem s(2);
	s.Init();
	auto j1 = s.SumbitJob(func1, {});
	auto j2 = s.SumbitJob(func2, {});
	auto j3 = s.SumbitJob(func3, {});
	auto j4 = s.SumbitJob(func4, {j1,j2,j3});
	auto j5 = s.SumbitJob(func5, {j4});
	auto j6 = s.SumbitJob(func6, {j4,j5});
	auto j7 = s.SumbitJob(func7, {j4});
	auto j8 = s.SumbitJob(func8, { j4 });
	s.StartJobs();
	s.WaitJobs();
	system("pause"); 
	return 0;
}
