#pragma once

#ifdef __cpp_lib_source_location
#include <source_location>
#define __ORM_LOCATION ORM::UUID::get(std::source_location::current())
#else
#define __ORM_LOCATION ORM::UUID::get(__FILE__, __LINE__)
#endif

namespace ORM {
    class UUID {
    public:
        static constexpr size_t get(const std::string_view filename, const uint_least32_t line) {
            return (hashString(filename) ^ line) * 1099511628211;
        }

#ifdef __cpp_lib_source_location
        static constexpr size_t get(const std::source_location& location = std::source_location::current()) {
            return get(std::string_view(location.file_name()), location.line());
        }
#endif
        template <typename Str>
        static constexpr size_t hashString(const Str& toHash)
        {
            // For this example, I'm requiring size_t to be 64-bit, but you could
            // easily change the offset and prime used to the appropriate ones
            // based on sizeof(size_t).
            static_assert(sizeof(size_t) == 8);
            // FNV-1a 64 bit algorithm
            size_t result = 0xcbf29ce484222325; // FNV offset basis

            for (char c : toHash) {
                result ^= c;
                result *= 1099511628211; // FNV prime
            }

            return result;
        }
        template <size_t N>
        constexpr size_t hashString(char const (&toHash)[N])
        {
            return hashString(std::string_view(toHash));
        }
    };
}