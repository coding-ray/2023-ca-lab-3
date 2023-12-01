typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef long long unsigned int uint64_t;

#define INTERLEAVE_BY_01 0x5555555555555555
#define INTERLEAVE_BY_02 0x3333333333333333
#define INTERLEAVE_BY_04 0x0F0F0F0F0F0F0F0F
#define INTERLEAVE_BY_08 0x00FF00FF00FF00FF
#define INTERLEAVE_BY_16 0x0000FFFF0000FFFF

// return the number of leading zeros in "input in binary"
uint16_t count_leading_zeros(uint64_t x) {
  // fill ones on the right of the first set bit (one)
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  x |= (x >> 16);
  x |= (x >> 32);

  // count ones with population count
  x -= (x >> 1) & INTERLEAVE_BY_01;
  x = ((x >> 2) & INTERLEAVE_BY_02) + (x & INTERLEAVE_BY_02);
  x = ((x >> 4) + x) & INTERLEAVE_BY_04;
  x += x >> 8;
  x += x >> 16;
  x += x >> 32;
  return 64 - (x & 0x7f);
}

uint32_t is_palindrome(uint64_t x) {
  uint64_t n_leading_zero = count_leading_zeros(x);
  // n_bit = number of bits to detect palindrome
  uint64_t n_bit = 64 - n_leading_zero;
  uint64_t is_even = n_bit % 2;

  // a = left half of input
  uint64_t a = x >> (n_bit / 2 + is_even);

  // b = right half of input x
  uint64_t n_left_shift = n_bit / 2 + is_even + (64 - n_bit);
  uint64_t b = (x << n_left_shift) >> n_left_shift;

  // reverse a
  uint64_t r = a;
  r = ((r >> 1) & INTERLEAVE_BY_01) + ((r & INTERLEAVE_BY_01) << 1);
  r = ((r >> 2) & INTERLEAVE_BY_02) + ((r & INTERLEAVE_BY_02) << 2);
  r = ((r >> 4) & INTERLEAVE_BY_04) + ((r & INTERLEAVE_BY_04) << 4);
  r = ((r >> 8) & INTERLEAVE_BY_08) + ((r & INTERLEAVE_BY_08) << 8);
  r = ((r >> 16) & INTERLEAVE_BY_16) + ((r & INTERLEAVE_BY_16) << 16);
  r = (r >> 32) + (r << 32);
  r >>= n_left_shift;

  // return number should be other than 0
  // otherwise, it is ambiguous with 'nothing'
  if (a == b)
    return 1; // palindrome
  else
    return 2; // not palindrome
}

int main() {
  uint64_t test_cases[] = {
      0x0000000000000000,  // palindrome,     return 1
      0x000000000C000003,  // palindrome,     return 1
      0x0000000000000001,  // not palindrome, return 2
      0x0F000000000000F0,  // not palindrome, return 2
  };
  uint32_t results[] = {0, 0, 0, 0};

  // must save results in proper memory location
  // othewise, results will be overwritten by 0 potentially
  for (int i = 0; i < 4; i++) {
      results[i] = is_palindrome(test_cases[i - 1]);
  }

  // write results to improper memory location (code section)
  // right before the program terminates
  for (int i = 1; i <= 4; i++) {
      *(volatile int *) (i * 4) = results[i - 1];
  }

  return 0;
}
