#ifndef GRPC_EXAMPLE_PERFORMANCE_H
#define GRPC_EXAMPLE_PERFORMANCE_H

#include <cstdint>
#include <deque>
#include <numeric>
#include <set>
#include <string>
#include <utility>

#include "LogHelper.h"

#define COUNT_FOR_PRINT_PERFORMANCE 10000u

class Performance {
  public:
    explicit Performance() = default;

    void setName(std::string name) { name_ = std::move(name); }

    void add(uint64_t timeDifference) {
        time_diff_set_.insert(timeDifference);
        if (time_diff_set_.size() > COUNT_FOR_PRINT_PERFORMANCE) {
            uint64_t time_diff_average =
                std::accumulate(time_diff_set_.begin(), time_diff_set_.end(), uint64_t(0u)) /
                time_diff_set_.size();
            LOG(INFO) << "[performance]name: " << name_ << ", average: " << time_diff_average
                      << ", min: " << *time_diff_set_.begin() << ", max: " << *time_diff_set_.rbegin();
            time_diff_set_.clear();
        }
        LOG(DEBUG) << "[performance]time difference: " << timeDifference;
    }

  private:
    std::set<uint64_t> time_diff_set_{};
    std::string name_{};
};

#endif  // GRPC_EXAMPLE_PERFORMANCE_H
