#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <cstdlib>
#include <ctime>
using namespace std;
const int WIDTH = 100;
const int HEIGHT = 30;
const int SPEED = 1;
const int DELAY_MS = 100;
mutex printMutex;
class Car {
public:
    string name;
    int x, y;
    int targetX, targetY;
    Car(string name, int startX, int startY)
        : name(name), x(startX), y(startY) {
        targetX = (WIDTH / 2) + rand() % ((WIDTH / 2) + 1);
        targetY = (HEIGHT / 2) + rand() % ((HEIGHT / 2) + 1);
    }
    void move() {
        if (x == targetX && y == targetY) {
            lock_guard<mutex> lock(printMutex);
            cout << name << " вже на (" << x << ", " << y << ")" << endl;
            return;
        }
        while (x != targetX || y != targetY) {
            {
                lock_guard<mutex> lock(printMutex);
                cout << name << " на (" << x << ", " << y << ") їде до ("
                    << targetX << ", " << targetY << ")" << endl;
            }
            if (x < targetX) x += SPEED;
            else if (x > targetX) x -= SPEED;
            if (y < targetY) y += SPEED;
            else if (y > targetY) y -= SPEED;
            this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
        }
        lock_guard<mutex> lock(printMutex);
        cout << name << " приїхала в (" << x << ", " << y << ")" << endl;
    }
};
int main() {
    setlocale(LC_CTYPE, "ukr");
    srand(time(0));
    vector<Car> cars;
    cars.emplace_back("Машина 1", 0, 0);
    cars.emplace_back("Машина 2", 5, 10);
    cars.emplace_back("Машина 3", 10, 5);
    vector<thread> threads;
    for (auto& c : cars) {
        threads.emplace_back(&Car::move, &c);
    }
    for (auto& th : threads) th.join();
    return 0;
}