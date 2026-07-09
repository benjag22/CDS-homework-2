#pragma once

#include <cstdint>

class Testable {
public:
    virtual ~Testable() = default;

    [[nodiscard]] virtual int32_t degree(int32_t u) = 0;
    [[nodiscard]] virtual bool neighbors(int32_t u, int32_t v) = 0;
    [[nodiscard]] virtual uint64_t size_in_bytes() const = 0;
};
