#include "moar.h"
#include "platform/random.h"
/* Platform specific random numbers. Returns 1 if it succeeded and otherwise 0
 * Does not block */
#if defined(___sun)
   #define MVM_use_getrandom 1
#endif
#if defined(__linux__)
   #include <sys/syscall.h>
   #if defined(SYS_getrandom)
      #define MVM_use_getrandom 1
   #endif
#endif
#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) \
   || defined(__APPLE__)
   #define MVM_use_getentropy 1
#endif

#if defined(MVM_use_getrandom)

   #include <sys/random.h>
   MVMint32 MVM_random64 (MVMThreadContext *tc, MVMuint64 *out) {
       fprintf(stderr, "in here\n");
       return getrandom(out, sizeof(MVMint64), GRND_NONBLOCK) == -1 ? 0 : 1;
   }

#elif defined(MVM_use_getentropy)
   #include <sys/random.h>
   MVMint32 MVM_random64 (MVMThreadContext *tc, MVMuint64 *out) {

      return getentropy(out, sizeof(MVMint64)) < 0 ? 0 : 1;
       /*#include <libstd.h>
       *out = arc4random();
       *out <<= 32;
       *out |= arc4random();
       return 1;*/
   }

#elif defined(_WIN32)
#include <windows.h>
#include <wincrypt.h>
static HCRYPTPROV hCryptProv = 0;
static int win32_urandom_init(void) {
    HINSTANCE hAdvAPI32 = NULL;
    CRYPTACQUIRECONTEXTA pCryptAcquireContext = NULL;

    /* Obtain handle to the DLL containing CryptoAPI. This should not fail. */
    hAdvAPI32 = GetModuleHandle("advapi32.dll");
    if(hAdvAPI32 == NULL)
        return 0;

    /* Obtain pointers to the CryptoAPI functions. This will fail on some early
       versions of Win95. */
    pCryptAcquireContext = (CRYPTACQUIRECONTEXTA)GetProcAddress(
                               hAdvAPI32, "CryptAcquireContextA");
    if (pCryptAcquireContext == NULL)
        return 0;

    pCryptGenRandom = (CRYPTGENRANDOM)GetProcAddress(hAdvAPI32,
                                                     "CryptGenRandom");
    if (pCryptGenRandom == NULL)
        return 0;

    /* Acquire context */
    if (! pCryptAcquireContext(&hCryptProv, NULL, NULL,
                               PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
        return 0;

    return 1;
}
MVMint32 MVM_random64 (MVMThreadContext *tc, MVMuint64 *out) {
   if (!HCRYPTPROV) {
      int rtrn = win32_urandom_init();
      if (!rtrn) return 0;
   }
   if (!pCryptGenRandom(provider, sizeof(MVMint64), out)) {
      return 0;
   }
   return 1;

}

#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) \
   || defined(__APPLE__)
#include <sys/random.h>
MVMint32 MVM_random64 (MVMThreadContext *tc, MVMuint64 *out) {

   getentropy(out, sizeof(MVMint64));
    /*#include <libstd.h>
    *out = arc4random();
    *out <<= 32;
    *out |= arc4random();
    return 1;*/
#else
MVMint32 MVM_random64 (MVMThreadContext *tc, MVMuint64 *out) {
   fprintf(stderr, "ERROR NO RANDOM NUMBER GENERATOR\n");
   return 0;
}
#endif
