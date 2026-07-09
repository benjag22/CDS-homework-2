#pragma once

#include <cstdint>

class Testable {
public:
    virtual ~Testable() = default;

    virtual int32_t degree(int32_t u) = 0;
    virtual bool neighbors(int32_t u, int32_t v) = 0;
};
