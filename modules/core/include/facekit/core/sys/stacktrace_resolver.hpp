/**
 *  @file   stacktrace_resolver.hpp
 *  @brief Utility class used by StackTrace class to populate the frames info.
 *  @see  https://github.com/bloomberg/bde/blob/master/groups/bal/balst/balst_stacktraceresolverimpl_dladdr.h
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   10.07.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_STACKTRACE_RESOLVER__
#define __FACEKIT_STACKTRACE_RESOLVER__

#include <unordered_set>

#include "facekit/core/library_export.hpp"
#include "facekit/core/status.hpp"
#include "facekit/core/sys/stacktrace.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  StackTraceResolver
 *  @brief  Abstract interface for stack trace resolver
 *  @author Christophe Ecabert
 *  @date   10.07.18
 *  @ingroup core
 */
class StackTraceResolver {
 public:
  
  /**
   *  @name   ~StackTraceResolver
   *  @fn     virtual ~StackTraceResolver(void) = default
   *  @brief  Destructor
   */
  virtual ~StackTraceResolver(void) = default;
  
  /**
   *  @name   Resolve
   *  @fn     virtual Status Resolve(StackTrace* trace) = 0
   *  @brief  Gather information for a given stack trace
   *  @param[in, out] trace Stack trace to gather information for
   *  @return Status of the operation.
   */
  virtual Status Resolve(StackTrace* trace) = 0;
};
  
/**
 *  @class  StackTraceResolverProxy
 *  @brief  Proxy for registering/creating StackTraceResolver
 *  @author Christophe Ecabert
 *  @date   10.07.18
 *  @ingroup core
 */
class FK_EXPORTS StackTraceResolverProxy {
 public:
  /**
   *  @name   StackTraceResolverProxy
   *  @fn     StackTraceResolverProxy(void)
   *  @brief  Constructor
   */
  StackTraceResolverProxy(void);
  
  /**
   *  @name   ~StackTraceResolverProxy
   *  @fn     ~StackTraceResolverProxy(void) = default
   *  @brief  Destructor
   */
  virtual ~StackTraceResolverProxy(void) = default;
  
  /**
   *  @name   Create
   *  @fn     virtual StackTraceResolver* Create(void) const = 0
   *  @brief  Craete an instance of StackTraceResolver for this proxy
   */
  virtual StackTraceResolver* Create(void) const = 0;
  
  /**
   *  @name   Name
   *  @fn     virtual std::string Name(void) const = 0
   *  @brief  Provide the name of the proxy
   */
  virtual std::string Name(void) const = 0;
};
  
/**
 *  @class  StackTraceResolverFactory
 *  @brief  Mechanism to store StackTraceResolver implementations
 *  @author Christophe Ecabert
 *  @date   10.07.18
 *  @ingroup core
 */
class FK_EXPORTS StackTraceResolverFactory {
 public:
  
  /**
   *  @name   Get
   *  @fn     static StackTraceResolverFactory& Get(void)
   *  @brief  Provide single instance of the factory
   */
  static StackTraceResolverFactory& Get(void);
  
  /**
   *  @name   ~StackTraceResolverFactory
   *  @fn     ~StackTraceResolverFactory(void)
   *  @brief  Destructor
   */
  ~StackTraceResolverFactory(void) = default;
  
  /**
   *  @name   StackTraceResolverFactory
   *  @fn     StackTraceResolverFactory(const StackTraceResolverFactory& other) = delete
   *  @brief  Copy constructor
   */
  StackTraceResolverFactory(const StackTraceResolverFactory& other) = delete;
  
  /**
   *  @name   operator=
   *  @fn     StackTraceResolverFactory& operator=(const StackTraceResolverFactory& rhs) = delete
   *  @brief  Copy assignment
   *  @param[in] rhs  Object to assign from
   *  @return Newly assigned object
   */
  StackTraceResolverFactory& operator=(const StackTraceResolverFactory& rhs) = delete;
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name   Register
   *  @fn     void Register(const StackTraceResolverProxy* proxy)
   *  @brief  Register a new stack trace resolver system into the factory
   *  @param[in]  proxy File system's proxy 
   *                    (from macro REGISTER_STACKTRACE_RESOLVER_SYSTEM)
   */
  void Register(const StackTraceResolverProxy* proxy);
  
  /**
   *  @name   Retrieve
   *  @fn     StackTraceResolver* Retrieve(const std::string& name) const
   *  @brief  Retrieve a stack trace resolver corresponding to a given name or
   *          nullptr
   *  @return StaceTraceResolver system instance or nullptr.
   */
  StackTraceResolver* Retrieve(const std::string& name) const;
  
#pragma mark -
#pragma mark Private
private:
  
  /**
   *  @name   StackTraceResolverFactory
   *  @fn     StackTraceResolverFactory(void) = default
   *  @brief  Constructor
   */
  StackTraceResolverFactory(void) = default;
  
  
  /** Registered FileSystem */
  std::unordered_set<const StackTraceResolverProxy*> proxies_;
};
  
/**
 *  @def  REGISTER_STACKTRACE_RESOLVER_SYSTEM
 *  @brief  Register a given resolver_system (StackTraceResolver)
 */
#define REGISTER_STACKTRACE_RESOLVER_SYSTEM(name, resolver_system)        \
  class resolver_system##Proxy : public StackTraceResolverProxy {         \
   public:                                                                \
    resolver_system##Proxy() : StackTraceResolverProxy() {}               \
    ~resolver_system##Proxy() override = default;                         \
    StackTraceResolver* Create(void) const override {                     \
      return (StackTraceResolver*)&resolver_;                             \
    }                                                                     \
    std::string Name(void) const override { return name;  }               \
   private:                                                               \
    resolver_system resolver_;                                            \
  };                                                                      \
  static resolver_system##Proxy resolver_system##Registrator;

/**
 *  @name   get_stacktrace_resolver
 *  @fn     StackTraceResolver* FK_EXPORTS get_stacktrace_resolver(void)
 *  @brief  Provide a stacktrace resolver
 *  @return Resolver object
 */
StackTraceResolver* FK_EXPORTS get_stacktrace_resolver(void);

}  // namespace FaceKit
#endif /* __FACEKIT_STACKTRACE_RESOLVER__ */
