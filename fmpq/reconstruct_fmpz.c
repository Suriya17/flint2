/*
    Copyright (C) 2011 Fredrik Johansson

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "fmpq.h"

#define ROT(u,v,t)   \
    do { fmpz _t = *u; *u = *v; *v = *t; *t = _t; } while (0);

int
_fmpq_reconstruct_fmpz_2(fmpz_t n, fmpz_t d,
    const fmpz_t a, const fmpz_t m, const fmpz_t N, const fmpz_t D)
{
    fmpz_t q, r, s, t;
    int success = 0;

    /* Quickly identify small integers */
    if (fmpz_cmp(a, N) <= 0)
    {
        fmpz_set(n, a);
        fmpz_one(d);
        return 1;
    }
    fmpz_sub(n, a, m);
    if (fmpz_cmpabs(n, N) <= 0)
    {
        fmpz_one(d);
        return 1;
    }

    fmpz_init(q);
    fmpz_init(r);
    fmpz_init(s);
    fmpz_init(t);

    fmpz_set(r, m); fmpz_zero(s);
    fmpz_set(n, a); fmpz_one(d);

    while (fmpz_cmpabs(n, N) > 0)
    {
    /*
        TODO: the following is correct but might be slow in this loop.
        Set k = bits(n) - bits(N). If k is large, we may
            keep track of d in a 2x2 matrix, and
            chop r and n and use _fmpq_hgcd(r>>j/n>>j) for j ~= k/2.
        This may require backup step(s)!
    */
        fmpz_fdiv_q(q, r, n);
        fmpz_mul(t, q, n); fmpz_sub(t, r, t); ROT(r, n, t);
        fmpz_mul(t, q, d); fmpz_sub(t, s, t); ROT(s, d, t);
    }

    if (fmpz_sgn(d) < 0)
    {
        fmpz_neg(n, n);
        fmpz_neg(d, d);
    }

    if (fmpz_cmp(d, D) <= 0)
    {
        fmpz_gcd(t, n, d);
        success = fmpz_is_one(t);
    }

    fmpz_clear(q);
    fmpz_clear(r);
    fmpz_clear(s);
    fmpz_clear(t);

    return success;
}

int
fmpq_reconstruct_fmpz_2(fmpq_t res, const fmpz_t a, const fmpz_t m,
                                        const fmpz_t N, const fmpz_t D)
{
    return _fmpq_reconstruct_fmpz_2(fmpq_numref(res),
                fmpq_denref(res), a, m, N, D);
}

int
_fmpq_reconstruct_fmpz(fmpz_t n, fmpz_t d,
    const fmpz_t a, const fmpz_t m)
{
    fmpz_t N;
    int result;

    fmpz_init(N);
    fmpz_fdiv_q_2exp(N, m, 1);
    fmpz_sqrt(N, N);
    result = _fmpq_reconstruct_fmpz_2(n, d, a, m, N, N);
    fmpz_clear(N);

    return result;
}

int
fmpq_reconstruct_fmpz(fmpq_t res, const fmpz_t a, const fmpz_t m)
{
    return _fmpq_reconstruct_fmpz(fmpq_numref(res),
                fmpq_denref(res), a, m);
}

