#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <vector>
#include <numeric>
#include <algorithm>
#include <mutex>

#define THREAD_COUNT 20
#define LIMIT 1000000

bool is_prime(int n) {
    if (n % 2 == 0) {
        return n == 2;
    }
    int i = 3;
    while (i * i <= n && n % i != 0) {
        i += 2;
    }
    return i * i > n;
}

int main() {
    int primes_count = 0;
    std::vector<int> nums(LIMIT);
    std::iota(nums.begin(), nums.end(), 1);
    std::mutex mtx;
    auto t1 = std::chrono::high_resolution_clock::now();

    std::thread threads[THREAD_COUNT];
    std::vector<int> count(THREAD_COUNT);
    int integer_part = LIMIT / THREAD_COUNT;
    for (int i = 0; i < THREAD_COUNT - 1; i++) {
        threads[i] = std::thread([&](std::vector<int> nums) {
            for (auto num : nums) {
                if (is_prime(num)) {
                    std::lock_guard<std::mutex> lock(mtx);
                    ++count[i];
                }
            }
        },
        std::vector<int>(nums.begin() + (i) * integer_part, nums.begin() + (i+1) * integer_part));
    }
    threads[THREAD_COUNT-1] = std::thread([&](std::vector<int> nums) {
            for (auto num : nums) {
                if (is_prime(num)) {
                    std::lock_guard<std::mutex> lock(mtx);
                    count[THREAD_COUNT-1]++;
                }
            }
        },
        std::vector<int>(nums.begin() + (THREAD_COUNT - 1) * integer_part, nums.begin() + LIMIT));

    for (auto& thread : threads) {
        thread.join();
    }

    auto t2 = std::chrono::high_resolution_clock::now();

    for (auto count : count) {
        primes_count += count;
    }
    std::cout << "Primes: " << primes_count << std::endl;
    std::cout << "Program took: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() <<
    " milliseconds" << std::endl;
    
    primes_count = 0;

    t1 = std::chrono::high_resolution_clock::now();

    std::for_each(nums.begin(), nums.end(), [&](int num){
        if(is_prime(num)) {
            ++primes_count;
        }
    });

    t2 = std::chrono::high_resolution_clock::now();

    std::cout << "Primes: " << primes_count << std::endl;
    std::cout << "Program took: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() <<
    " milliseconds" << std::endl;
    return 0;
}