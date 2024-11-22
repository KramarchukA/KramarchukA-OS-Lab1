

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
using namespace std;

condition_variable cv;
mutex mtx;
int num = 0; 
bool ready = false;

void provide() {
    while (true) {
        unique_lock<mutex> lk(mtx);
        cv.wait(lk, [] { return !ready; });
        ready = true;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        cout << "Generated transmission No " << ++num << "\n";
        cv.notify_one();
    }
}

void consume() {
    while (true) {
        unique_lock<mutex> lk(mtx);
        cv.wait(lk, [] { return ready; });
        ready = false;
        cout << "Received transmission No " << num << "\n";
        cv.notify_one();
    }
}

int main()
{
    thread t1(provide), t2(consume);
    t2.join();
    t1.join();
}

