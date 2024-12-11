#include <gtest/gtest.h>
#include <chrono>
#include <numeric>
#include "../is_prime.hpp"

#define THREAD_COUNT 20
#define LIMIT 1000000

TEST(async_tests, is_prime_test) {
    std::vector<int> nums = {1,2,3,4,5,6,7,8,9,10};
    std::vector<bool> expected = {1,1,1,0,1,0,1,0,0,0};
    std::vector<bool> is_prime_output(10);
    for(int i = 0; i < 10; i++) {
        is_prime_output[i] = is_prime(nums[i]);
    }
    EXPECT_EQ(is_prime_output, expected);
}

TEST(async_tests, small_scale_test) {
    std::unique_ptr<Async_policy> async = std::make_unique<Async_policy>();
    std::unique_ptr<Sync_policy> sync = std::make_unique<Sync_policy>();
    Primes_counter counter;
    int async_primes_count = 0;
    int sync_primes_count = 0;
    std::vector<int> nums(100);
    std::iota(nums.begin(), nums.end(), 1);
    async_primes_count = counter.count(nums, async.get(), THREAD_COUNT);
    sync_primes_count = counter.count(nums, sync.get(), THREAD_COUNT);
    EXPECT_EQ(async_primes_count, sync_primes_count);
    EXPECT_EQ(async_primes_count, 26);
}

TEST(async_tests, large_scale_test) {
    std::unique_ptr<Async_policy> async = std::make_unique<Async_policy>();
    std::unique_ptr<Sync_policy> sync = std::make_unique<Sync_policy>();
    Primes_counter counter;
    int primes_count = 0;
    int expected = 0;
    std::vector<int> nums(LIMIT);
    std::iota(nums.begin(), nums.end(), 1);
    std::for_each(nums.begin(), nums.end(), [&](int num){
        if(is_prime(num)) {
            ++expected;
        }
    }); 
    primes_count = counter.count(nums, async.get(), THREAD_COUNT);
    EXPECT_EQ(primes_count, expected);
}