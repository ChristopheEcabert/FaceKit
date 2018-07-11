/**
 *  @file   stacktrace.cpp
 *  @brief Class creating a StackTrace for debugging purpose. Would typically
 *         been use in expection.
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   10.07.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#if defined(__APPLE__) || defined(__linux__)
#include <execinfo.h> // for backtrace
#define IS_POSIX
#else
#include <Windows.h>
#define IS_WINDOW
#endif

#include <sstream>
#include <limits>

#include "facekit/core/sys/stacktrace.hpp"
#include "facekit/core/sys/stacktrace_resolver.hpp"
#include "facekit/core/logger.hpp"
#include "facekit/core/utils/string.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark StackTraceFrame
  
/*
 *  @name   StackTraceFrame
 *  @fn     StackTraceFrame(void)
 *  @brief  Constructor
 */
StackTraceFrame::StackTraceFrame(void) : address_(nullptr),
  library_name_(""),
  line_number_(-1),
  mangled_symbol_name_(""),
  symbol_name_(""),
  offset_from_symbol_(std::numeric_limits<size_t>::max()),
  src_file_name_("") {}

/*
 *  @name   StackTraceFrame
 *  @fn     StackTraceFrame(const void* address,
                             const std::string& library_name,
                             const int& line_number,
                             const std::string& mangled_symbol_name,
                             const std::string& symbol_name,
                             const std::size_t& offset,
                             const std::string& src_file)
 *  @brief  Constructor
 *  @param[in] address  Frame address
 *  @param[in] library_name Name of the executable or shared-object file
 *  @param[in] line_number  Line number
 *  @param[in] mangled_symbol_name  Mangled symnbol name
 *  @param[in] symbol_name  Unmangled symbol name
 *  @param[in] offset Function offset
 *  @param[in] src_file Source file
 */
StackTraceFrame::StackTraceFrame(const void* address,
                                 const std::string& library_name,
                                 const int& line_number,
                                 const std::string& mangled_symbol_name,
                                 const std::string& symbol_name,
                                 const std::size_t& offset,
                                 const std::string& src_file) :
  address_(address),
  library_name_(library_name),
  line_number_(line_number),
  mangled_symbol_name_(mangled_symbol_name),
  symbol_name_(symbol_name),
  offset_from_symbol_(offset),
  src_file_name_(src_file) {}

#pragma mark -
#pragma mark Usage

/*
 *  @name   Print
 *  @fn     std::iostream& Print(std::iostream& stream,
 const size_t& level,
 const size_t& space_per_level) const
 *  @brief  Print this frame into a given stream
 *  @param[in] stream Stream where to dump the frame
 *  @param[in] level  Level of the trace
 *  @param[in] space  Number of space to shift for each level
 *  @return Stream used
 */
std::ostream& StackTraceFrame::Print(std::ostream& stream,
                                     const size_t& level,
                                     const size_t& space) const {
  if (stream.good()) {
    auto shift = std::string((level * space) + 1, ' ');
    stream << " * " << address_ << shift << symbol_name_;
    if (offset_from_symbol_ != std::numeric_limits<size_t>::max()) {
      stream << " + " << offset_from_symbol_;
    }
    stream << " (" << Path::Basename(library_name_);
    if (!src_file_name_.empty() && line_number_ != -1) {
      stream << "," << src_file_name_ << ":" << line_number_;
    }
    stream << ")" << std::endl;
  }
  return stream;
}

#pragma mark -
#pragma mark Predicates

/*
 *  @name   IsAddressKnown
 *  @fn     bool IsAddressKnown(void) const
 *  @brief  Indicate if the address is known
 *  @return True if the address has been set to some value, false otherwise
 */
bool StackTraceFrame::IsAddressKnown(void) const {
  return address_ != nullptr;
}

/*
 *  @name   IsLibraryNameKnown
 *  @fn     bool IsLibraryNameKnown(void) const
 *  @brief  Indicate if the library name has been set
 *  @return True if the library has been set, false otherwise
 */
bool StackTraceFrame::IsLibraryNameKnown(void) const {
  return !library_name_.empty();
}

/*
 *  @name   IsLineNumberKnown
 *  @fn     bool IsLineNumberKnown(void) const
 *  @brief  Indicate if the line number has been set
 *  @return True if the line number has been set, false otherwise
 */
bool StackTraceFrame::IsLineNumberKnown(void) const {
  return line_number_ != -1;
}

/*
 *  @name   IsMangledSymbolNameKnown
 *  @fn     bool IsMangledSymbolNameKnown(void) const
 *  @brief  Indicate if the mangled symbol's name has been set
 *  @return True if the mangled symbol name has been set, false otherwise
 */
bool StackTraceFrame::IsMangledSymbolNameKnown(void) const {
  return !mangled_symbol_name_.empty();
}

/*
 *  @name   IsSymbolNameKnown
 *  @fn     bool IsSymbolNameKnown(void) const
 *  @brief  Indicate if the symbol's name has been set
 *  @return True if the symbol's name has been set, false otherwise
 */
bool StackTraceFrame::IsSymbolNameKnown(void) const {
  return !symbol_name_.empty();
}

/*
 *  @name   IsOffsetKnown
 *  @fn     bool IsOffsetKnown(void) const
 *  @brief  Indicate if the offset is set
 *  @return True if the offset has been set, false otherwise
 */
bool StackTraceFrame::IsOffsetKnown(void) const {
  return offset_from_symbol_ != std::numeric_limits<size_t>::max();
}

/*
 *  @name   IsSourceFileNameKnown
 *  @fn     bool IsSourceFileNameKnown(void) const
 *  @brief  Indicate if the source file is set
 *  @return True if the source filename has been set, false otherwise
 */
bool StackTraceFrame::IsSourceFileNameKnown(void) const {
  return !src_file_name_.empty();
}
  
#pragma mark -
#pragma mark StackTrace
  
/*
 *  @name   StackTrace
 *  @fn     StackTrace(void)
 *  @brief  Constructor
 */
StackTrace::StackTrace(void) : StackTrace(0, StackTrace::kDefaultDepth_) {}
  
/*
 *  @name   StackTrace
 *  @fn     StackTrace(const size_t& skip, const size_t& depth)
 *  @brief  Constructor
 *  @param[in]  skip  Number of level to skip before recording the trace
 *  @param[in]  depth Depth of the trace
 */
StackTrace::StackTrace(const size_t& skip, const size_t& depth) {
#ifdef IS_POSIX
  // storage array for stack trace address data
  std::vector<void*> callstack(depth, nullptr);
  // retrieve current stack addresses
  int n_call = backtrace(callstack.data(), static_cast<int>(callstack.size()));
  if (n_call != 0) {
    size_t sz = n_call > skip ? n_call - skip : 0;
    frames_.reserve(sz);
    for (size_t i = skip; i < n_call; ++i) {
      auto frame = StackTraceFrame();
      frame.set_address(callstack[i]);
      frames_.push_back(frame);
    }
    trace_valid_ = true;
  }
#else
  // storage array for stack trace address data
  std::vector<void*> callstack(depth, nullptr);
  const ULONG framesToSkip = skip;
  const ULONG framesToCapture = depth;
  const USHORT n_call = CaptureStackBackTrace(framesToSkip,
                                              framesToCapture,
                                              callstack.data(),
                                              nullptr);
  if (n_call != 0) {
    frames_.reserve(n_call);
    for (size_t i = 0; i < n_call; ++i) {
      auto frame = StackTraceFrame();
      frame.set_address(callstack[i]);
      frames_.push_back(frame);
    }
    trace_valid_ = true;
  }
#endif
}
  
/** Default depth */
constexpr size_t StackTrace::kDefaultDepth_;
  
  
#pragma mark -
#pragma mark Usage
  
/*
 *  @name   ToString
 *  @fn     void ToString(std::string* str) const
 *  @brief  Convert the trace into a readable string
 *  @param[out] str  String holding formatted stack trace, or empty if
 *                   something went wrong.
 *  @return Status of the operations
 */
Status StackTrace::ToString(std::string* str) const {
  Status s;
  if (trace_valid_) {
    // Resolve trace
    auto* resolver = get_stacktrace_resolver();
    s = resolver->Resolve(const_cast<StackTrace*>(this));
    if (s.Good()) {
      // Dump to string
      std::ostringstream stream;
      stream << "Stack trace:" << std::endl;
      for (size_t i = 0; i < frames_.size(); ++i) {
        frames_[i].Print(stream, i, 1);
      }
      str->assign(stream.str());
    } else {
      str->clear();
    }
  } else {
    str->clear();
    s = Status(Status::Type::kInternalError, "No valid trace was generated");
  }
  return s;
}
  
}  // namespace FaceKit
