
#include <atomic>
#include <thread>
#include <array>

using namespace std;

class barrier_object {
public:
    barrier_object() {
        // Probably don't need to do anything here.
    }

    void init(int num_threads) {
        this->num_threads = num_threads;
        this->position = num_threads;
        this->barrier_sense.store(true);
        this->thread_sense.store(false);
        this->thread_senses = new bool[num_threads];
    }

    void barrier(int tid) {
        bool tsense = this->thread_senses[tid];
        if (atomic_fetch_sub(&position, 1)) {
            this->position = this->num_threads.load();
            this->barrier_sense.store(tsense);
        } else {
            while (this->barrier_sense.load() != tsense) {
            }
        }
        this->thread_senses[tid] = !tsense;
    }

//    void barrier_swap_arrays(int tid, double *input, double *output) {
//        bool tsense = this->thread_senses[tid];
//        if (atomic_fetch_sub(&position, 1) == 1) {
//            auto temp = *output;
//            *output = *input;
//            *input = temp;
//            this->position = this->num_threads.load();
//            this->barrier_sense.store(tsense);
//        } else {
//            while (this->barrier_sense.load() != tsense) {
//            }
//        }
//        this->thread_senses[tid] = !tsense;
//    }

private:
    std::atomic_int num_threads;
    std::atomic_int position;
    std::atomic_bool barrier_sense;
    std::atomic_bool thread_sense;
    bool *thread_senses;
};
