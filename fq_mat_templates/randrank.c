/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2013 Mike Hansen

******************************************************************************/


#ifdef T

#include "templates.h"

void
TEMPLATE(T, mat_randrank) (TEMPLATE(T, mat_t) mat, flint_rand_t state,
                           slong rank, const TEMPLATE(T, ctx_t) ctx)
{
    slong i;
    TEMPLATE(T, struct) * diag;

    if (rank < 0 || rank > mat->r || rank > mat->c)
    {
        printf("Exception (nmod_mat_randrank). Impossible rank.\n");
        abort();
    }

    diag = _TEMPLATE(T, vec_init) (rank, ctx);
    for (i = 0; i < rank; i++)
        TEMPLATE(T, randtest_not_zero) (diag + i, state, ctx);

    TEMPLATE(T, mat_randpermdiag) (mat, state, diag, rank, ctx);

    _TEMPLATE(T, vec_clear) (diag, rank, ctx);
}


#endif