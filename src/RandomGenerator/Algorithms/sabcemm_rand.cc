/* Copyright 2017 - BSD-3-Clause
 *
 * Copyright Holder (alphabetical):
 *
 * Beikirch, Maximilian
 * Cramer, Simon
 * Frank, Martin
 * Otte, Philipp
 * Pabich, Emma
 * Trimborn, Torsten
 *
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *    disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *    following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
 *    products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 *//* Copyright 2017 - BSD-3-Clause
 *
 * Copyright Holder (alphabetical):
 *
 * Beikirch, Maximilian
 * Cramer, Simon
 * Frank, Martin
 * Otte, Philipp
 * Pabich, Emma
 * Trimborn, Torsten
 *
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *    disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *    following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
 *    products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#include <cstdint>

#ifndef _SABCEMM_RAND_
#define _SABCEMM_RAND_
namespace SABCEMM_RAND
{
	/* Linear congruential.  */
	#define	TYPE_0		0
	#define	BREAK_0		8
	#define	DEG_0		0
	#define	SEP_0		0

	/* x**7 + x**3 + 1.  */
	#define	TYPE_1		1
	#define	BREAK_1		32
	#define	DEG_1		7
	#define	SEP_1		3

	/* x**15 + x + 1.  */
	#define	TYPE_2		2
	#define	BREAK_2		64
	#define	DEG_2		15
	#define	SEP_2		1

	/* x**31 + x**3 + 1.  */
	#define	TYPE_3		3
	#define	BREAK_3		128
	#define	DEG_3		31
	#define	SEP_3		3

	/* x**63 + x + 1.  */
	#define	TYPE_4		4
	#define	BREAK_4		256
	#define	DEG_4		63
	#define	SEP_4		1
	#define	MAX_TYPES	5	/* Max number of types above.  */

	static int32_t randtbl[DEG_3 + 1] =
	{
		TYPE_3,

		-1726662223, 379960547, 1735697613, 1040273694, 1313901226,
		1627687941, -179304937, -2073333483, 1780058412, -1989503057,
		-615974602, 344556628, 939512070, -1249116260, 1507946756,
		-812545463, 154635395, 1388815473, -1926676823, 525320961,
		-1009028674, 968117788, -123449607, 1284210865, 435012392,
		-2017506339, -911064859, -370259173, 1132637927, 1398500161,
		-205601318,
	};

	struct random_data
	{
		int32_t *fptr, *rptr, *state, rand_type, rand_deg, rand_sep, *end_ptr;
	};

	static struct random_data unsafe_state =
	{
		/* FPTR and RPTR are two pointers into the state info, a front and a rear
		   pointer.  These two pointers are always rand_sep places aparts, as they
		   cycle through the state information.  (Yes, this does mean we could get
		   away with just one pointer, but the code for random is more efficient
		   this way).  The pointers are left positioned as they would be from the call:
		   initstate(1, randtbl, 128);
		   (The position of the rear pointer, rptr, is really 0 (as explained above
		   in the initialization of randtbl) because the state table pointer is set
		   to point to randtbl[1] (as explained below).)  */

		.fptr = &randtbl[SEP_3 + 1],
		.rptr = &randtbl[1],

		/* The following things are the pointer to the state information table,
		   the type of the current generator, the degree of the current polynomial
		   being used, and the separation between the two pointers.
		   Note that for efficiency of random, we remember the first location of
		   the state information, not the zeroth.  Hence it is valid to access
		   state[-1], which is used to store the type of the R.N.G.
		   Also, we remember the last location, since this is more efficient than
		   indexing every time to find the address of the last element to see if
		   the front and rear pointers have wrapped.  */

		.state = &randtbl[1],

		.rand_type = TYPE_3,
		.rand_deg = DEG_3,
		.rand_sep = SEP_3,

		.end_ptr = &randtbl[sizeof (randtbl) / sizeof (randtbl[0])]
	};

	int rand (int32_t *result)
	{
		int32_t *state;

		if ((&unsafe_state) == 0 || result == 0)
			goto fail;

		state = (&unsafe_state)->state;

		if ((&unsafe_state)->rand_type == TYPE_0)
		{
			int32_t val = state[0];
			val = ((state[0] * 1103515245) + 12345) & 0x7fffffff;
			state[0] = val;
			*result = val;
		}
		else
		{
			int32_t *fptr = (&unsafe_state)->fptr;
			int32_t *rptr = (&unsafe_state)->rptr;
			int32_t *end_ptr = (&unsafe_state)->end_ptr;
			int32_t val;

			val = *fptr += *rptr;
			/* Chucking least random bit.  */
			*result = (val >> 1) & 0x7fffffff;
			++fptr;
			if (fptr >= end_ptr)
			{
				fptr = state;
				++rptr;
			}
			else
			{
				++rptr;
				if (rptr >= end_ptr)
					rptr = state;
			}
			(&unsafe_state)->fptr = fptr;
			(&unsafe_state)->rptr = rptr;
		}
		return 0;

		fail:
		//	__set_errno (EINVAL);
			return -1;
	}

	int srand( unsigned int seed )
	{
		int type;
		int32_t *state;
		long int i;
		long int word;
		int32_t *dst;
		int kc;

		if ((&unsafe_state) == 0)
			goto fail;
		type = (&unsafe_state)->rand_type;
		if ((unsigned int) type >= MAX_TYPES)
			goto fail;

		state = (&unsafe_state)->state;
		/* We must make sure the seed is not 0.  Take arbitrarily 1 in this case.  */
		if (seed == 0)
			seed = 1;
		state[0] = (int32_t)seed;
		if (type == TYPE_0)
			goto done;

  		dst = state;
  		word = seed;
  		kc = (&unsafe_state)->rand_deg;
  		for (i = 1; i < kc; ++i)
    		{
      			/* This does:
	   		   state[i] = (16807 * state[i - 1]) % 2147483647;
	 		   but avoids overflowing 31 bits.  */
      			long int hi = word / 127773;
      			long int lo = word % 127773;
      			word = 16807 * lo - 2836 * hi;
      			if (word < 0)
				word += 2147483647;
      			*++dst = static_cast<int32_t>(word);
    		}

  		(&unsafe_state)->fptr = &state[(&unsafe_state)->rand_sep];
  		(&unsafe_state)->rptr = &state[0];
  		kc *= 10;
  		while (--kc >= 0)
    		{
      			int32_t discard;
      			(void) rand ( &discard);
    		}

 		done:
  			return 0;

 		fail:
			return -1;
	}
}


#endif
