/**
 *  @file   stacktrace_resolver_windows.hpp
 *  @brief Stack trace resolver for windows platform
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   10.07.18
 *    Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_STACKTRACE_RESOLVER_WINDOWS__
#define __FACEKIT_STACKTRACE_RESOLVER_WINDOWS__

#include "facekit/core/library_export.hpp"
#include "facekit/core/sys/stacktrace_resolver.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  ResolverWindows
 *  @brief  Stack trace resolver for windows platform
 *  @author Christophe Ecabert
 *  @date   10.07.18
 *  @ingroup core
 */
class FK_EXPORTS ResolverWindows : public StackTraceResolver {
 public:
  
  /**
   *  @name   ResolverWindows
   *  @fn     ResolverWindows(void) = default
   *  @brief  Constructor
   */
  ResolverWindows(void) = default;
  
  /**
   *  @name   ResolverWindows
   *  @fn     ~ResolverWindows(void) = default
   *  @brief  Destructor
   */
  ~ResolverWindows(void) = default;
  
  /**
   *  @name   Resolve
   *  @fn     Status Resolve(StackTrace* trace)
   *  @brief  Gather information for a given stack trace
   *  @param[in, out] trace Stack trace to gather information for
   *  @return Status of the operation.
   */
  Status Resolve(StackTrace* trace);
};
  
}  // namespace FaceKit
#endif /* __FACEKIT_STACKTRACE_RESOLVER_WINDOWS__ */
