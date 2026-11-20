#pragma once
#include "vec3.hpp"
#include <source_location>
#include  <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

inline std::vector<CheckFailure> checkFailures;

struct CheckFailure{
    std::source_location loc;
    std::string check, observed, expected;
};        

inline std::vector<CheckFailure> checkFailures;
// FORMATTING:

inline std::string str(long long v) { 
    return std::to_string(v); 
}
inline std::string str(double v) {
    std::ostringstream o;
    o << std::setprecision(17) << v;
    return o.str();
}

inline void recordFailure(std::string check, std::string observed, std::string expected, std::source_location loc) {
    checkFailures.push_back(CheckFailure{loc, std::move(check), std::move(observed), std::move(expected)});
}

#define CHECK_EQ(a, b)                                                               \
    do {                                                                             \
        const long long chk_obs_ = static_cast<long long>(a);                             \
        const long long chk_exp_ = static_cast<long long>(b);                             \
        if (chk_obs_ != chk_exp_) {                                                       \
            recordFailure("CHECK_EQ(" #a ", " #b ")", str(chk_obs_), str(chk_exp_),       \
                          std::source_location::current());                               \
        }                                                                                 \
    } while (0)
#define CHECK_WITH_COND(a, b, cond)                                                          \
    do {                                                                             \                            \
        if (!(cond(a, b))) {                                                          \     \
            recordFailure("CHECK_WITH_COND(" #a ", " #b ", " #cond ")", str(a), str(b), \
                          std::source_location::current());                               \
        }                                                                            \
    } while (0)
#define CHECK_NEAR(a, b, tol)                                                      \
    do {                                                                             \
        const double chk_obs_ = static_cast<double>(a);                                   \
        const double chk_exp_ = static_cast<double>(b);                                   \
        if (!(std::fabs(chk_obs_ - chk_exp_) <= (tol))) {                                 \
            recordFailure("CHECK_NEAR(" #a ", " #b ", " #tol ")", str(chk_obs_),          \
                          str(chk_exp_), std::source_location::current());                \
        }                                                                                 \
    } while (0)


// Helper function for vec3 comparisons
inline void checkVec3Near(const vec3<double>& observed, const vec3<double>& expected, double tol,
                          std::source_location loc = std::source_location::current()) {
    if (!vecmath::roughEquals(observed, expected, tol)) {
        recordFailure("checkVec3Near", observed.str(), expected.str(), loc);
    }
}

inline void checkVec3Equal(const vec3<double>& observed, const vec3<double>& expected,
                           std::source_location loc = std::source_location::current()) {
    if (!(observed.x == expected.x && observed.y == expected.y && observed.z == expected.z)) {
        recordFailure("checkVec3Equal", observed.str(), expected.str(), loc);
    }
}