#ifndef SEALIB__TYPES_H_
#define SEALIB__TYPES_H_
#include <functional>
#include <cstdint>

namespace Sealib {

typedef std::function<void(uint64_t)> const & Consumer;
typedef std::function<void(uint64_t, uint64_t)> const & BiConsumer;

static constexpr uint64_t INVALID = static_cast<uint64_t>(-1);

}  // namespace Sealib

#endif  // SEALIB__TYPES_H_
