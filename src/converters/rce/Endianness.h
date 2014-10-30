#ifndef ENDIANNESS_H_
#define ENDIANNESS_H_

#define ENDIANNESS_K_UNKNOWN    0
#define ENDIANNESS_K_LITTLE     1
#define ENDIANNESS_K_BIG        2

/*
#if   defined(__LITTLE_ENDIAN__) && (__BIG_ENDIAN__)

    #    undef ENDIANNESS_IS_LITTLE
    #    undef ENDIANNESS_IS_BIG

    #error "Endianness.h:found both __BIG_ENDIAN__ and __LITTLE_ENDIAN defined"

#else

#if   defined(__i386) || defined(i386) || defined(__IEEE_LITTLE_ENDIAN) || (__LITTLE_ENDIAN_)


    #   define ENDIANNESS_IS_LITTLE ENDIANNESS_K_LITTLE
    #   define ENDIANNESS           ENDIANNESS_K_LITTLE


  #elif defined(__sparc) || defined(__IEEE_BIG_ENDIAN) || (__BIG_ENDIAN__)

    #    define ENDIANNESS_IS_BIG    ENDIANNESS_K_BIG
    #    define ENDIANNESS           ENDIANNESS_K_BIG

  #else

    #    define ENDIANNESS           ENDIANNESS_K_UNKNOWN

    #    undef ENDIANNESS_IS_LITTLE
    #    undef ENDIANNESS_IS_BIG

    #    error "Endianness.h: Cannot determine the endianness of the machine"

   #endif
#endif
*/

  #     define ENDIANNESS_IS_LITTLE ENDIANNESS_K_LITTLE
  #     define ENDIANNESS           ENDIANNESS_K_LITTLE


#endif










