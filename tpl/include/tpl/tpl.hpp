//
// Created by ukheg on 25.07.2016.
//

#ifndef TPL_LIB_H
#define TPL_LIB_H

// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
#define TPL_HELPER_DLL_IMPORT __declspec(dllimport)
#define TPL_HELPER_DLL_EXPORT __declspec(dllexport)
#define TPL_HELPER_DLL_LOCAL
#else
#if __GNUC__ >= 4
#define TPL_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
#define TPL_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
#define TPL_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define TPL_HELPER_DLL_IMPORT
    #define TPL_HELPER_DLL_EXPORT
    #define TPL_HELPER_DLL_LOCAL
#endif
#endif

// Now we use the generic helper definitions above to define TPL_API and TPL_LOCAL.
// TPL_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// TPL_LOCAL is used for non-api symbols.

#ifdef TPL_DLL // defined if TPL is compiled as a DLL
#ifdef TPL_DLL_EXPORTS // defined if we are building the TPL DLL (instead of using it)
    #define TPL_API TPL_HELPER_DLL_EXPORT
  #else
    #define TPL_API TPL_HELPER_DLL_IMPORT
  #endif // TPL_DLL_EXPORTS
  #define TPL_LOCAL TPL_HELPER_DLL_LOCAL
#else // TPL_DLL is not defined: this means TPL is a static lib.
#define TPL_API
#define TPL_LOCAL
#endif // TPL_DLL


#endif //TPL_LIB_TPL_HPP_H
