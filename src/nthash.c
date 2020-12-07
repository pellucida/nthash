/*
// @(#) nthash.c
//
// Original source and license see--
// https://github.com/eworm-de/nthash#nthash
// Christian Hesse <mail@eworm.de>
// License: GPL
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <locale.h>
#include <langinfo.h>
#include <iconv.h>

#include "md4.h"

enum	{ BUF_SIZE	= 64, };

static 	int	get_line (char* buffer, size_t size){
	int	ch	= EOF;
	size_t	i	= 0;
	while (i < size && (ch=fgetc (stdin))!=EOF && ch != '\n'){
		buffer[i++]	= ch;
	}
	buffer [i] 	= '\0';
	return	i;
}

static	int	print_digest (uint8_t* digest) {
	int	i	= 0;
	for (i = 0; i < MD4_DIGEST_SIZE; i++)
		printf("%02X", digest[i]);
	putchar('\n');
	return	0;
}

static	size_t	convert_utf16le (char* pass, size_t passlen,
				char* utf, size_t utflen) {

	char*		locale	= setlocale (LC_ALL, "");
	iconv_t 	conv	= iconv_open("UTF-16LE", nl_langinfo(CODESET));
	size_t		utfleft	= utflen;

	iconv(conv, &pass, &passlen, &utf, &utfleft);
	return	utflen - utfleft;
}

static	int	md4_hash (char* utf16le, size_t utflen, uint8_t* digest) {
	md4_ctx*	ctx	= 0;

	md4_Create (&ctx);
	md4_update(ctx, (uint8_t*)utf16le, utflen);
	md4_final (digest, ctx);
	md4_Delete (&ctx);
	return 0;
}
int main(int argc, char* argv[]) {

	uint8_t		digest [MD4_DIGEST_SIZE];

	char		buffer	[BUF_SIZE+1];	// +1 nul terminate
	char*		pass	= buffer;
	size_t		passlen	= 0;

	char		utf16le [2 * BUF_SIZE];
	size_t		utflen	= sizeof (utf16le);

	if (argc < 2) {
		get_line (buffer, sizeof(buffer));
		pass	= buffer;
	}
	else	{
		pass	= argv[1];
	}
	passlen	= strlen (pass);

	utflen	= convert_utf16le (pass, passlen, utf16le, utflen);
	
	md4_hash (utf16le, utflen, digest);
	
	print_digest (digest);

	exit (EXIT_SUCCESS);
}
