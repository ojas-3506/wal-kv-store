#include "crc32.hpp"

#include <gtest/gtest.h>

#include <cstring>

namespace {

// The canonical CRC-32 test vector: "123456789" -> 0xCBF43926.
// Every conforming CRC-32 implementation in any language passes this.
constexpr uint32_t kKnownCrc123456789 = 0xCBF43926u;

}  // namespace

TEST(Crc32, EmptyInputIsZero) {
    EXPECT_EQ(kv::crc32(nullptr, 0), 0x00000000u);
    EXPECT_EQ(kv::crc32("", 0), 0x00000000u);
}

TEST(Crc32, KnownVector_123456789) {
    const char* s = "123456789";
    EXPECT_EQ(kv::crc32(s, 9), kKnownCrc123456789);
}

TEST(Crc32, IsIdempotent) {
    const char* s = "the quick brown fox";
    size_t n = std::strlen(s);
    EXPECT_EQ(kv::crc32(s, n), kv::crc32(s, n));
}

TEST(Crc32, SplitUpdateEqualsOneShot) {
    // crc32_update + final-XOR should equal one-shot crc32.
    const char* s = "split me into two";
    size_t n = std::strlen(s);
    size_t mid = n / 2;

    uint32_t crc = 0xFFFFFFFFu;
    crc = kv::crc32_update(crc, s, mid);
    crc = kv::crc32_update(crc, s + mid, n - mid);
    crc ^= 0xFFFFFFFFu;
    EXPECT_EQ(crc, kv::crc32(s, n));
}
