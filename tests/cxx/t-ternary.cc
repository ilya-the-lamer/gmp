/* Test mp*_class ternary expressions.

Copyright 2001, 2002 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111-1307, USA. */

#include <cstdlib>
#include <iostream>
#include <string>
#include "gmp.h"
#include "gmpxx.h"
#include "gmp-impl.h"
#include "tests.h"

using namespace std;


#define CHECK_GMP(type, message, want)                           \
  do                                                             \
    {                                                            \
      type##_set_str(ref, want, 0);                              \
      if (type##_cmp(val.get_##type##_t(), ref) != 0)            \
        {                                                        \
          cout << "error on " #type "_class operator/function: " \
	       << message << "\n";                               \
          cout << "  want:  " << ref << "\n";                    \
          cout << "  got:   " << val.get_##type##_t() << "\n";   \
          abort();                                               \
        }                                                        \
    }                                                            \
  while (0)

#define CHECK_MPZ(expr, want) CHECK_GMP(mpz, expr, want)
#define CHECK_MPQ(expr, want) CHECK_GMP(mpq, expr, want)
#define CHECK_MPF(expr, want) CHECK_GMP(mpf, expr, want)


void check_mpz(void)
{
  mpz_class val;
  mpz_t ref;
  mpz_init(ref);

  mpz_class z(1), w(2), v(3);

  // mpz_addmul
  val = z + w * v;   CHECK_MPZ("val = z + w * v"  , "7");
  val = z + w * 3u;  CHECK_MPZ("val = z + w * 3u" , "7");
  val = z + 2u * v;  CHECK_MPZ("val = z + 2u * v" , "7");
  val = z + w * 3;   CHECK_MPZ("val = z + w * 3"  , "7");
  val = z + 2 * v;   CHECK_MPZ("val = z + 2 * v"  , "7");
  val = z + w * 3.0; CHECK_MPZ("val = z + w * 3.0", "7");
  val = z + 2.0 * v; CHECK_MPZ("val = z + 2.0 * v", "7");

  val = z * w + v;   CHECK_MPZ("val = z * w + v"  , "5");
  val = z * 2u + v;  CHECK_MPZ("val = z * 2u + v" , "5");
  val = 1u * w + v;  CHECK_MPZ("val = 1u * w + v" , "5");
  val = z * 2 + v;   CHECK_MPZ("val = z * 2 + v"  , "5");
  val = 1 * w + v;   CHECK_MPZ("val = 1 * w + v"  , "5");
  val = z * 2.0 + v; CHECK_MPZ("val = z * 2.0 + v", "5");
  val = 1.0 * w + v; CHECK_MPZ("val = 1.0 * w + v", "5");

  // mpz_submul
  val = z - w * v;   CHECK_MPZ("val = z - w * v"  , "-5");
  val = z - w * 3u;  CHECK_MPZ("val = z - w * 3u" , "-5");
  val = z - 2u * v;  CHECK_MPZ("val = z - 2u * v" , "-5");
  val = z - w * 3;   CHECK_MPZ("val = z - w * 3"  , "-5");
  val = z - 2 * v;   CHECK_MPZ("val = z - 2 * v"  , "-5");
  val = z - w * 3.0; CHECK_MPZ("val = z - w * 3.0", "-5");
  val = z - 2.0 * v; CHECK_MPZ("val = z - 2.0 * v", "-5");

  val = z * w - v;   CHECK_MPZ("val = z * w - v"  , "-1");
  val = z * 2u - v;  CHECK_MPZ("val = z * 2u - v" , "-1");
  val = 1u * w - v;  CHECK_MPZ("val = 1u * w - v" , "-1");
  val = z * 2 - v;   CHECK_MPZ("val = z * 2 - v"  , "-1");
  val = 1 * w - v;   CHECK_MPZ("val = 1 * w - v"  , "-1");
  val = z * 2.0 - v; CHECK_MPZ("val = z * 2.0 - v", "-1");
  val = 1.0 * w - v; CHECK_MPZ("val = 1.0 * w - v", "-1");

  mpz_clear(ref);
}

void check_mpq(void)
{
  mpq_class val;
  mpq_t ref;
  mpq_init(ref);

  // currently there's no ternary mpq operation

  mpq_clear(ref);
}

void check_mpf(void)
{
  mpf_class val;
  mpf_t ref;
  mpf_init(ref);

  // currently there's no ternary mpf operation

  mpf_clear(ref);
}


int main()
{
  tests_start();

  check_mpz();
  check_mpq();
  check_mpf();

  tests_end();
  return 0;
}
