/**
 *  @file   stacktrace.hpp
 *  @brief Class creating a StackTrace for debugging purpose. Would typically 
 *         been use in expection.
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   10.07.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_STACKTRACE__
#define __FACEKIT_STACKTRACE__

#include <vector>

#include "facekit/core/library_export.hpp"
#include "facekit/core/status.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  StackTraceFrame
 *  @brief  Provide an attribute class describing an element (frame) of the
 *          current stacktrace.
 *  @see: https://github.com/bloomberg/bde/blob/master/groups/bal/balst/
 *  @author Christophe Ecabert
 *  @date   09.07.18
 *  @ingroup core
 */
class FK_EXPORTS StackTraceFrame {
public:
  
#pragma mark -
#pragma mark Initialisation
  
  /**
   *  @name   StackTraceFrame
   *  @fn     StackTraceFrame(void)
   *  @brief  Constructor
   */
  StackTraceFrame(void);
  
  /**
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
  StackTraceFrame(const void* address,
                  const std::string& library_name,
                  const int& line_number,
                  const std::string& mangled_symbol_name,
                  const std::string& symbol_name,
                  const std::size_t& offset,
                  const std::string& src_file);
  
  /**
   *  @name   StackTraceFrame
   *  @fn     StackTraceFrame(const StackTraceFrame& other)
   *  @brief  Copy constructor
   *  @param[in] other  Object to copy from
   */
  StackTraceFrame(const StackTraceFrame& other) = default;
  
  /**
   *  @name   operator=
   *  @fn     StackTraceFrame& operator=(const StackTraceFrame& rhs) = default
   *  @brief  Assignment operator
   *  @param[in] rhs  Object to assign from
   *  @return newly assigned operator
   */
  StackTraceFrame& operator=(const StackTraceFrame& rhs) = default;
  
  /**
   *  @name   ~StackTraceFrame
   *  @fn     ~StackTraceFrame(void) = default
   *  @brief  Destructor
   */
  ~StackTraceFrame(void) = default;
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name   Print
   *  @fn     std::ostream& Print(std::ostream& stream,
                                const size_t& level, const size_t& space) const
   *  @brief  Print this frame into a given stream
   *  @param[in] stream Stream where to dump the frame
   *  @param[in] level  Level of the trace
   *  @param[in] space  Number of space to shift for each level
   *  @return Stream used
   */
  std::ostream& Print(std::ostream& stream,
                      const size_t& level,
                      const size_t& space) const;
  
#pragma mark -
#pragma mark Accessors
  
  /**
   *  @name   set_address
   *  @fn     void set_address(const void* address)
   *  @brief  Fill the address attribute
   *  @param[in]  address Frame's address
   */
  void set_address(const void* address) {
    address_ = address;
  }
  
  /**
   *  @name   get_address
   *  @fn     const void* get_address(void) const
   *  @brief  Provide the frame's address
   *  @return The return address in the parent function on the return from the
   *          fild function
   */
  const void* get_address(void) const {
    return address_;
  }
  
  /**
   *  @name   set_library_name
   *  @fn     void set_library_name(const std::string& name)
   *  @brief  Set the library's name for the frame
   *  @param[in] name Library's name
   */
  void set_library_name(const std::string& name) {
    library_name_ = name;
  }
  
  /**
   *  @name   get_library_name
   *  @fn     const std::string& get_library_name(void) const
   *  @brief  Provide the library's name for the frame
   *  @return Executable / shared-object's name containing the parent function
   */
  const std::string& get_library_name(void) const {
    return library_name_;
  }
  
  /**
   *  @name   set_line_number
   *  @fn     void set_line_number(const int& number)
   *  @brief  Set line number of the frame
   *  @param[in] number Line number
   */
  void set_line_number(const int& number) {
    line_number_ = number;
  }
  
  /**
   *  @name   get_line_number
   *  @fn     int get_line_number(void) const
   *  @brief  Provide the frame's line number
   *  @return Line number
   */
  int get_line_number(void) const {
    return line_number_;
  }
  
  /**
   *  @name   set_mangled_symbol_name
   *  @fn     void set_mangled_symbol_name(const std::string& name)
   *  @brief  Set symbol's mangled name
   *  @param[in] name Symbol's mangled name
   */
  void set_mangled_symbol_name(const std::string& name) {
    mangled_symbol_name_ = name;
  }
  
  /**
   *  @name   get_mangled_symbol_name
   *  @fn     const std::string& get_mangled_symbol_name(void) const
   *  @brief  Provide frame's mangled symbol's name
   *  @return Mangled name of the parent function
   */
  const std::string& get_mangled_symbol_name(void) const {
    return mangled_symbol_name_;
  }
  
  /**
   *  @name   set_symbol_name
   *  @fn     void set_symbol_name(const std::string& name)
   *  @brief  Set unmnagled symbol's name
   *  @param[in] name Unmangled symbol's name
   */
  void set_symbol_name(const std::string& name) {
    symbol_name_ = name;
  }
  
  /**
   *  @name   get_symbol_name
   *  @fn     const std::string& get_symbol_name(void) const
   *  @brief  Give the unmangled symbol's name of the frame
   *  @return Unmangled synmbol name
   */
  const std::string& get_symbol_name(void) const {
    return symbol_name_;
  }
  
  /**
   *  @name   set_offset
   *  @fn     void set_offset(const size_t& offset)
   *  @brief  Set symbol's offset
   *  @param[in] offset Symbol's offset
   */
  void set_offset(const size_t& offset) {
    offset_from_symbol_ = offset;
  }
  
  /**
   *  @name   get_offset
   *  @fn     size_t get_offset(void) const
   *  @brief  Give the symbol's offset
   *  @return Offset from the start of the parent function to the call of the
   *          child function
   */
  size_t get_offset(void) const {
    return offset_from_symbol_;
  }
  
  /**
   *  @name   set_src_file_name
   *  @fn     void set_src_file_name(const std::string& name)
   *  @brief  Set the src filename of the frame's symbol
   *  @param[in] name File name
   */
  void set_src_file_name(const std::string& name) {
    src_file_name_ = name;
  }
  
  /**
   *  @name   get_src_file_name
   *  @fn     const std::string& get_src_file_name(void) const
   *  @brief  Give the source filename
   *  @return Source file of the parent function
   */
  const std::string& get_src_file_name(void) const {
    return src_file_name_;
  }
  
#pragma mark -
#pragma mark Predicates
  
  /**
   *  @name   IsAddressKnown
   *  @fn     bool IsAddressKnown(void) const
   *  @brief  Indicate if the address is known
   *  @return True if the address has been set to some value, false otherwise
   */
  bool IsAddressKnown(void) const;
  
  /**
   *  @name   IsLibraryNameKnown
   *  @fn     bool IsLibraryNameKnown(void) const
   *  @brief  Indicate if the library name has been set
   *  @return True if the library has been set, false otherwise
   */
  bool IsLibraryNameKnown(void) const;
  
  /**
   *  @name   IsLineNumberKnown
   *  @fn     bool IsLineNumberKnown(void) const
   *  @brief  Indicate if the line number has been set
   *  @return True if the line number has been set, false otherwise
   */
  bool IsLineNumberKnown(void) const;
  
  /**
   *  @name   IsMangledSymbolNameKnown
   *  @fn     bool IsMangledSymbolNameKnown(void) const
   *  @brief  Indicate if the mangled symbol's name has been set
   *  @return True if the mangled symbol name has been set, false otherwise
   */
  bool IsMangledSymbolNameKnown(void) const;
  
  /**
   *  @name   IsSymbolNameKnown
   *  @fn     bool IsSymbolNameKnown(void) const
   *  @brief  Indicate if the symbol's name has been set
   *  @return True if the symbol's name has been set, false otherwise
   */
  bool IsSymbolNameKnown(void) const;
  
  /**
   *  @name   IsOffsetKnown
   *  @fn     bool IsOffsetKnown(void) const
   *  @brief  Indicate if the offset is set
   *  @return True if the offset has been set, false otherwise
   */
  bool IsOffsetKnown(void) const;
  
  /**
   *  @name   IsSourceFileNameKnown
   *  @fn     bool IsSourceFileNameKnown(void) const
   *  @brief  Indicate if the source file is set
   *  @return True if the source filename has been set, false otherwise
   */
  bool IsSourceFileNameKnown(void) const;
  
#pragma mark -
#pragma mark Private
private:
  /** Address: Return address in the `parent` (calling) function on the return
   from the `child` (called) function */
  const void* address_;
  /** Library filename: Executable / Shared-object file name containing the
   parent function */
  std::string library_name_;
  /** Line number: Line number in the `parent` function corresponding to a call
   to the `child` function */
  int line_number_;
  /** Mangled Symbol Name: Mangled symbol name of the parent function */
  std::string mangled_symbol_name_;
  /** Symbol Name: Unmangled symbol name */
  std::string symbol_name_;
  /** Offset From Symbol: Offset from the start of the `parent` function to the
   the call of the `child` function */
  std::size_t offset_from_symbol_;
  /** Source File Name: Name of the source file of the `parent` function */
  std::string src_file_name_;
};
  

/**
 *  @class  StackTrace
 *  @brief  Create a trace of the current stack for debugging purpose. Would 
 *          typically be used in within expection context.
 *  @author Christophe Ecabert
 *  @date   10.07.18
 *  @ingroup core
 */
class FK_EXPORTS StackTrace {
 public:
  
#pragma mark -
#pragma mark Initialisation
  
  /**
   *  @name   StackTrace
   *  @fn     StackTrace(void)
   *  @brief  Constructor
   */
  StackTrace(void);
  
  /**
   *  @name   StackTrace
   *  @fn     StackTrace(const size_t& skip, const size_t& depth)
   *  @brief  Constructor
   *  @param[in]  skip  Number of level to skip before recording the trace
   *  @param[in]  depth Depth of the trace
   */
  StackTrace(const size_t& skip, const size_t& depth);
  
  /**
   *  @name   StackTrace
   *  @fn     StackTrace(const StackTrace& other) = default
   *  @brief  Copy constructor
   *  @param[in] other  Object to copy from
   */
  StackTrace(const StackTrace& other) = default;
  
  /**
   *  @name   operator=
   *  @fn     StackTrace& operator=(const StackTrace& rhs) = default
   *  @brief  Assignment operator
   *  @param[in] rhs  Object to assign from
   *  @return Newly assigned object
   */
  StackTrace& operator=(const StackTrace& rhs) = default;
  
  /**
   *  @name   ~StackTrace
   *  @fn     ~StackTrace(void) = default
   *  @brief  Destructor
   */
  ~StackTrace(void) = default;
  
  /** Default depth */
  static constexpr size_t kDefaultDepth_ = 64;
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name   ToString
   *  @fn     void ToString(std::string* str) const
   *  @brief  Convert the trace into a readable string
   *  @param[out] str  String holding formatted stack trace, or empty if
   *                   something went wrong.
   *  @return Status of the operations
   */
  Status ToString(std::string* str) const;
  
  /**
   *  @name   IsTraceValid
   *  @fn     bool IsTraceValid(void) const
   *  @brief  Indicate if the trace has been properly queried
   *  return  True if query was successful, false otherwise
   */
  bool IsTraceValid(void) const {
    return trace_valid_;
  }
  
  /**
   *  @name   Size
   *  @fn     size_t Size(void) const
   *  @brief  Give the dimension of the trace
   *  @return Trace's length
   */
  size_t Size(void) const {
    return frames_.size();
  }
  
  /**
   *  @name   At
   *  @fn     StackTraceFrame& At(const size_t& k)
   *  @brief  Provide access to the frame at position `k` (read/write)
   *  @param[in] k  Position of the frame to access
   *  @return Frame's instance
   */
  StackTraceFrame& At(const size_t& k) {
    return frames_.at(k);
  }
  
#pragma mark -
#pragma mark Private
 private:
  /** Stack frames */
  std::vector<StackTraceFrame> frames_;
  /** Indicate if stack trace has been properly queried */
  bool trace_valid_;
};
  
  

}  // namespace FaceKit
#endif /* __FACEKIT_STACKTRACE__ */
