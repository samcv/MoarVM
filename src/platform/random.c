/* OpenBSD getentropy starting 5.6
 * Linux added getrandom to kernel in 3.17 WORKS
 * FreeBSD __FreeBSD_version with revision 331279 (version identifier: 1200061)  Wed Mar 21, 2018
 * https://svnweb.freebsd.org/base?view=revision&revision=r331279
 * FreeBSD Works
 * NetBSD __NetBSD_Version__ <sys/param.h> may not include it
 * Solaris since 11.3
 * OSX since 10.12
*/
/* Platform specific random numbers. Returns 1 if it succeeded and otherwise 0
 * Does not block. Designed for getting small amounts of random data at a time */
#if defined(___sun)
    #define MVM_random_use_getrandom 1
#endif
#if defined(__linux__)
    #include <sys/syscall.h>
    #if defined(SYS_getrandom)
    /* With glibc you are supposed to declare _GNU_SOURCE to use the
     * syscall function */
       #define _GNU_SOURCE
       #define GRND_NONBLOCK 0x01
       #include <unistd.h>
       #define MVM_random_use_getrandom 1
    #else
        #define MVM_random_use_urandom 1
    #endif
#endif
#if defined(__FreeBSD__)
    #include <osreldate.h>
    #if __FreeBSD_version >= 1200061
        #define MVM_random_use_getentropy
    #endif
#endif
#if defined(__OpenBSD__)
    #include <sys/param.h>
    #if OpenBSD >= 201301
        #define MVM_random_use_getentropy
    #endif
#endif
#if defined(__APPLE__) && MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_12
    #define MVM_random_use_getentropy 1
#endif

#include "moar.h"

#if defined(MVM_random_use_getrandom)
/* getrandom was added to glibc much later than it was added to the kernel. Since
 * we detect the presence of the system call to decide whether to use this,
 * just use the syscall instead since the wrapper is not guaranteed to exist.*/
    MVMint32 MVM_getrandom (MVMThreadContext *tc, char *out, size_t size) {
        return syscall(SYS_getrandom, out, size, GRND_NONBLOCK) <= 0 ? 0 : 1;
    }

#elif defined(MVM_random_use_getentropy)
    #include <sys/random.h>
    MVMint32 MVM_getrandom (MVMThreadContext *tc, char *out, size_t size) {
        return getentropy(out, size) < 0 ? 0 : 1;
    }

#elif defined(_WIN32)
    /* The Windows random functions are adapted from those in CPython */
    #include <windows.h>
    #include <wincrypt.h>
    typedef BOOL (WINAPI *CRYPTACQUIRECONTEXTA)(HCRYPTPROV *phProv,\
                  LPCSTR pszContainer, LPCSTR pszProvider, DWORD dwProvType,\
                  DWORD dwFlags );
    typedef BOOL (WINAPI *CRYPTGENRANDOM)(HCRYPTPROV hProv, DWORD dwLen,\
                  BYTE *pbBuffer );

    static HCRYPTPROV hCryptContext = 0;
    static CRYPTGENRANDOM pCryptGenRandom = NULL;
    static int win32_urandom_init(void) {
        HINSTANCE hAdvAPI32 = NULL;
        CRYPTACQUIRECONTEXTA pCryptAcquireContext = NULL;

        /* Obtain handle to the DLL containing CryptoAPI. This should not fail. */
        hAdvAPI32 = GetModuleHandle("advapi32.dll");
        if(hAdvAPI32 == NULL)
            return 0;

        /* Obtain pointers to the CryptoAPI functions. This will fail on some early
           versions of Win95. */
        pCryptAcquireContext =
            (CRYPTACQUIRECONTEXTA)GetProcAddress(hAdvAPI32, "CryptAcquireContextA");
        if (pCryptAcquireContext == NULL)
            return 0;

        pCryptGenRandom = (CRYPTGENRANDOM)GetProcAddress(hAdvAPI32, "CryptGenRandom");
        if (pCryptGenRandom == NULL)
            return 0;

        /* Get the pCrypt Context */
        if (!pCryptAcquireContext(&hCryptContext, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
            return 0;

        return 1;
    }
    MVMint32 MVM_getrandom (MVMThreadContext *tc, char *out, size_t size) {
        if (!hCryptContext) {
            int rtrn = win32_urandom_init();
            if (!rtrn) return 0;
        }
        if (!pCryptGenRandom(hCryptContext, (DWORD)size, (BYTE*)out)) {
            return 0;
        }
        return 1;
    }
#else
    #include <unistd.h>
    MVMint32 MVM_getrandom (MVMThreadContext *tc, char *out, size_t size) {
        int fd = open("/dev/urandom", O_RDONLY);
        size_t size = sizeof(MVMint64);
        int n = 0;
        fprintf(stderr, "FALLBACK\n");
        if (fd < 0)
            return 0;
        n = read(fd, out, size);
        close(fd);
        if (n <= 0)
            return 0;
        return 1;
    }
#endif
