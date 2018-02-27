/**
 *  @file   nd_array.cpp
 *  @brief Class for ND-Array implementation. Inspired by Tensorflow
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   23.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include <iostream>

#include "nd_array.pb.h"

#include "facekit/core/nd_array.hpp"
#include "facekit/core/logger.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
  
#pragma mark -
#pragma mark NDArrayBuffer
  
/**
 *  @class  Buffer
 *  @brief  Memory buffer
 *  @author Christophe Ecabert
 *  @date   27.02.18
 *  @ingroup core
 *  @tparam T Data type
 */
template<typename T>
class Buffer : public NDArrayBuffer {
 public:
  
#pragma mark Initialization
  
  /**
   *  @name   Buffer
   *  @fn     Buffer(const size_t& n_element, Allocator* allocator)
   *  @brief  Constructor
   *  @param[in] n_element Number of element of type `T` to allocate
   *  @param[in] allocator  Allocator to use for memory allocation
   */
  Buffer(const size_t& n_element, Allocator* allocator);
  
  /**
   *  @name   Buffer
   *  @fn     Buffer(const Buffer& other) = delete
   *  @brief  Copy constructor
   *  @param[in] other  Object to copy from
   */
  Buffer(const Buffer& other) = delete;
  
  /**
   *  @name   operator=
   *  @fn     Buffer& operator=(const Buffer& rhs) = delete
   *  @brief  Assignment operator
   *  @param[in] rhs  Object to assign from
   *  @return Newly assigned object
   */
  Buffer& operator=(const Buffer& rhs) = delete;
  
#pragma mark Usage
  
  /**
   *  @name   data
   *  @fn     void* data(void) const override
   *  @brief  Pointer to the buffer storing data of a given `size` in bytes
   *  @return Buffer address
   */
  void* data(void) const override {
    return data_;
  }
  
  /**
   *  @name   size
   *  @fn     size_t size(void) const override
   *  @brief  Buffer dimension in bytes
   *  @return Number of bytes in the buffer
   */
  size_t size(void) const override {
    return n_elem_ * sizeof(T);
  }
  
  /**
   *  @name   root
   *  @fn     NDArrayBuffer* root(void) override
   *  @brief  Provide reference to the buffer interface, most of the time it
   *          return the address of `this` however when the buffer is a
   *          subpart of another one, it will point to this one.
   */
  NDArrayBuffer* root(void) override {
    return this;
  }
  
#pragma mark Private
 private:
  
  /**
   *  @name   ~Buffer
   *  @fn     ~Buffer(void) override
   *  @brief  Destructor
   */
  ~Buffer(void) override;
  
  /** Allocator */
  Allocator* const allocator_;
  /** Raw data array */
  T* data_;
  /** Buffer size */
  size_t n_elem_;
};
  
/*
 *  @name   Buffer
 *  @fn     Buffer(const size_t& n_element, Allocator* allocator)
 *  @brief  Constructor
 *  @param[in] n_element Number of element of type `T` to allocate
 *  @param[in] allocator  Allocator to use for memory allocation
 */
template<typename T>
Buffer<T>::Buffer(const size_t& n_element,
                  Allocator* allocator) : allocator_(allocator),
                                          data_(allocator->Allocate<T>(n_element)),
                                          n_elem_(n_element) {
}

/*
 *  @name   ~Buffer
 *  @fn     ~Buffer(void) override
 *  @brief  Destructor
 */
template<typename T>
Buffer<T>::~Buffer(void) {
  // Has some data ?
  if (data_) {
    allocator_->Deallocate<T>(n_elem_, data_);
  }
}

/** Forward args */
#define ARG(...) __VA_ARGS__
/** Define statement for on case of the switch */
#define CASE(TYPE, CASE_CODE)           \
  case DataTypeToEnum<TYPE>::value: {   \
    using T = TYPE;                     \
    CASE_CODE;                          \
  }                                     \
    break;
/** Define each case of possible DataType enum */
#define SWITCH_WITH_DEFAULT(TYPE_ENUM, CCODE, UNKNOWN, DEFAULT) \
  switch(TYPE_ENUM) {                                           \
    CASE(int8_t, ARG(CCODE))                                    \
    CASE(uint8_t, ARG(CCODE))                                   \
    CASE(int16_t, ARG(CCODE))                                   \
    CASE(uint16_t, ARG(CCODE))                                  \
    CASE(int32_t, ARG(CCODE))                                   \
    CASE(uint32_t, ARG(CCODE))                                  \
    CASE(float, ARG(CCODE))                                     \
    CASE(double, ARG(CCODE))                                    \
    CASE(size_t, ARG(CCODE))                                    \
    CASE(bool, ARG(CCODE))                                      \
    CASE(std::string, ARG(CCODE))                               \
    case DataType::kUnknown:                                    \
      UNKNOWN;                                                  \
      break;                                                    \
    default:                                                    \
      DEFAULT;                                                  \
      break;                                                    \
  }


#pragma mark -
#pragma mark Initialization
  
/*
 *  @name   NDArray
 *  @fn     NDArray(void)
 *  @brief  Constructor, create an empty NDArray container with 0 element
 *          inside.
 */
NDArray::NDArray(void) : buffer_(nullptr),
                         allocator_(DefaultCpuAllocator()),
                         dims_({}),
                         type_(DataType::kUnknown) {
}
  
/*
 *  @name   NDArray
 *  @fn     explicit NDArray(Allocator* allocator)
 *  @brief  Constructor, create an empty NDArray container with 0 element
 *          inside.
 *  @param[in] allocator  Allocator to use when create internal buffer
 */
NDArray::NDArray(Allocator* allocator) : buffer_(nullptr),
                                         allocator_(allocator),
                                         dims_({}),
                                         type_(DataType::kUnknown){
}
  
/*
 *  @name   NDArray
 *  @fn     NDArray(const DataType& type, const NDArrayDims& dims)
 *  @brief  Constructor, create a container of a specific type with given
 *          dimensions. The buffer is allocated with default cpu allocator.
 *  @param[in] type Data type to store
 *  @param[in] dims Array dimensions
 */
NDArray::NDArray(const DataType& type,
                 const NDArrayDims& dims) : buffer_(nullptr),
                                            allocator_(DefaultCpuAllocator()),
                                            dims_(dims),
                                            type_(type) {
  this->Resize(type_, dims_);
}
  
/*
 *  @name   NDArray
 *  @fn     NDArray(const DataType& type, const NDArrayDims& dims)
 *  @brief  Constructor, create a container of a specific type with given
 *          dimensions. The buffer is allocated with a given allocator
 *          (should last longer than the lifetime of this container).
 *  @param[in] type Data type to store
 *  @param[in] dims Array dimensions
 *  @param[in] allocator  Allocator to use to instantiate buffer.
 */
NDArray::NDArray(const DataType& type,
                 const NDArrayDims& dims,
                 Allocator* allocator) : buffer_(nullptr),
                                         allocator_(allocator),
                                         dims_(dims),
                                         type_(type) {
  this->Resize(type_, dims_);
}
  
/*
 *  @name   ~NDArray
 *  @fn     ~NDArray(void)
 *  @brief  Destructor
 */
NDArray::~NDArray(void) {
  if (buffer_) {
    buffer_->Dec();
  }
}
  
/*
 *  @name   Resize
 *  @fn     void Resize(const DataType& type, const NDArrayDims& dims)
 *  @brief  Resize the NDArray to new dimensions/type
 */
void NDArray::Resize(const DataType& type, const NDArrayDims& dims) {
  // Compute curr
  size_t size = dims_.n_elems() * DataTypeDynamicSize(type_);
  size_t wanted_size = dims.n_elems() * DataTypeDynamicSize(type);
  if (buffer_ == nullptr || (size != wanted_size || type_ != type)) {
    // Release current buffer if any
    if (buffer_) {
      buffer_->Dec();
    }
    // Define type + size
    type_ = type;
    dims_ = dims;
    // Allocate buffer
    SWITCH_WITH_DEFAULT(type_,
                        buffer_ = new Buffer<T>(dims_.n_elems(), allocator_),
                        FACEKIT_LOG_ERROR("Unknown data type: " << type_),
                        FACEKIT_LOG_ERROR("Data type not set"));
  }
}

/*
 *  @name   ToProto
 *  @fn     void ToProto(NDArrayProto* proto) const
 *  @brief  Export the NDArray to a Protocol buffer object
 *  @param[out] proto Protocol Buffer Object to write into
 */
void NDArray::ToProto(NDArrayProto* proto) const {
  proto->Clear();
  // Do conversion
  
  
}

/*
 *  @name   FromProto
 *  @fn     Status FromProto(const NDArrayProto& proto)
 *  @brief  Fill this NDArray from a given Protocol Buffer Object.
 *  @return Operation status
 */
Status NDArray::FromProto(const NDArrayProto& proto) {
  return Status(Status::Type::kUnimplemented,
                "No conversion supported at the moment");
}
  
  
  
  
}  // namespace FaceKit

