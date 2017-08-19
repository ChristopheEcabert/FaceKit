/**
 *  @file   library_export.hpp
 *  @brief  Export macro for dll
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   30/07/16
 *  Copyright © 2016 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_LIBRARY_EXPORT__
#define __FACEKIT_LIBRARY_EXPORT__

#if (defined WIN32 || defined _WIN32 || defined WINCE)
  #define FUNC_NAME __FUNCTION__
  #if defined FACEKIT_API_EXPORTS
    #define FK_EXPORTS __declspec(dllexport)
    #if _MSC_VER && !__INTEL_COMPILER
      /* disable unknown pragma warnings */
      #pragma warning (disable : 4068 )
    #endif
  #else
    #define FK_EXPORTS __declspec(dllimport)
  #endif
#else
  #define FK_EXPORTS
  #define FUNC_NAME __PRETTY_FUNCTION__
#endif

#endif /* __FACEKIT_LIBRARY_EXPORT__ */
