#include <iostream>
#include <string>
#include <mutex>
#include <JobSystem.h>

namespace TEST2 {
	// 额外用于控制台输出的互斥锁
	std::mutex printMtx;

	void func1() {
		Sleep(700);
		printMtx.lock();
		cout << "【JOB1】Update Translation" << endl;
		std::cout << "work thread:[" << std::this_thread::get_id() << "]" << std::endl << std::endl;
		printMtx.unlock();
	}

	void func2() {
		Sleep(700);
		printMtx.lock();
		cout << "【JOB2】Update Rotation" << endl;
		std::cout << "work thread:[" << std::this_thread::get_id() << "]" << std::endl << std::endl;
		printMtx.unlock();
	}

	void func3() {
		Sleep(700);
		printMtx.lock();
		cout << "【JOB3】Update Scale" << endl;
		std::cout << "work thread:[" << std::this_thread::get_id() << "]" << std::endl << std::endl;
		printMtx.unlock();
	}

	void func4() {
		Sleep(700);
		printMtx.lock();
		cout << "【JOB4】Transform \t 依赖 【JOB1】 【JOB2】 【JOB3】" << endl;
		std::cout << "work thread:[" << std::this_thread::get_id() << "]" << std::endl << std::endl;
		printMtx.unlock();
	}

	void func5() {
		Sleep(700);
		printMtx.lock();
		cout << "【JOB5】Physic \t \t 依赖 【JOB4】" << endl;
		std::cout << "work thread:[" << std::this_thread::get_id() << "]" << std::endl << std::endl;
		printMtx.unlock();
	}

	void func6() {
		Sleep(700);
		printMtx.lock();
		cout << "【JOB6】Render \t \t 依赖 【JOB4】 【JOB5】" << endl;
		std::cout << "work thread:[" << std::this_thread::get_id() << "]" << std::endl << std::endl;
		printMtx.unlock();
	}

	void func7() {
		Sleep(700);
		printMtx.lock();
		cout << "【JOB7】Particle \t 依赖 【JOB4】" << endl;
		std::cout << "work thread:[" << std::this_thread::get_id() << "]" << std::endl << std::endl;
		printMtx.unlock();
	}

	void func8() {
		Sleep(700);
		printMtx.lock();
		cout << "【JOB8】Post \t \t 依赖 【JOB4】" << endl;
		std::cout << "work thread:[" << std::this_thread::get_id() << "]" << std::endl << std::endl;
		printMtx.unlock();
	}


	void testJobSystem()
	{
		std::cout << "|-------------------------- JobSystem Unit Test------------------------------------|" << std::endl;
		std::atomic<unsigned int> head = 0xffffffffff;

		JobSystem s(2);
		s.Init();
		auto j1 = s.SumbitJob(func1, {});
		auto j2 = s.SumbitJob(func2, {});
		auto j3 = s.SumbitJob(func3, {});
		auto j4 = s.SumbitJob(func4, { j1,j2,j3 });
		auto j5 = s.SumbitJob(func5, { j4 });
		auto j6 = s.SumbitJob(func6, { j4,j5 });
		auto j7 = s.SumbitJob(func7, { j4 });
		auto j8 = s.SumbitJob(func8, { j4 });
		s.StartJobs();
		s.WaitJobs();

		std::cout << std::endl << std::endl;
	}
}