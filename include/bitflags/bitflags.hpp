/*
 * Copyright (c) 2020, Marin Peko
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef BITFLAGS_HPP
#define BITFLAGS_HPP

#include <cstdint>
#include <string_view>

namespace bf {

namespace internal {

template <typename T>
struct flag;

/**
 * Bitwise operators overloads
 *
 *     T <op> flag<T>
 */

template <typename T>
[[nodiscard]] constexpr T operator&(T const& lhs, flag<T> const& rhs) noexcept;

template <typename T>
[[nodiscard]] constexpr T operator|(T const& lhs, flag<T> const& rhs) noexcept;

template <typename T>
[[nodiscard]] constexpr T operator^(T const& lhs, flag<T> const& rhs) noexcept;

/**
 * Bitwise operators overloads
 *
 *     <op> flag<T>
 *
 *     flag<T> <op> flag<T>
 */

template <typename T>
[[nodiscard]] constexpr T operator~(flag<T> const& rhs) noexcept;

template <typename T>
[[nodiscard]] constexpr T operator&(flag<T> const& lhs, flag<T> const& rhs) noexcept;

template <typename T>
[[nodiscard]] constexpr T operator|(flag<T> const& lhs, flag<T> const& rhs) noexcept;

template <typename T>
[[nodiscard]] constexpr T operator^(flag<T> const& lhs, flag<T> const& rhs) noexcept;

/**
 * class flag
 *
 * Represents a single flag in a collection of multiple flags.
 * It is for internal use only.
 */
template <typename T = uint8_t>
struct flag {
    T bits;
    std::string_view name;

    constexpr flag() = default;
    constexpr flag(flag&& rhs) = default;
    constexpr flag(flag const& rhs) = default;

    constexpr flag(T bits) noexcept
        : bits(bits)
    {}

    constexpr flag(T bits, std::string_view name) noexcept
        : bits(bits)
        , name(name)
    {}

    flag& operator=(flag&& rhs) = default;
    flag& operator=(flag const& rhs) = default;

    ~flag() = default;

    [[nodiscard]] constexpr operator T() const noexcept {
        return bits;
    }

    [[nodiscard]] constexpr bool operator==(T rhs) const noexcept {
        return bits == rhs;
    }

    [[nodiscard]] constexpr bool operator==(flag const& rhs) const noexcept {
        return bits == rhs.bits &&
               name == rhs.name;
    }

    /**
     * Bitwise operators overloads
     *
     *     T <op> flag<T>
     */

    [[nodiscard]] friend constexpr T operator&(T const& lhs, flag const& rhs) noexcept {
        return lhs & rhs.bits;
    }

    [[nodiscard]] friend constexpr T operator|(T const& lhs, flag const& rhs) noexcept {
        return lhs | rhs.bits;
    }

    [[nodiscard]] friend constexpr T operator^(T const& lhs, flag const& rhs) noexcept {
        return lhs ^ rhs.bits;
    }

    /**
     * Bitwise operators overloads
     *
     *     <op> flag<T>
     *
     *     flag<T> <op> flag<T>
     *
     *     flag<T> <op>= flag<T>
     */

    [[nodiscard]] friend constexpr T operator~(flag const& rhs) noexcept {
        return ~rhs.bits;
    }

    [[nodiscard]] friend constexpr T operator&(flag const& lhs, flag const& rhs) noexcept {
        return lhs.bits & rhs.bits;
    }

    [[nodiscard]] friend constexpr T operator|(flag const& lhs, flag const& rhs) noexcept {
        return lhs.bits | rhs.bits;
    }

    [[nodiscard]] friend constexpr T operator^(flag const& lhs, flag const& rhs) noexcept {
        return lhs.bits ^ rhs.bits;
    }

    constexpr flag& operator&=(flag const& rhs) noexcept {
        bits &= rhs.bits;
        return *this;
    }

    constexpr flag& operator|=(flag const& rhs) noexcept {
        bits |= rhs.bits;
        return *this;
    }

    constexpr flag& operator^=(flag const& rhs) noexcept {
        bits ^= rhs.bits;
        return *this;
    }
};

} // internal

template <typename T>
class bitflags;

/**
 * Bitwise operators overloads
 *
 *     bitflags<T> <op> flag<T>
 */

template <typename T>
[[nodiscard]] constexpr T operator&(bitflags<T> const& lhs, internal::flag<T> const& rhs) noexcept;

template <typename T>
[[nodiscard]] constexpr T operator|(bitflags<T> const& lhs, internal::flag<T> const& rhs) noexcept;

template <typename T>
[[nodiscard]] constexpr T operator^(bitflags<T> const& lhs, internal::flag<T> const& rhs) noexcept;

/**
 * class bitflags
 *
 * Represents a typesafe bitmask flag generator. It can be used as a base class that
 * manages a set of flags. The flags should only be defined for integral types.
 */
template <typename T = uint8_t>
class bitflags {
public:
    using underlying_type = T;

    constexpr bitflags() = default;
    constexpr bitflags(bitflags&& rhs) = default;
    constexpr bitflags(bitflags const& rhs) = default;

    constexpr bitflags(internal::flag<T>&& rhs) noexcept
        : curr_(std::move(rhs))
    {}

    constexpr bitflags(internal::flag<T> const& rhs) noexcept
        : curr_(rhs)
    {}

    constexpr bitflags(T bits) noexcept
        : curr_(bits)
    {}

    constexpr bitflags(T bits, std::string_view name) noexcept
        : curr_(bits, name)
    {}

    bitflags& operator=(bitflags&& rhs) = default;
    bitflags& operator=(bitflags const& rhs) = default;

    bitflags& operator=(T bits) noexcept {
        curr_.bits = bits;
        curr_.name = "";
        return *this;
    }

    bitflags& operator=(internal::flag<T>&& rhs) noexcept {
        curr_ = std::move(rhs);
        return *this;
    }

    bitflags& operator=(internal::flag<T> const& rhs) noexcept {
        curr_ = rhs;
        return *this;
    }

    ~bitflags() = default;

    [[nodiscard]] constexpr operator T() const noexcept {
        return curr_.bits;
    }

    [[nodiscard]] constexpr bool operator==(internal::flag<T> const& rhs) const noexcept {
        return curr_ == rhs;
    }

    /**
     * Bitwise operators overloads
     *
     *     <op> flag<T>
     *
     *     bitflags<T> <op> flag<T>
     *
     *     bitflags<T> <op>= flag<T>
     */

    [[nodiscard]] friend constexpr T operator~(internal::flag<T> const& rhs) noexcept {
        return ~rhs;
    }

    [[nodiscard]] friend constexpr T operator&(bitflags const& lhs, internal::flag<T> const& rhs) noexcept {
        return lhs.curr_ & rhs;
    }

    [[nodiscard]] friend constexpr T operator|(bitflags const& lhs, internal::flag<T> const& rhs) noexcept {
        return lhs.curr_ | rhs;
    }

    [[nodiscard]] friend constexpr T operator^(bitflags const& lhs, internal::flag<T> const& rhs) noexcept {
        return lhs.curr_ ^ rhs;
    }

    constexpr bitflags& operator&=(internal::flag<T> const& rhs) noexcept {
        curr_ &= rhs;
        return *this;
    }

    constexpr bitflags& operator|=(internal::flag<T> const& rhs) noexcept {
        curr_ |= rhs;
        return *this;
    }

    constexpr bitflags& operator^=(internal::flag<T> const& rhs) noexcept {
        curr_ ^= rhs;
        return *this;
    }

    /**
     * Gets an underlying bits of current set of flags.
     *
     * @return Underlying bits
     */
    [[nodiscard]] constexpr T bits() const noexcept {
        return curr_.bits;
    }

    /**
     * Gets an empty set of flags.
     *
     * @return Empty set of flags
     */
    [[nodiscard]] static constexpr T empty() noexcept {
        return T{};
    }

    /**
     * Gets the set of all defined flags.
     *
     * @return Set of all defined flags
     */
    [[nodiscard]] static constexpr T all() noexcept {
        return ~T{};
    }

    /**
     * Checks whether no flag is currently set.
     *
     * @return True if no flag is currently set, otherwise false
     */
    [[nodiscard]] constexpr bool is_empty() const noexcept {
        return curr_ == T{};
    }

    /**
     * Checks whether all flags are currently set.
     *
     * @return True if all flags are currently set, otherwise false
     */
    [[nodiscard]] constexpr bool is_all() const noexcept {
        return curr_ == ~T{};
    }

    /**
     * Checks whether specified flag is contained within the current
     * set of flags. Zero flags are treated as always present.
     *
     * @param rhs Flag to check
     *
     * @return True if the specified flags is contained within the
     *         current set of flags, otherwise false
     */
    [[nodiscard]] constexpr bool contains(internal::flag<T> const& rhs) const noexcept {
        return curr_ & rhs || rhs == empty();
    }

    /**
     * Checks whether all the specified flags are contained within the
     * current set of flags. Zero flags are treated as always present.
     *
     * @param rhs_1 First flag to check
     * @param rhs_n Other flags to check
     *
     * @return True if all the specified flags are contained within the
     *         current set of flags, otherwise false
     */
    template <typename ... U>
    [[nodiscard]] constexpr bool contains(internal::flag<T> const& rhs_1, U const& ... rhs_n) const noexcept {
        return contains(rhs_1) && contains(rhs_n...);
    }

    /**
     * Sets specified flag.
     *
     * @param rhs Flag to be set
     */
    constexpr void set(internal::flag<T> const& rhs) noexcept {
        if (!contains(rhs)) {
            curr_ ^= rhs;
        }
    }

    /**
     * Unsets specified flag.
     *
     * @param rhs Flag to be unset
     */
    constexpr void remove(internal::flag<T> const& rhs) noexcept {
        if (contains(rhs)) {
            curr_ ^= rhs;
        }
    }

    /**
     * Sets specified flag if not already present. Otherwise, unsets the specified flag.
     *
     * @param rhs Flag to be toggled
     */
    constexpr void toggle(internal::flag<T> const& rhs) noexcept {
        curr_ ^= rhs;
    }

    /**
     * Clears all flags currently set.
     */
    constexpr void clear() noexcept {
        curr_ = T{};
    }

protected:
    using flag = internal::flag<T>;

private:
    internal::flag<T> curr_;
};

} // bf

#define BITFLAG(BITS, NAME) \
    static constexpr flag NAME{ BITS, #NAME }

#endif // BITFLAGS_HPP