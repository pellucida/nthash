/*
// @(#) md4.c
//
// Original source and license see--
// http://openwall.info/wiki/people/solar/software/public-domain-source-code/md4
// Alexander Peslyak <solar@openwall.com>
// License: BSD
*/

# include	<stdlib.h>
# include	<string.h>

# include	"md4.h"

enum	{MD4_CBLOCK	= 64,};
enum	{MD4_LBLOCK	= MD4_CBLOCK/sizeof(md4_long_t),};


typedef struct md4_ctx {
	md4_long_t	lo, hi;
	md4_long_t	a, b, c, d;
	uint8_t		buffer [MD4_CBLOCK];
//	md4_long_t	block [MD4_LBLOCK];
} md4_ctx;

/*
 * The basic MD4 functions.
 *
 * F and G are optimized compared to their RFC 1320 definitions, with the
 * optimization for F borrowed from Colin Plumb's MD5 implementation.
 */
#define F(x, y, z)			((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z)			(((x) & ((y) | (z))) | ((y) & (z)))
#define H(x, y, z)			((x) ^ (y) ^ (z))

/*
 * The MD4 transformation for all three rounds.
 */
#define STEP(f, a, b, c, d, x, s) \
	(a) += f((b), (c), (d)) + (x); \
	(a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s))));

static inline md4_long_t GET (uint8_t* ptr, size_t n) {

# if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	return	*(md4_long_t *)&ptr[(n) * 4];
# else
	return	__builtin_bswap32 (*(md4_long_t *)&ptr[(n) * 4]);
# endif
}

// data should be 4 byte (md4_long_t)  aligned.
static const void*	body (md4_ctx *ctx, const void *data, size_t size)
{
	uint8_t*	ptr	= (typeof(ptr))data;
	md4_long_t	a	= ctx->a;
	md4_long_t	b	= ctx->b;
	md4_long_t	c	= ctx->c;
	md4_long_t	d	= ctx->d;
const	md4_long_t	ac1	= 0x5a827999;
const	md4_long_t	ac2	= 0x6ed9eba1;

	do {
		md4_long_t	saved_a	= a;
		md4_long_t	saved_b = b;
		md4_long_t	saved_c = c;
		md4_long_t	saved_d = d;

/* Round 1 */
		STEP(F, a, b, c, d, GET(ptr, 0), 3)
		STEP(F, d, a, b, c, GET(ptr, 1), 7)
		STEP(F, c, d, a, b, GET(ptr, 2), 11)
		STEP(F, b, c, d, a, GET(ptr, 3), 19)
		STEP(F, a, b, c, d, GET(ptr, 4), 3)
		STEP(F, d, a, b, c, GET(ptr, 5), 7)
		STEP(F, c, d, a, b, GET(ptr, 6), 11)
		STEP(F, b, c, d, a, GET(ptr, 7), 19)
		STEP(F, a, b, c, d, GET(ptr, 8), 3)
		STEP(F, d, a, b, c, GET(ptr, 9), 7)
		STEP(F, c, d, a, b, GET(ptr, 10), 11)
		STEP(F, b, c, d, a, GET(ptr, 11), 19)
		STEP(F, a, b, c, d, GET(ptr, 12), 3)
		STEP(F, d, a, b, c, GET(ptr, 13), 7)
		STEP(F, c, d, a, b, GET(ptr, 14), 11)
		STEP(F, b, c, d, a, GET(ptr, 15), 19)

/* Round 2 */
		STEP(G, a, b, c, d, GET(ptr, 0) + ac1, 3)
		STEP(G, d, a, b, c, GET(ptr, 4) + ac1, 5)
		STEP(G, c, d, a, b, GET(ptr, 8) + ac1, 9)
		STEP(G, b, c, d, a, GET(ptr, 12) + ac1, 13)
		STEP(G, a, b, c, d, GET(ptr, 1) + ac1, 3)
		STEP(G, d, a, b, c, GET(ptr, 5) + ac1, 5)
		STEP(G, c, d, a, b, GET(ptr, 9) + ac1, 9)
		STEP(G, b, c, d, a, GET(ptr, 13) + ac1, 13)
		STEP(G, a, b, c, d, GET(ptr, 2) + ac1, 3)
		STEP(G, d, a, b, c, GET(ptr, 6) + ac1, 5)
		STEP(G, c, d, a, b, GET(ptr, 10) + ac1, 9)
		STEP(G, b, c, d, a, GET(ptr, 14) + ac1, 13)
		STEP(G, a, b, c, d, GET(ptr, 3) + ac1, 3)
		STEP(G, d, a, b, c, GET(ptr, 7) + ac1, 5)
		STEP(G, c, d, a, b, GET(ptr, 11) + ac1, 9)
		STEP(G, b, c, d, a, GET(ptr, 15) + ac1, 13)

/* Round 3 */
		STEP(H, a, b, c, d, GET(ptr, 0) + ac2, 3)
		STEP(H, d, a, b, c, GET(ptr, 8) + ac2, 9)
		STEP(H, c, d, a, b, GET(ptr, 4) + ac2, 11)
		STEP(H, b, c, d, a, GET(ptr, 12) + ac2, 15)
		STEP(H, a, b, c, d, GET(ptr, 2) + ac2, 3)
		STEP(H, d, a, b, c, GET(ptr, 10) + ac2, 9)
		STEP(H, c, d, a, b, GET(ptr, 6) + ac2, 11)
		STEP(H, b, c, d, a, GET(ptr, 14) + ac2, 15)
		STEP(H, a, b, c, d, GET(ptr, 1) + ac2, 3)
		STEP(H, d, a, b, c, GET(ptr, 9) + ac2, 9)
		STEP(H, c, d, a, b, GET(ptr, 5) + ac2, 11)
		STEP(H, b, c, d, a, GET(ptr, 13) + ac2, 15)
		STEP(H, a, b, c, d, GET(ptr, 3) + ac2, 3)
		STEP(H, d, a, b, c, GET(ptr, 11) + ac2, 9)
		STEP(H, c, d, a, b, GET(ptr, 7) + ac2, 11)
		STEP(H, b, c, d, a, GET(ptr, 15) + ac2, 15)

		a	+= saved_a;
		b	+= saved_b;
		c	+= saved_c;
		d	+= saved_d;

		ptr	+= 64;
	} while ((size -= 64)!=0);

	ctx->a	= a;
	ctx->b	= b;
	ctx->c	= c;
	ctx->d	= d;

	return	ptr;
}

void md4_init(md4_ctx *ctx) {
	ctx->a	= 0x67452301;
	ctx->b	= 0xefcdab89;
	ctx->c	= 0x98badcfe;
	ctx->d	= 0x10325476;

	ctx->lo	= 0;
	ctx->hi	= 0;
}
int	md4_Create (md4_ctx** ctx) {
	int	result	= -1;
	md4_ctx*	new	= calloc (sizeof(*new),1);
	if (new) {
		md4_init (new);
		*ctx	= new;
		result	= 0;
	}
	return	result;
}
int	md4_Delete (md4_ctx** ctxp) {
	if (ctxp) {
		md4_ctx*	ctx	= *ctxp;
		if (ctx) {
			free (ctx);
			*ctxp	= 0;
		}
	}
	return	0;
}

void md4_update(md4_ctx *ctx, const void *data, size_t size) {
	md4_long_t	saved_lo	= ctx->lo;
	size_t		used		= saved_lo & 0x3f;

	if ((ctx->lo = (saved_lo + size) & 0x1fffffff) < saved_lo)
		ctx->hi++;
	ctx->hi += size >> 29;


	if (used) {
		size_t	available = 64 - used;

		if (size < available) {
			memcpy(&ctx->buffer[used], data, size);
			return;
		}

		memcpy (&ctx->buffer[used], data, available);
		data	= (const uint8_t*)data + available;
		size -= available;
		body (ctx, ctx->buffer, 64);
	}

	if (size >= 64) {
		data	= body(ctx, data, size & ~(unsigned long)0x3f);
		size	&= 0x3f;
	}

	memcpy (ctx->buffer, data, size);
}

#define OUT(dst, src) \
	(dst)[0] = (unsigned char)(src); \
	(dst)[1] = (unsigned char)((src) >> 8); \
	(dst)[2] = (unsigned char)((src) >> 16); \
	(dst)[3] = (unsigned char)((src) >> 24);

void md4_final(uint8_t* result, md4_ctx *ctx){
	size_t	used = ctx->lo & 0x3f;
	size_t	available	= (64-1) - used;

	ctx->buffer[used++] = 0x80;

	if (available < 8) {
		memset(&ctx->buffer[used], 0, available);
		body(ctx, ctx->buffer, 64);
		used = 0;
		available = 64;
	}

	memset(&ctx->buffer[used], 0, available - 8);

	ctx->lo <<= 3;
	OUT(&ctx->buffer[56], ctx->lo)
	OUT(&ctx->buffer[60], ctx->hi)

	body(ctx, ctx->buffer, 64);

	OUT(&result[0], ctx->a)
	OUT(&result[4], ctx->b)
	OUT(&result[8], ctx->c)
	OUT(&result[12], ctx->d)

	memset(ctx, 0, sizeof(*ctx));
}

