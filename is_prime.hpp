#ifndef ASYNC_HPP
#define ASYNC_HPP
#include <vector>
#include <algorithm>
#include <future>
#include <memory>

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

class Policy {
public:
    virtual int use(std::vector<int>& nums, int THREAD_COUNT = 4) = 0;
};

class Async_policy: public Policy {
    int use(std::vector<int>& nums, int THREAD_COUNT) {
        std::atomic<int> atomic_count(0);
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
};

class Sync_policy: public Policy {
    int use(std::vector<int>& nums, int THREAD_COUNT) {
        int count = 0;
        std::for_each(nums.begin(), nums.end(), [&](int num){
            if(is_prime(num)) {
                ++count;
            }
        });
        return count;
    }
};

class Primes_counter final {
public:
    int count(std::vector<int>& nums, Policy* policy, int THREAD_COUNT = 4) const { //добавить класс policy
        return policy->use(nums, THREAD_COUNT);
    }
};


#endif //ASYNC_HPP