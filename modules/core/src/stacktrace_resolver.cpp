/**
 *  @file   stacktrace_resolver.cpp
 *  @brief Utility class used by StackTrace class to populate the frames info.
 *  @see  https://github.com/bloomberg/bde/blob/master/groups/bal/balst/balst_stacktraceresolverimpl_dladdr.h
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   10.07.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#if defined(__APPLE__) || defined(__linux__)
#define IS_POSIX
#endif

#include "facekit/core/sys/stacktrace_resolver.hpp"
#include "facekit/core/logger.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark Proxy
  
/*
 *  @name   StackTraceResolverProxy
 *  @fn     StackTraceResolverProxy(void)
 *  @brief  Constructor
 */
StackTraceResolverProxy::StackTraceResolverProxy(void) {
  StackTraceResolverFactory::Get().Register(this);
}
  
#pragma mark -
#pragma mark Factory
  
/*
 *  @name   Get
 *  @fn     static StackTraceResolverFactory& Get(void)
 *  @brief  Provide single instance of the factory
 */
StackTraceResolverFactory& StackTraceResolverFactory::Get(void) {
  static StackTraceResolverFactory instance;
  return instance;
}
  
/*
 *  @name   Register
 *  @fn     void Register(const StackTraceResolverProxy* proxy)
 *  @brief  Register a new stack trace resolver system into the factory
 *  @param[in]  proxy File system's proxy
 *                    (from macro REGISTER_STACKTRACE_RESOLVER_SYSTEM)
 */
void StackTraceResolverFactory::Register(const StackTraceResolverProxy* proxy) {
  // Already registered ?
  auto it = proxies_.find(proxy);
  if (it == proxies_.end()) {
    // No register
    proxies_.insert(proxy);
  } else {
    FACEKIT_LOG_ERROR("StackTraceResolver already registered");
  }
}
  
/*
 *  @name   Retrieve
 *  @fn     StackTraceResolverProxy* Retrieve(const std::string& name) const
 *  @brief  Retrieve a stack trace resolver corresponding to a given name or
 *          nullptr
 *  @return StaceTraceResolver system instance or nullptr.
 */
StackTraceResolver* StackTraceResolverFactory::Retrieve(const std::string& name) const {
  StackTraceResolver* instance = nullptr;
  for (const auto& p : proxies_) {
    if (p->Name() == name) {
      instance = p->Create();
      break;
    }
  }
  return instance;
}
  
  
  
#pragma mark -
#pragma mark Getter
  
/*
 *  @name   get_stacktrace_resolver
 *  @fn     StackTraceResolver* FK_EXPORTS get_stacktrace_resolver(void)
 *  @brief  Provide a stacktrace resolver
 *  @return Resolver object
 */
StackTraceResolver* get_stacktrace_resolver(void) {
#ifdef IS_POSIX
  return StackTraceResolverFactory::Get().Retrieve("DlAddr");
#else
  return StackTraceResolverFactory::Get().Retrieve("Windows");
#endif
}

}  // namespace FaceKit
