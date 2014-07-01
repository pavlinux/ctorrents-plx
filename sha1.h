#ifndef SHA1_H
#define SHA1_H

/*
SHA-1 in C
By Steve Reid <sreid@sea-to-sky.net>
100% Public Domain

-----------------
23 Apr 2001 version from http://sea-to-sky.net/~sreid/
Modified slightly to take advantage of autoconf.
See sha1.c for full history comments.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <inttypes.h>

    typedef struct {
        u_int32_t state[5];
        u_int32_t count[2];
        u_int8_t buffer[64];
    } __attribute__((aligned(4))) SHA1_CTX;

    void SHA1Transform(u_int32_t [5], u_int8_t [64]);
    void SHA1Init(SHA1_CTX*);
    void SHA1Update(SHA1_CTX*, u_int8_t *, u_int32_t); /* JHB */
    void SHA1Final(u_int8_t digest[20], SHA1_CTX *);

#ifdef __cplusplus
}
#endif

#endif

