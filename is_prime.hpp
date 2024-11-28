#ifndef ASYNC_HPP
#define ASYNC_HPP
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <future>

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

int count_primes(std::vector<int> nums, std::string policy = "SYNC_POLICY", int THREAD_COUNT = 4) {
    int count = 0;
    if (policy == "SYNC_POLICY") {
        std::for_each(nums.begin(), nums.end(), [&](int num){
            if(is_prime(num)) {
                ++count;
            }
        });
        return count;
    }
    else if (policy == "ASYNC_POLICY") {
        std::atomic<int> atomic_count(count);
        std::vector<std::future<void>> futures;
        int integer_part = nums.size() / THREAD_COUNT;
        for (int i = 0; i < THREAD_COUNT - 1; i++) {
            futures.push_back(std::async(std::launch::async, [&](std::vector<int> nums) {
                for (auto num : nums) {
                    if (is_prime(num)) {
                        ++atomic_count;
                    }
                }
            },
            std::vector<int>(nums.begin() + (i) * integer_part, nums.begin() + (i+1) * integer_part)));
        }

        futures.push_back(std::async(std::launch::async, [&](std::vector<int> nums) {
            for (auto num : nums) {
                if (is_prime(num)) {
                    ++atomic_count;
                }
            }
        },
        std::vector<int>(nums.begin() + (THREAD_COUNT - 1) * integer_part, nums.begin() + nums.size())));

        for (auto& future : futures) {
            future.get();
        }

        return atomic_count;
    }
    else {
        throw std::invalid_argument("Object not found in pool");
    }
}

#endif //ASYNC_HPP