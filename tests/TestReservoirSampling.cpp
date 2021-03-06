/**
 * @file
 * @brief Tests for StreamMerger
 * @author Manuel Penschuck
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <gtest/gtest.h>
#include <ReservoirSampling.hpp>
#include <vector>
#include <algorithm>

class TestReservoirSampling : public ::testing::Test {};

TEST_F(TestReservoirSampling, Uniformity) {
    constexpr uint64_t elements = 1llu << 28;
    constexpr uint64_t reservoir_size = elements >> 12;
    constexpr uint64_t no_buckets = 32;

    constexpr double avg_samples_in_bucket = reservoir_size / no_buckets;

    ReservoirSampling<uint64_t> res(reservoir_size);

    // sample elements
    for(uint64_t i = 1; i <= elements; i++) {
        res.push(i);
    }

    // compute distribution
    std::vector<uint32_t> bins(no_buckets);
    uint64_t samples = 0;
    constexpr uint64_t bucket_size = (elements + no_buckets - 1) / no_buckets;
    std::sort(res.begin(), res.end());
    uint64_t last_element = 0;

    for(auto it = res.begin(); it != res.end(); ++it, ++samples) {
        EXPECT_GT(*it, last_element);
        ASSERT_LE(*it, elements);

        bins.at((*it-1) / bucket_size)++;
        last_element = *it;
    }

    // verify distribution
    ASSERT_EQ(samples, reservoir_size);
    std::vector<uint32_t> sorted_bins(bins);

    ASSERT_NEAR(sorted_bins.front(), avg_samples_in_bucket, 0.2*avg_samples_in_bucket);
    ASSERT_NEAR(sorted_bins.back(), avg_samples_in_bucket, 0.2*avg_samples_in_bucket);
    ASSERT_NEAR(sorted_bins[sorted_bins.size()/2], avg_samples_in_bucket, 0.05*avg_samples_in_bucket);
}