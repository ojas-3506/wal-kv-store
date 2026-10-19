#include "crc32.hpp"

#include <array>
#include <cstdint>

namespace kv {
namespace {

// Build the 256-entry lookup table for the standard CRC-32 polynomial
// 0xEDB88320 (the bit-reversed form of 0x04C11DB7).
//   For each i in 0..255, simulate shifting i through 8 rounds of the
//   bit-wise CRC algorithm, where the "XOR polynomial" step happens
//   whenever the low bit is set.
constexpr std::array<uint32_t, 256> make_crc_table() {
    std::array<uint32_t, 256> table{};
    constexpr uint32_t kPoly = 0xEDB88320u;
    for (uint32_t i = 0; i < 256; ++i) {
        uint32_t c = i;
        for (int j = 0; j < 8; ++j) {
            c = (c & 1u) ? (c >> 1) ^ kPoly : (c >> 1);
        }
        table[i] = c;
    }
    return table;
}

constexpr auto kCrcTable = make_crc_table();

}  // namespace

// Per-byte update: shift crc right by 8, XOR with table[(crc XOR byte) & 0xFF].
// This is the ONLY place the actual algorithm lives — the rest of the file
// is table generation and init/final XOR wrapping.
uint32_t crc32_update(uint32_t crc, const void* data, size_t len) {
    const uint8_t* bytes = static_cast<const uint8_t*>(data);

    // TODO: walk each byte of `bytes` (length `len`) and update `crc`.
    //   For every byte b:
    //     crc = (crc >> 8) ^ kCrcTable[(crc ^ b) & 0xFF];
    //   Hint: a plain for-loop is enough.
    //
    // Sanity check before moving on:
    //   crc32_update(0xFFFFFFFF, "123456789", 9)  should equal  0xCBF43926
        for (size_t i = 0; i < len; ++i) {
            uint8_t b = bytes[i];
            crc = (crc >> 8) ^ kCrcTable[(crc ^ b) & 0xFF];
        }
        return crc;
}

// Top-level: init with 0xFFFFFFFF, update, final XOR with 0xFFFFFFFF.
uint32_t crc32(const void* data, size_t len) {
    uint32_t crc = 0xFFFFFFFFu;
    crc = crc32_update(crc, data, len);
    return crc ^ 0xFFFFFFFFu;
}

}  // namespace kv
