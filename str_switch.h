#ifndef STR_SWITCH_H
#define STR_SWITCH_H

#include <cstdint>
#include <cassert>
#include <string>


#define SWITCH(str)  switch(utils::string_switch::str_hash_for_switch(str))
#define CASE(str)    static_assert(utils::string_switch::str_is_correct(str) && (utils::string_switch::str_len(str) <= utils::string_switch::MAX_LEN),\
"CASE string contains wrong characters, or its length is greater than MAX_LEN");\
case utils::string_switch::str_hash(str, utils::string_switch::str_len(str))
#define DEFAULT  default


namespace utils {
namespace string_switch {

    using uchar = uint8_t;
    using ullong = uint64_t;

    constexpr size_t MAX_LEN = 64;
    constexpr ullong N_HASH = static_cast<ullong>(-1);

    constexpr ullong raise_128_to(const size_t power)
    {
        return 1ULL << (7 * (power % 7));
    }

    constexpr bool str_is_correct(const char*  const str)
    {
        return (static_cast<signed char>(*str) > 0) ? str_is_correct(str + 1) : (*str ? false : true);
    }

    constexpr size_t str_len(const char*  str)
    {
        return *str ? (1 + str_len(str + 1)) : 0;
    }

    constexpr ullong str_hash(const char*  str, const size_t current_len)
    {
        return *str ? (raise_128_to(current_len - 1) * static_cast<uchar>(*str) + str_hash(str + 1, current_len - 1)) : 0;
    }

    constexpr ullong str_hash_for_switch(const char*  str)
    {
        return (str_is_correct(str) && (str_len(str) <= MAX_LEN)) ? str_hash(str, str_len(str)) : N_HASH;
    }
}
}


constexpr typename utils::string_switch::ullong operator "" _hash(char const* const str, size_t len)
{
    assert(utils::string_switch::str_is_correct(str) && (len <= utils::string_switch::MAX_LEN) && 
        "CASE string contains wrong characters, or its length is greater than MAX_LEN");
    return utils::string_switch::str_hash(str, len);
}


#endif  // STR_SWITCH_H