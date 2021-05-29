#include <atomic>

using namespace std;

class dekkers_mutex {
public:
    dekkers_mutex() {
        turn.store(0);
        wants_to_enter = new atomic_bool [2];
        wants_to_enter[0] = false;
        wants_to_enter[1] = false;
    }

    void lock(int tid) {
        wants_to_enter[tid].store(true);
        while (wants_to_enter[!tid].load()) {
            if (turn.load() != tid) {
                wants_to_enter[tid].store(false);
                while (turn.load() != tid) {
                }
                wants_to_enter[tid].store(true);
            }
        }
    }

    void unlock(int tid) {
        turn.store(!tid);
        wants_to_enter[tid].store(false);
    }

private:
    atomic_bool *wants_to_enter;
    atomic_int turn = 0;
};
