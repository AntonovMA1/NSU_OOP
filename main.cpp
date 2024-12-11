#include <chrono>
#include <numeric>
#include <stdlib.h>
#include <iostream>
#include "is_prime.hpp"

#define THREAD_COUNT 20
#define LIMIT 1000000

int main() {
    std::unique_ptr<Async_policy> async = std::make_unique<Async_policy>();
    std::unique_ptr<Sync_policy> sync = std::make_unique<Sync_policy>();
    Primes_counter counter;
    int primes_count = 0;
    std::vector<int> nums(LIMIT);
    std::iota(nums.begin(), nums.end(), 1);

    auto t1 = std::chrono::high_resolution_clock::now();
    
    primes_count = counter.count(nums, async.get(), THREAD_COUNT);

    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << "Primes: " << primes_count << std::endl;
    std::cout << "Program took: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() <<
    " milliseconds" << std::endl;

    t1 = std::chrono::high_resolution_clock::now();

    primes_count = counter.count(nums, sync.get(), THREAD_COUNT);

    t2 = std::chrono::high_resolution_clock::now();

    std::cout << "Primes: " << primes_count << std::endl;
    std::cout << "Program took: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() <<
    " milliseconds" << std::endl;
    return 0;
}