/**
 *  @file   tee_stream.hpp
 *  @brief Mechanism to duplicate stream and dump information into various
 *         streams. I.e. file + console similar to `tee` on linux
 *  @ingroup  core
 *  @see  https://stackoverflow.com/a/1761027/4546884
 *
 *  @author Christophe Ecabert
 *  @date   19.09.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_TEE_STREAM__
#define __FACEKIT_TEE_STREAM__

#include <iostream>
#include <vector>

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  TeeStream
 *  @brief  Mirroring stream
 *  @author Christophe Ecabert
 *  @date   19.09.18
 *  @see    https://stackoverflow.com/a/1761027/4546884
 *  @ingroup core
 */
class TeeStream : public std::ostream {
 private:
  
  /**
   *  @class  TeeStreambuf
   *  @brief  Mirroring stream buffers
   *  @author Christophe Ecabert
   *  @date   19.09.18
   *  @see https://stackoverflow.com/a/1761027/4546884
   *  @ingroup core
   */
  class TeeStreambuf : public std::basic_streambuf<char> {
   private:
    /** Base type */
    using base_t = std::basic_streambuf<char>;
    
   public:
    /** Integer type */
    using int_type = typename base_t::int_type;
    /** Traits type */
    using traits_type = typename base_t::traits_type;
    
    /**
     *  @name   ~TeeStreambuf
     *  @fn     ~TeeStreambuf()
     *  @brief  Destructor
     */
    ~TeeStreambuf() override = default;
    
    /**
     *  @name   InsertBuffer
     *  @fn     void InsertBuffer(std::streambuf* streambuf)
     *  @brief  $add a streambuf where to mirror incoming data
     *  @param[in] streambuf  Stream to be mirrored
     */
    void InsertBuffer(std::streambuf* streambuf);
    
    /**
     *  @name   overflow
     *  @fn     virtual int_type overflow(int_type c) override
     *  @brief  C.f. C++ standard section 27.5.2.4.5
     */
    int_type overflow(int_type c /*=traits_type::eof()*/) override;
    
    /**
     *  @name sync
     *  @fn int sync() override
     *  @brief  Synchronizes the buffer arrays with the controlled sequences.
     *  @return  -1 on failure.
     *
     *  Each derived class provides its own appropriate behavior,
     *  including the definition of @a failure.
     *  @note  Base class version does nothing, returns zero.
     */
    int sync() override;
    
   private:
    /** Stream where to mirror data */
    std::vector<std::streambuf*> streams_;
  };
  
 public:
  
  /**
   *  @name   TeeStream
   *  @fn     TeeStream()
   *  @brief  Constructor
   */
  TeeStream();
  
  /**
   *  @name   ~TeeStream
   *  @fn     ~TeeStream() override = default
   *  @brief  Destructor
   */
  ~TeeStream() override = default;
  
  /**
   *  @name   LinkStream
   *  @fn     void LinkStream(std::ostream& stream)
   *  @brief  Link with another stream (Aggregation)
   *  @param[in] stream Stream to link with where mirrored data will be dumped
   */
  void LinkStream(std::ostream& stream);
};
  
}  // namespace FaceKit
#endif /* __FACEKIT_TEE_STREAM__ */
