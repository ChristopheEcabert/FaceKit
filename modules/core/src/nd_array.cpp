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
#include <type_traits>
#include <algorithm>

#include "google/protobuf/repeated_field.h"
#include "google/protobuf/stubs/port.h"
#include "nd_array.pb.h"
#include "types.pb.h"

#include "facekit/core/nd_array.hpp"
#include "facekit/core/logger.hpp"
#include "facekit/core/utils/proto.hpp"

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

/**
 *  @class  SubBuffer
 *  @brief  Buffer representing only a sub region of a Buffer
 *  @author Christophe Ecabert
 *  @date   1.03.18
 *  @ingroup core
 *  @tparam T Data type
 */
template<typename T>
class SubBuffer : public NDArrayBuffer {
 public:

#pragma mark Initialization

  /**
   *  @name   SubBuffer
   *  @fn     SubBuffer(NDArrayBuffer* buff, const size_t& start,
                        const size_t& n_elem)
   *  @brief  Constructor
   *  @param[in] buff   Original buffer
   *  @param[in] start  Position where the sub-buffer starts. Should be
   *                    within buffer range[0, N]
   *  @param[in] n_elem Number of element in the sub-buffer.
   *                    start + n_elem <= N sould be true (N being the size
   *                    of the complete buffer)
   */
  SubBuffer(NDArrayBuffer* buff,
            const size_t& start,
            const size_t& n_elem) : root_(buff->root()),
                                    data_(buff->base<T>() + start),
                                    n_elem_(n_elem) {
    assert(buff->base<T>() <= this->base<T>());
    T* root_end = buff->base<T>() + (buff->size() / sizeof(T));
    assert(this->base<T>() <= root_end);
    assert(this->base<T>() + n_elem <= root_end);
    root_->Inc();
  }

  /**
   *  @name   SubBuffer
   *  @fn     SubBuffer(const SubBuffer& other) = delete
   *  @brief  Copy constructor
   *  @param[in] other  Object to copy from
   */
  SubBuffer(const SubBuffer& other) = delete;

  /**
   *  @name   operator=
   *  @fn     SubBuffer& operator=(const SubBuffer& rhs) = delete
   *  @brief  Assignment operator
   *  @param[in] rhs  Object to assign from
   *  @return Newly assigned object
   */
  SubBuffer& operator=(const SubBuffer& rhs) = delete;

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
    return root_;
  }

#pragma mark Private
 private:

  /**
   *  @name   ~SubBuffer
   *  @fn     ~SubBuffer(void) override
   *  @brief  Destructor
   */
  ~SubBuffer(void) override {
    root_->Dec();
  }

  /** Original buffer */
  NDArrayBuffer* root_;
  /** Data */
  T* data_;
  /** Size */
  size_t n_elem_;
};

#pragma mark -
#pragma mark Proto utility function

/**
 *  @struct  ProtoStream
 *  @brief  Functor for reading/writing to NDArrayProto object (serialization)
 *  @author Christophe Ecabert
 *  @date   1.03.18
 *  @ingroup core
 *  @tparam T Data type
 */
template<typename T>
struct ProtoStream {
  // Ensure only supported type are used
  static_assert(IsDataTypeValid<T>::value, "T is not a supported type");
  static_assert(std::is_trivial<T>::value,
                "T is not trivial, specialization needed");

  /**
   *  @name   Write
   *  @fn     static void Write(const NDArrayBuffer* buffer, const size_t n, NDArrayProto* dst)
   *  @brief  Dump a given buffer into a proto object
   *  @param[in] buffer Data to write
   *  @param[in] n      Number of element to write
   *  @param[out] dst Protocol Buffer object to write into
   */
  static void Write(const NDArrayBuffer* buffer, const size_t& n, NDArrayProto* dst) {
    assert(buffer->size() == n * sizeof(T));
    // Copy data
    dst->mutable_data()->assign(buffer->base<const char>(), buffer->size());
  }

  /**
   *  @name   Read
   *  @fn     static NDArrayBuffer* Read(const NDArrayProto& src, const size_t& n, Allocator* alloc)
   *  @brief  Create an NDArrayBuffer from agiven proto object
   *  @param[in]  src Protocol Buffer Object
   *  @param[in]  n   Number of element to read
   *  @param[in]  alloc Allocator to use for creating the underlying buffer
   *  @return An NDArrayBuffer if everything went well, nullptr otherwise.
   */
  static NDArrayBuffer* Read(const NDArrayProto& src, const size_t& n, Allocator* alloc) {
    auto data = src.data();
    if (data.size() != n * sizeof(T)) {
      // Size miss match
      FACEKIT_LOG_DEBUG("Dimensions miss-match: provided" << n * sizeof(T) << " actual " << data.size());
      return nullptr;
    }
    // Create buffer
    NDArrayBuffer* buffer = new Buffer<T>(n, alloc);
    char* ptr = buffer->base<char>();
    if (ptr == nullptr) {
      // Allocation went wrong, release buffer
      buffer->Dec();
      return nullptr;
    }
    // Copy data to buffer
    std::copy(data.begin(), data.end(), ptr);
    return buffer;
  }
};

template<>
struct ProtoStream<std::string> {

  /**
   *  @name   Write
   *  @fn     static void Write(const NDArrayBuffer* buffer, const size_t n, NDArrayProto* dst)
   *  @brief  Dump a given buffer into a proto object
   *  @param[in] buffer Data to write
   *  @param[in] n      Number of element to write
   *  @param[out] dst Protocol Buffer object to write into
   */
  static void Write(const NDArrayBuffer* buffer, const size_t& n, NDArrayProto* dst) {
    // Write string list
    const std::string* strings = buffer->base<const std::string>();
    EncodeStringList(strings, n, dst->mutable_data());
  }

  /**
   *  @name   Read
   *  @fn     static NDArrayBuffer* Read(const NDArrayProto& src, const size_t& n, Allocator* alloc)
   *  @brief  Create an NDArrayBuffer from agiven proto object
   *  @param[in]  src Protocol Buffer Object
   *  @param[in]  n   Number of element to read
   *  @param[in]  alloc Allocator to use for creating the underlying buffer
   *  @return An NDArrayBuffer if everything went well, nullptr otherwise.
   */
  static NDArrayBuffer* Read(const NDArrayProto& src, const size_t& n, Allocator* alloc) {
    auto data = src.data();
    // Create buffer
    NDArrayBuffer* buffer = new Buffer<std::string>(n, alloc);
    std::string* ptr = buffer->base<std::string>();
    // Read string list
    if (ptr == nullptr || !DecodeStringList(src.data(), n, ptr)) {
      // Allocation / Decoding went wrong, release buffer
      buffer->Dec();
      return nullptr;
    }
    return buffer;
  }
};
  
/**
 *  @name   FromProtoToDataType
 *  @fn     DataType FromProtoToDataType(const ProtoDataType& dtype)
 *  @brief  Convert a ProtoDataType into the corresponding DataType
 *  @return DataType value
 */
DataType FromProtoToDataType(const ProtoDataType& dtype) {
  switch (dtype) {
    case ProtoDataType::kUnknown: return DataType::kUnknown;
    case ProtoDataType::kInt8: return DataType::kInt8;
    case ProtoDataType::kUInt8: return DataType::kUInt8;
    case ProtoDataType::kInt16: return DataType::kInt16;
    case ProtoDataType::kUInt16: return DataType::kUInt16;
    case ProtoDataType::kInt32: return DataType::kInt32;
    case ProtoDataType::kUInt32: return DataType::kUInt32;
    case ProtoDataType::kInt64: return DataType::kInt64;
    case ProtoDataType::kUInt64: return DataType::kUInt64;
    case ProtoDataType::kFloat: return DataType::kFloat;
    case ProtoDataType::kDouble: return DataType::kDouble;
    case ProtoDataType::kBool: return DataType::kBool;
    case ProtoDataType::kString: return DataType::kString;
    default: return DataType::kUnknown;
  }
  return DataType::kUnknown;
}

/**
 *  @name   FromDataTypeToProto
 *  @fn     ProtoDataType FromDataTypeToProto(const DataType& dtype)
 *  @brief  Convert a DataType into the corresponding ProtoDataType
 *  @return ProtoDataType value
 */
ProtoDataType FromDataTypeToProto(const DataType& dtype) {
  switch (dtype) {
    case DataType::kUnknown: return ProtoDataType::kUnknown;
    case DataType::kInt8: return ProtoDataType::kInt8;
    case DataType::kUInt8: return ProtoDataType::kUInt8;
    case DataType::kInt16: return ProtoDataType::kInt16;
    case DataType::kUInt16: return ProtoDataType::kUInt16;
    case DataType::kInt32: return ProtoDataType::kInt32;
    case DataType::kUInt32: return ProtoDataType::kUInt32;
    case DataType::kInt64: return ProtoDataType::kInt64;
    case DataType::kUInt64: return ProtoDataType::kUInt64;
    case DataType::kFloat: return ProtoDataType::kFloat;
    case DataType::kDouble: return ProtoDataType::kDouble;
    case DataType::kBool: return ProtoDataType::kBool;
    case DataType::kString: return ProtoDataType::kString;
    default: return ProtoDataType::kUnknown;
  }
  return ProtoDataType::kUnknown;
}
  
  

#pragma mark -
#pragma mark Macro definition
  

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
    CASE(int64_t, ARG(CCODE))                                   \
    CASE(uint64_t, ARG(CCODE))                                  \
    CASE(float, ARG(CCODE))                                     \
    CASE(double, ARG(CCODE))                                    \
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
 *  @name   DeepCopy
 *  @fn     void DeepCopy(NDArray* other) const
 *  @brief  Perform a deep copy into an `other` array. Underlying buffer will
 *          not be shared.
 *  @param[out] other Where to copy the array
 */
void NDArray::DeepCopy(NDArray* other) const {
  // Init target
  other->Resize(type_, dims_);
  // Copy data
  SWITCH_WITH_DEFAULT(type_,
                      internal::Initializer<T>::FromArray(this->Base<const T>(),
                                                          dims_.n_elems(),
                                                          other->Base<T>()),
                      FACEKIT_LOG_ERROR("Unknown data type: " << (int)type_),
                      FACEKIT_LOG_ERROR("Data type not set"));
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
                        FACEKIT_LOG_ERROR("Unknown data type: " << (int)type_),
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
  if (IsInitialized()) {
    // Do conversion
    // Type
    proto->set_type(FromDataTypeToProto(type_));
    // Shape
    dims_.ToProto(proto->mutable_dims());
    // Buffer if any
    if (buffer_) {
      // Fill proto object
      SWITCH_WITH_DEFAULT(type_,
                          ProtoStream<T>::Write(buffer_, dims_.n_elems(), proto),
                          FACEKIT_LOG_ERROR("Unknown data type: " << (int)type_),
                          FACEKIT_LOG_ERROR("Data type not set"));
    }
  } else {
    FACEKIT_LOG_DEBUG("Can not convert to protobuf object uninitialized array");
  }
}

/*
 *  @name   FromProto
 *  @fn     Status FromProto(const NDArrayProto& proto)
 *  @brief  Fill this NDArray from a given Protocol Buffer Object.
 *  @param[in] proto  Protobuf object holding NDArray
 *  @return Operation status
 */
Status NDArray::FromProto(const NDArrayProto& proto) {
  return FromProto(proto, DefaultCpuAllocator());
}

/*
 *  @name   FromProto
 *  @fn     Status FromProto(const NDArrayProto& proto)
 *  @brief  Fill this NDArray from a given Protocol Buffer Object.
 *  @return Operation status
 */
Status NDArray::FromProto(const NDArrayProto& proto, Allocator* allocator) {
  // Check type
  auto type = FromProtoToDataType(proto.type());
  if (type == DataType::kUnknown) {
    return Status(Status::Type::kInvalidArgument,
                  "Unknown data type in protobuf object");
  }
  // Check dimensions
  if(!NDArrayDims::IsValid(proto.dims())) {
    return Status(Status::Type::kInvalidArgument,
                  "NDArray dimensions in protobuf object are not valid");
  }
  // Try to Load array
  NDArrayDims dims(proto.dims());
  size_t n_elem = dims.n_elems();
  NDArrayBuffer* buff = nullptr;
  if (n_elem > 0) {
    bool err = false;
    if (!proto.data().empty()) {
      SWITCH_WITH_DEFAULT(type,
                          buff = ProtoStream<T>::Read(proto, n_elem, allocator),
                          err = true,
                          err = true);
    }
    if (err || buff == nullptr) {
      return Status(Status::Type::kInvalidArgument,
                    "Error while reading array data from protobuf object");
    }
  }
  // Reach here, data were read correctly -> Can init array content
  dims_ = dims;
  type_ = type;
  allocator_ = allocator;
  if (buffer_) {
    buffer_->Dec();
  }
  buffer_ = buff;
  return Status();
}

#pragma mark -
#pragma mark Usage

/*
 *  @name   ShareBuffer
 *  @fn     bool ShareBuffer(const NDArray& other) const
 *  @brief  Check if two arrays share the same underlying buffer
 */
bool NDArray::ShareBuffer(const NDArray& other) const {
  //assert(buffer_ != nullptr && other.buffer_ != nullptr);
  if (buffer_ != nullptr || other.buffer_ != nullptr) {
    return buffer_ == other.buffer_;
  }
  return false;
}

/*
 *  @name   Slice
 *  @fn     NDArray Slice(const size_t& start, const size_t& stop) const
 *  @brief  Create an array being a  subregion of this array.
 *  @param[in] start  Begining of the subregion
 *  @param[in] stop   End of the subregion algon first dimension (i.e. dim0)
 *  @return Subregion's array.
 */
NDArray NDArray::Slice(const size_t& start, const size_t& stop) const {
  assert(dims() >= 1);
  assert(start >= 0);
  size_t dim0 = dim_size(0);
  assert(stop <= dim0);
  // Ask for the whole array ? -> trivial case
  if (start == 0 && stop == dim0) {
    return *this;
  }

  // Create new array
  NDArray array;
  array.type_ = type_;
  array.dims_ = dims_;
  array.buffer_ = nullptr;
  if (dim0 > 0) {
    const size_t elem_per_dim0 = n_elems() / dim0;
    const size_t delta = start * elem_per_dim0;
    dim0 = stop - start;
    array.dims_.set_dim(0, dim0);
    const size_t n_elem = dim0 * elem_per_dim0;
    if (buffer_) {
      SWITCH_WITH_DEFAULT(type_,
                          array.buffer_ = new SubBuffer<T>(buffer_, delta, n_elem),
                          FACEKIT_LOG_ERROR("Unknown data type: " << (int)type_),
                          FACEKIT_LOG_ERROR("Data type not set"));
    }
  }
  return array;
}


}  // namespace FaceKit

