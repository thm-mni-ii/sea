#ifndef SEALIB__TYPES_H_
#define SEALIB__TYPES_H_
#include <functional>
#include <cstdint>

namespace Sealib {
// the standard numeric type (used mainly for node names)
typedef uint32_t uint;

typedef std::function<void(uint)> Consumer;
typedef std::function<void(uint, uint)> BiConsumer;

static constexpr uint INVALID = static_cast<uint>(-1);
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
