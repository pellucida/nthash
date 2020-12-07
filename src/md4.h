/*
// @(#) md4.h
//
// Original source and license see--
// http://openwall.info/wiki/people/solar/software/public-domain-source-code/md4
// Alexander Peslyak <solar@openwall.com>
// License: BSD
*/
# if	!defined( MD4_H)
# define	MD4_H

# include	<stdint.h>

typedef uint32_t	md4_long_t;

enum	{MD4_DIGEST_SIZE= 16,};
typedef struct md4_ctx  md4_ctx;

int	md4_Create (md4_ctx** ctxp);
int	md4_Delete (md4_ctx** ctxp);
void	md4_init (md4_ctx *ctx);
void	md4_update (md4_ctx *ctx, const void* data, size_t size);
void	md4_final (uint8_t* result, md4_ctx* ctx);

#endif
