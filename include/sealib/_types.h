#ifndef SEALIB__TYPES_H_
#define SEALIB__TYPES_H_
#include <functional>
#include <cstdint>

namespace Sealib {

typedef std::function<void(uint64_t)> Consumer;
typedef std::function<void(uint64_t, uint64_t)> BiConsumer;

static constexpr uint64_t INVALID = static_cast<uint64_t>(-1);

}  // namespace Sealib

/**
 * Macro for constexpr qualifier on member functions
 */
#ifdef __clang__
#define CONSTEXPR_IF_CLANG constexpr
#else
#define CONSTEXPR_IF_CLANG inline
#endif

#endif  // SEALIB__TYPES_H_
