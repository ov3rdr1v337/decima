 
#include <cstdint>
#include <random>
#include <string>
#include <type_traits>
#include <vector>
#include <string.h>

#include <benchmark/benchmark.h>

char* uint32_to_string_0
        (
                uint32_t n,
                char    *out_str
        ) noexcept
{
    if ( n < 10u )
    {
        const uint64_t in = n + 0x30ull;
        
        memcpy( out_str, &in, 8u );
        
        return out_str + 1u;
    }

    const uint32_t b = n / 10u;

    if ( n < 100u )
    {
        const uint64_t in = 256ull * n - 2559ull * b + 0x3030ull;

        memcpy( out_str, &in, 8u );

        return out_str + 2u;
    }

    const uint32_t c = n / 100u;

    if ( n < 1000u )
    {
        const uint64_t in = 65536ull * n - 2559ull * ( 256ull * b + c ) + 0x303030ull;

        memcpy( out_str, &in, 8u );

        return out_str + 3u;
    }

    const uint32_t d = n / 1000u;

    if ( n < 10000u )
    {
        const uint64_t in = 16777216ull * n - 2559ull * ( 256ull * ( 256ull * b + c ) + d ) + 0x30303030ull;

        memcpy( out_str, &in, 8u );

        return out_str + 4u;
    }

    const uint32_t e = n / 10000u;

    if ( n < 100000u )
    {
        const uint64_t in = ( ( 16777216ull * n - 2559ull * ( 256ull * ( 256ull * b + c ) + d ) - 10 * e ) << 0x08ull ) + e + 0x3030303030ull;

        memcpy( out_str, &in, 8u );

        return out_str + 5u;
    }

    const uint32_t f = n / 100000u;

    if ( n < 1000000u )
    {
        const uint64_t in = ( ( 16777216ull * n - 2559ull * ( 256ull * ( 256ull * b + c ) + d ) - 10 * e ) << 0x10ull ) +
                            ( ( 256ull      * e - 2559ull * f ) + 0x303030303030ull );

        memcpy( out_str, &in, 8u );

        return out_str + 6u;
    }

    const uint32_t g = n / 1000000u;

    if ( n < 10000000u )
    {
        const uint64_t in = ( ( 16777216ull * n - 2559ull * ( 256ull * ( 256ull * b + c ) + d ) - 10 * e ) << 0x18ull ) +
                            ( ( 65536ull    * e - 2559ull * ( 256ull * f + g ) + 0x30303030303030ull ) );

        memcpy( out_str, &in, 8u );

        return out_str + 7u;
    }

    const uint32_t h = n / 10000000u;

    if ( n < 100000000u )
    {
        const uint64_t in = ( ( 16777216ull * n - 2559ull * ( 256ull * ( 256ull * b + c ) + d ) - 10 * e ) << 0x20ull ) +
                            ( ( 16777216ull * e - 2559ull * ( 256ull * ( 256ull * f + g ) + h ) ) + 0x3030303030303030ull );

        memcpy( out_str, &in, 8u );

        return out_str + 8u;
    }

    const uint32_t x = n / 100000000u;

    const uint64_t r_0 = ( ( 16777216ull * n - 2559ull * ( 256ull * ( 256ull * b + c ) + d ) - 10 * e ) << 0x20ull ) +
                         ( 16777216ull * e - 2559ull * ( 256ull * ( 256ull * f + g ) + h ) - 10 * x );

    if ( 9u < x )
    {
        const uint64_t in_1 = ( ( x % 10u ) << 8ull ) + x / 10u + 0x3030ull;
 
        memcpy( out_str, &in_1, 8u );

        const uint64_t in_2 = ( ( r_0 + 0x3030303030303030ull ) );

        memcpy( out_str + 2u, &in_2, 8u );

        return out_str + 10u;
    }
    else
    {
        const uint64_t in_1 = x   + 0x30u;

        memcpy( out_str, &in_1, 8u );

        const uint64_t in_2 = r_0 + 0x3030303030303030ull;

        memcpy( out_str + 1u, &in_2, 8u );

        return out_str + 9u;
    }
}

template <typename T>
void BM_FastIntToBuffer_1(benchmark::State& state) {
    const int inc = state.range(0);
    char buf[16];
    // Use the unsigned type to increment to take advantage of well-defined
    // modular arithmetic.
    typename std::make_unsigned<T>::type x = 0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(uint32_to_string_0(static_cast<T>(x), buf));
        benchmark::DoNotOptimize(buf);
        x += inc;
    }
}
BENCHMARK_TEMPLATE(BM_FastIntToBuffer_1, int32_t)->Range(0, 1 << 15);
BENCHMARK_TEMPLATE(BM_FastIntToBuffer_1, int64_t)->Range(0, 1 << 30);

const char one_ASCII_final_digits[10][2]
        {
                {'0', 0}, {'1', 0}, {'2', 0}, {'3', 0}, {'4', 0},
                {'5', 0}, {'6', 0}, {'7', 0}, {'8', 0}, {'9', 0},
        };

const char two_ASCII_digits[100][2] =
        {
                {'0', '0'}, {'0', '1'}, {'0', '2'}, {'0', '3'}, {'0', '4'}, {'0', '5'},
                {'0', '6'}, {'0', '7'}, {'0', '8'}, {'0', '9'}, {'1', '0'}, {'1', '1'},
                {'1', '2'}, {'1', '3'}, {'1', '4'}, {'1', '5'}, {'1', '6'}, {'1', '7'},
                {'1', '8'}, {'1', '9'}, {'2', '0'}, {'2', '1'}, {'2', '2'}, {'2', '3'},
                {'2', '4'}, {'2', '5'}, {'2', '6'}, {'2', '7'}, {'2', '8'}, {'2', '9'},
                {'3', '0'}, {'3', '1'}, {'3', '2'}, {'3', '3'}, {'3', '4'}, {'3', '5'},
                {'3', '6'}, {'3', '7'}, {'3', '8'}, {'3', '9'}, {'4', '0'}, {'4', '1'},
                {'4', '2'}, {'4', '3'}, {'4', '4'}, {'4', '5'}, {'4', '6'}, {'4', '7'},
                {'4', '8'}, {'4', '9'}, {'5', '0'}, {'5', '1'}, {'5', '2'}, {'5', '3'},
                {'5', '4'}, {'5', '5'}, {'5', '6'}, {'5', '7'}, {'5', '8'}, {'5', '9'},
                {'6', '0'}, {'6', '1'}, {'6', '2'}, {'6', '3'}, {'6', '4'}, {'6', '5'},
                {'6', '6'}, {'6', '7'}, {'6', '8'}, {'6', '9'}, {'7', '0'}, {'7', '1'},
                {'7', '2'}, {'7', '3'}, {'7', '4'}, {'7', '5'}, {'7', '6'}, {'7', '7'},
                {'7', '8'}, {'7', '9'}, {'8', '0'}, {'8', '1'}, {'8', '2'}, {'8', '3'},
                {'8', '4'}, {'8', '5'}, {'8', '6'}, {'8', '7'}, {'8', '8'}, {'8', '9'},
                {'9', '0'}, {'9', '1'}, {'9', '2'}, {'9', '3'}, {'9', '4'}, {'9', '5'},
                {'9', '6'}, {'9', '7'}, {'9', '8'}, {'9', '9'}
        };

static inline void PutTwoDigits(size_t i, char* buf)
{
    memcpy( buf, two_ASCII_digits[i], 2 );
}

char* uint32_to_string_1
        (
                uint32_t i,
                char    *buffer
        ) noexcept
{
  uint32_t digits;
  // The idea of this implementation is to trim the number of divides to as few
  // as possible, and also reducing memory stores and branches, by going in
  // steps of two digits at a time rather than one whenever possible.
  // The huge-number case is first, in the hopes that the compiler will output
  // that case in one branch-free block of code, and only output conditional
  // branches into it from below.
  if (i >= 1000000000) {     // >= 1,000,000,000
    digits = i / 100000000;  //      100,000,000
    i -= digits * 100000000;
    PutTwoDigits(digits, buffer);
    buffer += 2;
  lt100_000_000:
    digits = i / 1000000;  // 1,000,000
    i -= digits * 1000000;
    PutTwoDigits(digits, buffer);
    buffer += 2;
  lt1_000_000:
    digits = i / 10000;  // 10,000
    i -= digits * 10000;
    PutTwoDigits(digits, buffer);
    buffer += 2;
  lt10_000:
    digits = i / 100;
    i -= digits * 100;
    PutTwoDigits(digits, buffer);
    buffer += 2;
 lt100:
    digits = i;
    PutTwoDigits(digits, buffer);
    buffer += 2;
    *buffer = 0;
    return buffer;
  }

  if (i < 100) {
    digits = i;
    if (i >= 10) goto lt100;
    memcpy(buffer, one_ASCII_final_digits[i], 2);
    return buffer + 1;
  }
  if (i < 10000) {  //    10,000
    if (i >= 1000) goto lt10_000;
    digits = i / 100;
    i -= digits * 100;
    *buffer++ = '0' + digits;
    goto lt100;
  }
  if (i < 1000000) {  //    1,000,000
    if (i >= 100000) goto lt1_000_000;
    digits = i / 10000;  //    10,000
    i -= digits * 10000;
    *buffer++ = '0' + digits;
    goto lt10_000;
  }
  if (i < 100000000) {  //    100,000,000
    if (i >= 10000000) goto lt100_000_000;
    digits = i / 1000000;  //   1,000,000
    i -= digits * 1000000;
    *buffer++ = '0' + digits;
    goto lt1_000_000;
  }
  // we already know that i < 1,000,000,000
  digits = i / 100000000;  //   100,000,000
  i -= digits * 100000000;
  *buffer++ = '0' + digits;
  goto lt100_000_000;
}

template <typename T>
void BM_FastIntToBuffer_2(benchmark::State& state) {
    const int inc = state.range(0);
    char buf[16];
    // Use the unsigned type to increment to take advantage of well-defined
    // modular arithmetic.
    typename std::make_unsigned<T>::type x = 0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(uint32_to_string_1(static_cast<T>(x), buf));
        benchmark::DoNotOptimize(buf);
        x += inc;
    }
}
BENCHMARK_TEMPLATE(BM_FastIntToBuffer_2, int32_t)->Range(0, 1 << 15);
BENCHMARK_TEMPLATE(BM_FastIntToBuffer_2, int64_t)->Range(0, 1 << 30);

BENCHMARK_MAIN();
