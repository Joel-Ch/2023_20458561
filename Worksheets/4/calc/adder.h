#ifndef MATHSLIB_ADDER_H
#define MATHSLIB_ADDER_H

// We need to prefix our function names with an additional
// keyword which is different depending on the operating
// system we are using, and whether we are using or creating the
// library.
// The variables "maths_EXPORTS" must be defined at build time if
// we are building the library, but not if we are using it.
#if defined(WIN32) || defined(WIN32)
    #ifdef maths_STATIC
        // don't add any keywords if building a static library
        #define MATHSLIB_API
    #else
        #ifdef maths_EXPORTS
            #define MATHSLIB_API __declspec(dllexport)
        #else
            #define MATHSLIB_API __declspec(dllimport)
        #endif
    #endif
#else
    // MATHSLIB_API is defined as nothing if we're not on Windows
    #define MATHSLIB_API
#endif

// The above will include one of:
// __declspec(dllexport)
// __declspec(dllimport)
// before declarations. This is a Microsoft specific
// extension to C/C++

// prototype for the function including additional keyword
MATHSLIB_API int add(int a, int b);

#endif
