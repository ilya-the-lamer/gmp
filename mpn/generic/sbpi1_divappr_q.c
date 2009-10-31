/* mpn_sbpi1_divappr_q -- Schoolbook division using the M�ller-Granlund 3/2
   division algorithm, returning approximate quotient.  The quotient returned
   is either correct, or one too large.

   Contributed to the GNU project by Torbjorn Granlund.

   THE FUNCTIONS IN THIS FILE ARE INTERNAL WITH A MUTABLE INTERFACE.  IT IS
   ONLY SAFE TO REACH THEM THROUGH DOCUMENTED INTERFACES.  IN FACT, IT IS
   ALMOST GUARANTEED THAT THEY WILL CHANGE OR DISAPPEAR IN A FUTURE GMP
   RELEASE.

Copyright 2007, 2009 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library.  If not, see http://www.gnu.org/licenses/.  */


#include "gmp.h"
#include "gmp-impl.h"
#include "longlong.h"

mp_limb_t
mpn_sbpi1_divappr_q (mp_ptr qp,
		     mp_ptr np, mp_size_t nn,
		     mp_srcptr dp, mp_size_t dn,
		     mp_limb_t dinv)
{
  mp_limb_t qh;
  mp_size_t qn, i;
  mp_limb_t n1, n0;
  mp_limb_t d1, d0;
  mp_limb_t cy, cy1;
  mp_limb_t q, q0;
  mp_limb_t t1, t0;
  mp_limb_t mask, flag;

  ASSERT (dn > 2);
  ASSERT (nn >= dn);
  ASSERT ((dp[dn-1] & GMP_NUMB_HIGHBIT) != 0);

  np += nn;

  qn = nn - dn;
  if (qn + 1 < dn)
    {
      dp += dn - (qn + 1);
      dn = qn + 1;
    }

  qh = mpn_cmp (np - dn, dp, dn) >= 0;
  if (qh != 0)
    mpn_sub_n (np - dn, np - dn, dp, dn);

  qp += qn;

  dn -= 2;			/* offset dn by 2 for main division loops,
				   saving two iterations in mpn_submul_1.  */
  d1 = dp[dn + 1];
  d0 = dp[dn + 0];

  np -= 2;

  n1 = np[1];

  for (i = qn - (dn + 2); i >= 0; i--)
    {
      np--;
      if (UNLIKELY (n1 == d1) && np[1] == d0)
	{
	  q = GMP_NUMB_MASK;
	  mpn_submul_1 (np - dn, dp, dn + 2, q);
	  n1 = np[1];		/* update n1, last loop's value will now be invalid */
	}
      else
	{
	  umul_ppmm (q, q0, n1, dinv);
	  add_ssaaaa (q, q0, q, q0, n1, np[1]);

	  /* Compute the two most significant limbs of n - q'd */
	  n1 = np[1] - d1 * q;
	  n0 = np[0];
	  sub_ddmmss (n1, n0, n1, n0, d1, d0);
	  umul_ppmm (t1, t0, d0, q);
	  sub_ddmmss (n1, n0, n1, n0, t1, t0);
	  q++;

	  /* Conditionally adjust q and the remainders */
	  mask = - (mp_limb_t) (n1 >= q0);
	  q += mask;
	  add_ssaaaa (n1, n0, n1, n0, mask & d1, mask & d0);

	  if (UNLIKELY (n1 >= d1))
	    {
	      if (n1 > d1 || n0 >= d0)
		{
		  q++;
		  sub_ddmmss (n1, n0, n1, n0, d1, d0);
		}
	    }

	  cy = mpn_submul_1 (np - dn, dp, dn, q);

	  cy1 = n0 < cy;
	  n0 = (n0 - cy) & GMP_NUMB_MASK;
	  cy = n1 < cy1;
	  n1 -= cy1;
	  np[0] = n0;

	  if (UNLIKELY (cy != 0))
	    {
	      n1 += d1 + mpn_add_n (np - dn, np - dn, dp, dn + 1);
	      q--;
	    }
	}

      *--qp = q;
    }

  flag = ~CNST_LIMB(0);

  if (dn >= 0)
    {
      for (i = dn; i > 0; i--)
	{
	  np--;
	  if (UNLIKELY (n1 >= (d1 & flag)))
	    {
	      q = GMP_NUMB_MASK;
	      cy = mpn_submul_1 (np - dn, dp, dn + 2, q);

	      if (UNLIKELY (n1 != cy))
		{
		  if (n1 < (cy & flag))
		    {
		      q--;
		      mpn_add_n (np - dn, np - dn, dp, dn + 2);
		    }
		  else
		    flag = 0;
		}
	      n1 = np[1];
	    }
	  else
	    {
	      umul_ppmm (q, q0, n1, dinv);
	      add_ssaaaa (q, q0, q, q0, n1, np[1]);

	      /* Compute the two most significant limbs of n - q'd */
	      n1 = np[1] - d1 * q;
	      n0 = np[0];
	      sub_ddmmss (n1, n0, n1, n0, d1, d0);
	      umul_ppmm (t1, t0, d0, q);
	      sub_ddmmss (n1, n0, n1, n0, t1, t0);
	      q++;

	      /* Conditionally adjust q and the remainders */
	      mask = - (mp_limb_t) (n1 >= q0);
	      q += mask;
	      add_ssaaaa (n1, n0, n1, n0, mask & d1, mask & d0);

	      if (UNLIKELY (n1 >= d1))
		{
		  if (n1 > d1 || n0 >= d0)
		    {
		      q++;
		      sub_ddmmss (n1, n0, n1, n0, d1, d0);
		    }
		}

	      cy = mpn_submul_1 (np - dn, dp, dn, q);

	      cy1 = n0 < cy;
	      n0 = (n0 - cy) & GMP_NUMB_MASK;
	      cy = n1 < cy1;
	      n1 -= cy1;
	      np[0] = n0;

	      if (UNLIKELY (cy != 0))
		{
		  n1 += d1 + mpn_add_n (np - dn, np - dn, dp, dn + 1);
		  q--;
		}
	    }

	  *--qp = q;

	  /* Truncate operands.  */
	  dn--;
	  dp++;
	}

      np--;
      if (UNLIKELY (n1 >= (d1 & flag)))
	{
	  q = GMP_NUMB_MASK;
	  cy = mpn_submul_1 (np, dp, 2, q);

	  if (UNLIKELY (n1 != cy))
	    {
	      if (n1 < (cy & flag))
		{
		  q--;
		  add_ssaaaa (np[1], np[0], np[1], np[0], dp[1], dp[0]);
		}
	      else
		flag = 0;
	    }
	  n1 = np[1];
	}
      else
	{
	  umul_ppmm (q, q0, n1, dinv);
	  add_ssaaaa (q, q0, q, q0, n1, np[1]);

	  /* Compute the two most significant limbs of n - q'd */
	  n1 = np[1] - d1 * q;
	  n0 = np[0];
	  sub_ddmmss (n1, n0, n1, n0, d1, d0);
	  umul_ppmm (t1, t0, d0, q);
	  sub_ddmmss (n1, n0, n1, n0, t1, t0);
	  q++;

	  /* Conditionally adjust q and the remainders */
	  mask = - (mp_limb_t) (n1 >= q0);
	  q += mask;
	  add_ssaaaa (n1, n0, n1, n0, mask & d1, mask & d0);

	  if (UNLIKELY (n1 >= d1))
	    {
	      if (n1 > d1 || n0 >= d0)
		{
		  q++;
		  sub_ddmmss (n1, n0, n1, n0, d1, d0);
		}
	    }

	  np[0] = n0;
	  np[1] = n1;
	}

      *--qp = q;
    }
  ASSERT_ALWAYS (np[1] == n1);
  np += 2;

  return qh;
}