#include <atomic>

using namespace std;

class dekkers_mutex {
public:
    dekkers_mutex() {
        turn.store(0, memory_order_relaxed);
        wants_to_enter = new atomic_bool [2];
        wants_to_enter[0] = false;
        wants_to_enter[1] = false;
    }

    void lock(int tid) {
        wants_to_enter[tid].store(true, memory_order_relaxed);
        while (wants_to_enter[!tid].load(memory_order_relaxed)) {
            if (turn.load(memory_order_relaxed) != tid) {
                wants_to_enter[tid].store(false, memory_order_relaxed);
                while (turn.load(memory_order_relaxed) != tid) {
                }
                wants_to_enter[tid].store(true, memory_order_relaxed);
            }
        }
    }

    void unlock(int tid) {
        turn.store(!tid, memory_order_relaxed);
        wants_to_enter[tid].store(false, memory_order_relaxed);
    }

private:
    atomic_bool *wants_to_enter;
    atomic_int turn = 0;
};