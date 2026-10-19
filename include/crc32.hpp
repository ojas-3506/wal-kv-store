#pragma once
#include <cstddef>
#include <cstdint>

namespace kv {

// Compute CRC-32 over a contiguous buffer.
//   data: pointer to bytes (may be nullptr only if len == 0)
//   len:  number of bytes
// Returns: standard CRC-32 (same polynomial as zlib/gzip/ethernet).
//          crc32(nullptr, 0) == 0x00000000.
uint32_t crc32(const void* data, size_t len);

// Incremental CRC-32 update. Use to CRC a record split across buffers.
//   crc:  current CRC state (start with 0xFFFFFFFF for a fresh record)
//   data: next chunk of bytes
//   len:  number of bytes in this chunk
// Returns: updated CRC state (do NOT final-XOR — only the top-level crc32() does that).
uint32_t crc32_update(uint32_t crc, const void* data, size_t len);

}  // namespace kv
