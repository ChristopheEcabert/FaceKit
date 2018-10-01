/**
 *  @file   tee_stream.cpp
 *  @brief Mechanism to duplicate stream and dump information into various
 *         streams. I.e. file + console similar to `tee` on linux
 *  @ingroup core
 *  @see  https://stackoverflow.com/a/1761027/4546884
 *
 *  @author Christophe Ecabert
 *  @date   19.09.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include "facekit/core/tee_stream.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark Streambuf

/*
 *  @name   InsertBuffer
 *  @fn     void InsertBuffer(std::streambuf* streambuf)
 *  @brief  $add a streambuf where to mirror incoming data
 *  @param[in] streambuf  Stream to be mirrored
 */
void TeeStream::TeeStreambuf::InsertBuffer(std::streambuf *streambuf) {
  streams_.push_back(streambuf);
}
  
/*
 *  @name   overflow
 *  @fn     virtual int_type overflow(int_type c) override
 *  @brief  C.f. C++ standard section 27.5.2.4.5
 */
TeeStream::TeeStreambuf::int_type
TeeStream::TeeStreambuf::overflow(int_type c ) {
  if (!traits_type::eq_int_type(c, traits_type::eof())) {
    bool is_eof = false;
    for (auto* s : streams_) {
      auto r = s->sputc(traits_type::to_char_type(c));
      is_eof |= traits_type::eq_int_type(r, traits_type::eof());
    }
    return is_eof ? traits_type::eof() : c;
  } else {
    return traits_type::not_eof(c);
  }
}
  
/*
 *  @brief  Synchronizes the buffer arrays with the controlled sequences.
 *  @return  -1 on failure.
 *
 *  Each derived class provides its own appropriate behavior,
 *  including the definition of @a failure.
 *  @note  Base class version does nothing, returns zero.
 */
int TeeStream::TeeStreambuf::sync() {
  int err = 0;
  for (auto* s : streams_) {
    err |= s->pubsync();
  }
  return err;
}
  
  
  
#pragma mark Stream
  
}  // namespace FaceKit
