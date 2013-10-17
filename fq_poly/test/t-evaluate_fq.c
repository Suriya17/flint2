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

    Copyright (C) 2012 Sebastian Pancratz 

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "fq_poly.h"

#include "ulong_extras.h"
#include "long_extras.h"

int
main(void)
{
    int i, result;
    flint_rand_t state;

    flint_printf("evaluate_fq... ");
    fflush(stdout);

    flint_randinit(state);

    /* Check aliasing */
    for (i = 0; i < 2000; i++)
    {
        long len;
        fq_ctx_t ctx;

        fq_poly_t f;
        fq_t x, y, z;

        len = n_randint(state, 15) + 1;
        fq_ctx_randtest(ctx, state);
        fq_poly_init(f, ctx);
        fq_init(x, ctx);
        fq_init(y, ctx);
        fq_init(z, ctx);

        fq_poly_randtest(f, state, len, ctx);
        fq_randtest(x, state, ctx);

        fq_set(z, x, ctx);
        fq_poly_evaluate_fq(y, f, x, ctx);
        fq_poly_evaluate_fq(x, f, x, ctx);

        result = (fq_equal(x, y, ctx));
        if (!result)
        {
            flint_printf("FAIL:\n\n");
            flint_printf("f = "), fq_poly_print_pretty(f, "X", ctx), flint_printf("\n");
            flint_printf("x = "), fq_print_pretty(x, ctx), flint_printf("\n");
            flint_printf("y = "), fq_print_pretty(y, ctx), flint_printf("\n");
            flint_printf("z = "), fq_print_pretty(z, ctx), flint_printf("\n");
            abort();
        }

        fq_poly_clear(f, ctx);
        fq_clear(x, ctx);
        fq_clear(y, ctx);
        fq_clear(z, ctx);

        fq_ctx_clear(ctx);
    }

    /* Check (f + g)(a) == f(a) + g(a) */
    for (i = 0; i < 2000; i++)
    {
        long len;
        fq_ctx_t ctx;

        fq_poly_t f, g, h;
        fq_t x, y, z;

        len = n_randint(state, 15) + 1;
        fq_ctx_randtest(ctx, state);
        fq_poly_init(f, ctx);
        fq_poly_init(g, ctx);
        fq_poly_init(h, ctx);
        fq_init(x, ctx);
        fq_init(y, ctx);
        fq_init(z, ctx);

        fq_poly_randtest(f, state, len, ctx);
        fq_poly_randtest(g, state, len, ctx);
        fq_randtest(x, state, ctx);

        fq_poly_add(h, f, g, ctx);
        fq_poly_evaluate_fq(y, f, x, ctx);
        fq_poly_evaluate_fq(z, g, x, ctx);
        fq_add(y, y, z, ctx);
        fq_poly_evaluate_fq(z, h, x, ctx);

        result = (fq_equal(y, z, ctx));
        if (!result)
        {
            flint_printf("FAIL:\n\n");
            flint_printf("f = "), fq_poly_print_pretty(f, "X", ctx), flint_printf("\n");
            flint_printf("g = "), fq_poly_print_pretty(g, "X", ctx), flint_printf("\n");
            flint_printf("h = "), fq_poly_print_pretty(h, "X", ctx), flint_printf("\n");
            flint_printf("x = "), fq_print_pretty(x, ctx), flint_printf("\n");
            flint_printf("y = "), fq_print_pretty(y, ctx), flint_printf("\n");
            flint_printf("z = "), fq_print_pretty(z, ctx), flint_printf("\n");
            abort();
        }

        fq_poly_clear(f, ctx);
        fq_poly_clear(g, ctx);
        fq_poly_clear(h, ctx);
        fq_clear(x, ctx);
        fq_clear(y, ctx);
        fq_clear(z, ctx);

        fq_ctx_clear(ctx);
    }

    flint_randclear(state);
    _fmpz_cleanup();
    flint_printf("PASS\n");
    return EXIT_SUCCESS;
}

