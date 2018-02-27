/**
 *  @file   nd_array.hpp
 *  @brief Class for ND-Array implementation. Inspired by Tensorflow
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   23.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_ND_ARRAY__
#define __FACEKIT_ND_ARRAY__

#include <cassert>

#include "facekit/core/library_export.hpp"
#include "facekit/core/types.hpp"
#include "facekit/core/refcounter.hpp"
#include "facekit/core/nd_array_dims.hpp"
#include "facekit/core/mem/allocator.hpp"
#include "facekit/core/type_traits.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/** NDArrayProto */
class NDArrayProto;
  
  
/**
 *  @class  NDArrayBuffer
 *  @brief  Storage space interface for NDArray container
 *  @author Christophe Ecabert
 *  @date   23.02.18
 *  @ingroup core
 */
class FK_EXPORTS NDArrayBuffer : public RefCounter {
 public:
  
  /**
   *  @name   ~NDArrayBuffer
   *  @fn     ~NDArrayBuffer(void)
   *  @brief  Destructor
   */
  ~NDArrayBuffer(void) override {};
  
  /**
   *  @name   data
   *  @fn     virtual void* data(void) const = 0
   *  @brief  Pointer to the buffer storing data of a given `size` in bytes
   *  @return Buffer address
   */
  virtual void* data(void) const = 0;
  
  /**
   *  @name   size
   *  @fn     virtual size_t size(void) const = 0
   *  @brief  Buffer dimension in bytes
   *  @return Number of bytes in the buffer
   */
  virtual size_t size(void) const = 0;
  
  /**
   *  @name   root
   *  @fn     virtual NDArrayBuffer* root(void) = 0
   *  @brief  Provide reference to the buffer interface, most of the time it 
   *          return the address of `this` however when the buffer is a
   *          subpart of another one, it will point to this one.
   */
  virtual NDArrayBuffer* root(void) = 0;
  
  /**
   *  @name   base
   *  @fn     T* base(void) const
   *  @brief  Convinent method to access buffer data for a specific type.
   */
  template<typename T>
  T* base(void) const {
    return reinterpret_cast<T*>(data());
  }
};
  
  
/**
 *  @class  NDArray
 *  @brief  Representation of a n-dimension array storing values of a 
 *          given types.
 *  @author Christophe Ecabert
 *  @date   23.02.18
 *  @ingroup core
 */
class FK_EXPORTS NDArray {
 public:
  
#pragma mark -
#pragma mark Initialization
  
  /**
   *  @name   NDArray
   *  @fn     NDArray(void)
   *  @brief  Constructor, create an empty NDArray container with 0 element 
   *          inside.
   */
  NDArray(void);
  
  /**
   *  @name   NDArray
   *  @fn     explicit NDArray(Allocator* allocator)
   *  @brief  Constructor, create an empty NDArray container with 0 element
   *          inside.
   *  @param[in] allocator  Allocator to use when create internal buffer
   */
  explicit NDArray(Allocator* allocator);
  
  /**
   *  @name   NDArray
   *  @fn     NDArray(const DataType& type, const NDArrayDims& dims)
   *  @brief  Constructor, create a container of a specific type with given
   *          dimensions. The buffer is allocated with default cpu allocator.
   *  @param[in] type Data type to store
   *  @param[in] dims Array dimensions
   */
  NDArray(const DataType& type, const NDArrayDims& dims);
  
  /**
   *  @name   NDArray
   *  @fn     NDArray(const DataType& type, const NDArrayDims& dims)
   *  @brief  Constructor, create a container of a specific type with given
   *          dimensions. The buffer is allocated with a given allocator 
   *          (should last longer than the lifetime of this container).
   *  @param[in] type Data type to store
   *  @param[in] dims Array dimensions
   *  @param[in] allocator  Allocator to use to instantiate buffer.
   */
  NDArray(const DataType& type, const NDArrayDims& dims, Allocator* allocator);
  
  /**
   *  @name   NDArray
   *  @fn     NDArray(const NDArray& other)
   *  @brief  Copy constructor
   *  @param[in] other  Object to copy from
   */
  NDArray(const NDArray& other);
  
  /**
   *  @name   NDArray
   *  @fn     NDArray(NDArray&& other)
   *  @brief  Move constructor
   *  @param[in] other  Object to move from
   */
  NDArray(NDArray&& other);
  
  /**
   *  @name   operator=
   *  @fn     NDArray& operator=(const NDArray& rhs)
   *  @brief  Copy Assignment operator
   *  @param[in] rhs  Object to assign from
   *  @return Newly assigned object
   */
  NDArray& operator=(const NDArray& rhs);

  /**
   *  @name   operator=
   *  @fn     NDArray& operator=(NDArray&& rhs)
   *  @brief  Move Assignment operator
   *  @param[in] rhs  Object to move-assign from
   *  @return Newly moved assign object
   */
  NDArray& operator=(NDArray&& rhs);
  
  /**
   *  @name   ~NDArray
   *  @fn     ~NDArray(void)
   *  @brief  Destructor
   */
  ~NDArray(void);
  
  /**
   *  @name   Resize
   *  @fn     void Resize(const DataType& type, const NDArrayDims& dims)
   *  @brief  Resize the NDArray to new dimensions/type
   */
  void Resize(const DataType& type, const NDArrayDims& dims);
  
  /**
   *  @name   ToProto
   *  @fn     void ToProto(NDArrayProto* proto) const
   *  @brief  Export the NDArray to a Protocol buffer object
   *  @param[out] proto Protocol Buffer Object to write into
   */
  void ToProto(NDArrayProto* proto) const;
  
  /**
   *  @name   FromProto
   *  @fn     Status FromProto(const NDArrayProto& proto)
   *  @brief  Fill this NDArray from a given Protocol Buffer Object.
   *  @return Operation status
   */
  Status FromProto(const NDArrayProto& proto);
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name   IsInitialized
   *  @fn     bool IsInitialized(void) const
   *  @brief  Indicate if array as been initialized
   *  @return True if initialized, false otherwise
   */
  bool IsInitialized(void) const {
    return (buffer_ != nullptr) && (buffer_->data() != nullptr);
  }
  
  /**
   *  @name   AsScalar
   *  @tparam T Data type
   *  @fn     typename NDATypes<T>::Scalar AsScalar(void)
   *  @brief  Access the array's data as a scalar. Used when dimensions and 
   *          type of the array is known
   *  @return Array mapped as a scalar
   */
  template<typename T>
  typename NDATypes<T>::Scalar AsScalar(void);
  
  /**
   *  @name   AsVector
   *  @tparam T Data type
   *  @fn     typename NDATypes<T>::Vector AsVector(void)
   *  @brief  Access the array's data as a vector. Used when dimensions and
   *          type of the array is known
   *  @return Array mapped as a vector
   */
  template<typename T>
  typename NDATypes<T>::Vector AsVector(void);
  
  /**
   *  @name   AsMatrix
   *  @tparam T Data type
   *  @fn     typename NDATypes<T>::Matrix AsMatrix(void)
   *  @brief  Access the array's data as a matrix. Used when dimensions and
   *          type of the array is known
   *  @return Array mapped as a matrix
   */
  template<typename T>
  typename NDATypes<T>::Matrix AsMatrix(void);
  
  /**
   *  @name   AsNDArray
   *  @tparam T Data type
   *  @tparam NDIMS Array dimensions
   *  @fn     typename NDATypes<T, NDIMS>::NDArray AsNDArray(void)
   *  @brief  Access the array's data as a nd-array. Used when dimensions and
   *          type of the array is known
   *  @return Array mapped as a nd-array
   */
  template<typename T, size_t NDIMS>
  typename NDATypes<T, NDIMS>::NDArray AsNDArray(void);
  
  /**
   *  @name   AsFlat
   *  @tparam T Data type
   *  @fn     typename NDATypes<T>::Flat AsFlat(void)
   *  @brief  Access the array's data as a flatten array.
   *  @return Array mapped as a flatten array
   */
  template<typename T>
  typename NDATypes<T>::Flat AsFlat(void);
  
  /**
   *  @name   AsScalar
   *  @tparam T Data type
   *  @fn     typename NDATypes<T>::ConstScalar AsScalar(void) const
   *  @brief  Access the array's data as a scalar. Used when dimensions and
   *          type of the array is known
   *  @return Array mapped as a scalar
   */
  template<typename T>
  typename NDATypes<T>::ConstScalar AsScalar(void) const;
  
  /**
   *  @name   AsVector
   *  @tparam T Data type
   *  @fn     typename NDATypes<T>::ConstVector AsVector(void) const
   *  @brief  Access the array's data as a vector. Used when dimensions and
   *          type of the array is known
   *  @return Array mapped as a vector
   */
  template<typename T>
  typename NDATypes<T>::ConstVector AsVector(void) const;
  
  /**
   *  @name   AsMatrix
   *  @tparam T Data type
   *  @fn     typename NDATypes<T>::ConstMatrix AsMatrix(void) const
   *  @brief  Access the array's data as a matrix. Used when dimensions and
   *          type of the array is known
   *  @return Array mapped as a matrix
   */
  template<typename T>
  typename NDATypes<T>::ConstMatrix AsMatrix(void) const;
  
  /**
   *  @name   AsNDArray
   *  @tparam T Data type
   *  @tparam NDIMS Array dimensions
   *  @fn     typename NDATypes<T, NDIMS>::ConstNDArray AsNDArray(void) const
   *  @brief  Access the array's data as a nd-array. Used when dimensions and
   *          type of the array is known
   *  @return Array mapped as a nd-array
   */
  template<typename T, size_t NDIMS>
  typename NDATypes<T, NDIMS>::ConstNDArray AsNDArray(void) const;
  
  /**
   *  @name   AsFlat
   *  @tparam T Data type
   *  @fn     typename NDATypes<T>::ConstFlat AsFlat(void) const
   *  @brief  Access the array's data as a flatten array.
   *  @return Array mapped as a flatten array
   */
  template<typename T>
  typename NDATypes<T>::ConstFlat AsFlat(void) const;
  
#pragma mark -
#pragma mark Accessors
  
  /**
   *  @name   type
   *  @fn     DataType type(void) const
   *  @brief  Underlying type stored in the array
   *  @return Data type
   */
  DataType type(void) const {
    return type_;
  }
  
  /**
   *  @name   dims
   *  @fn     size_t dims(void) const
   *  @brief  Number of dimensions for this arrays
   *  @return Number of dimensions
   */
  size_t dims(void) const {
    return dims_.dims();
  }
  
  /**
   *  @name   dim_size
   *  @fn     size_t dim_size(const size_t axis) const
   *  @brief  Provide dimension along a given axis
   *  @return Axis dimensions
   */
  size_t dim_size(const size_t axis) const {
    return dims_.dim_size(axis);
  }
  
  /**
   *  @name   n_elems
   *  @fn     size_t n_elems(void) const
   *  @brief  Number total of elements stored in the array (i.e. capacity)
   *  @return Number of elements
   */
  size_t n_elems(void) const {
    return dims_.n_elems();
  }
  
#pragma mark -
#pragma mark Private
  
 private:
  
  /**
   *  @name   Base
   *  @fn     T* Base(void) const
   *  @tparam T Data type
   *  @brief  Access underlying buffer base address of a given type T
   *  @return Buffer address or nullptr
   */
  template<typename T>
  T* Base(void) const;
  
  /** Buffer*/
  NDArrayBuffer* buffer_;
  /** Allocator */
  Allocator* allocator_;
  /** Dimensions */
  NDArrayDims dims_;
  /** Data type */
  DataType type_;
};
  
  
  
  
#pragma mark -
#pragma mark Implementation

/*
 *  @name   NDArray
 *  @fn     NDArray(const NDArray& other)
 *  @brief  Copy constructor
 *  @param[in] other  Object to copy from
 */
inline NDArray::NDArray(const NDArray& other) : buffer_(other.buffer_),
                                                allocator_(other.allocator_),
                                                dims_(other.dims_),
                                                type_(other.type_) {
  if(buffer_) {
    buffer_->Inc(); // Buffer increase ref counter
  }
}

/*
 *  @name   NDArray
 *  @fn     NDArray(NDArray&& other)
 *  @brief  Move constructor
 *  @param[in] other  Object to move from
 */
inline NDArray::NDArray(NDArray&& other) : buffer_(other.buffer_),
                                           allocator_(other.allocator_),
                                           dims_(std::move(other.dims_)),
                                           type_(other.type_) {
  other.buffer_ = nullptr;
}
  
/*
 *  @name   operator=
 *  @fn     NDArray& operator=(const NDArray& rhs)
 *  @brief  Copy Assignment operator
 *  @param[in] rhs  Object to assign from
 *  @return Newly assigned object
 */
inline NDArray& NDArray::operator=(const NDArray& rhs) {
  if (this != &rhs) {
    type_ = rhs.type_;
    dims_ = rhs.dims_;
    if (buffer_ != rhs.buffer_) {
      // Is a buffer attach to this ? If yes decrease ref counter
      if(buffer_) {
        buffer_->Dec();
      }
      // Define new buffer from other
      buffer_ = rhs.buffer_;
      if (buffer_) {
        buffer_->Inc(); // Increase ref counter
      }
    }
    // Take allocator has well
    allocator_ = rhs.allocator_;
  }
  return *this;
}
  
/*
 *  @name   operator=
 *  @fn     NDArray& operator=(NDArray&& rhs)
 *  @brief  Move Assignment operator
 *  @param[in] rhs  Object to move-assign from
 *  @return Newly moved assign object
 */
inline NDArray& NDArray::operator=(NDArray&& rhs) {
  if(this != &rhs) {
    type_ = rhs.type_;
    dims_ = std::move(rhs.dims_);
    // Is a buffer attach to this ? If yes decrease ref counter
    if(buffer_) {
      buffer_->Dec();
    }
    // Define new buffer from other, take ownership
    buffer_ = rhs.buffer_;
    rhs.buffer_ = nullptr;
    // Take allocator has well
    allocator_ = rhs.allocator_;
    rhs.allocator_ = nullptr;
  }
  return *this;
}
  
/*
 *  @name   Base
 *  @fn     T* Base(void) const
 *  @tparam T Data type
 *  @brief  Access underlying buffer base address of a given type T
 *  @return Buffer address or nullptr
 */
template<typename T>
inline T* NDArray::Base(void) const {
  return buffer_ == nullptr ? nullptr : buffer_->base<T>();
}
  
/*
 *  @name   AsScalar
 *  @tparam T Data type
 *  @fn     typename NDATypes<T>::Scalar AsScalar(void)
 *  @brief  Access the array's data as a scalar. Used when dimensions and
 *          type of the array is known
 *  @return Array mapped as a scalar
 */
template<typename T>
typename NDATypes<T>::Scalar NDArray::AsScalar(void) {
  assert(dims() == 0);
  return typename NDATypes<T>::Scalar(dims_, Base<T>());
}

/*
 *  @name   AsVector
 *  @tparam T Data type
 *  @fn     typename NDATypes<T>::Vector AsVector(void)
 *  @brief  Access the array's data as a vector. Used when dimensions and
 *          type of the array is known
 *  @return Array mapped as a vector
 */
template<typename T>
typename NDATypes<T>::Vector NDArray::AsVector(void) {
  assert(dims() == 1);
  return typename NDATypes<T>::Vector(dims_, Base<T>());
}

/*
 *  @name   AsMatrix
 *  @tparam T Data type
 *  @fn     typename NDATypes<T>::Matrix AsMatrix(void)
 *  @brief  Access the array's data as a matrix. Used when dimensions and
 *          type of the array is known
 *  @return Array mapped as a matrix
 */
template<typename T>
typename NDATypes<T>::Matrix NDArray::AsMatrix(void) {
  assert(dims() == 2);
  return typename NDATypes<T>::Matrix(dims_, Base<T>());
}

/*
 *  @name   AsMatrix
 *  @tparam T Data type
 *  @tparam NDIMS Array dimensions
 *  @fn     typename NDATypes<T, NDIMS>::NDArray AsNDArray(void)
 *  @brief  Access the array's data as a nd-array. Used when dimensions and
 *          type of the array is known
 *  @return Array mapped as a nd-array
 */
template<typename T, size_t NDIMS>
typename NDATypes<T, NDIMS>::NDArray NDArray::AsNDArray(void) {
  assert(dims() > 2);
  return typename NDATypes<T, NDIMS>::NDArray(dims_, Base<T>());
}

/*
 *  @name   AsFlat
 *  @tparam T Data type
 *  @fn     typename NDATypes<T>::Flat AsFlat(void)
 *  @brief  Access the array's data as a flatten array.
 *  @return Array mapped as a flatten array
 */
template<typename T>
typename NDATypes<T>::Flat NDArray::AsFlat(void) {
  return typename NDATypes<T>::Flat(Base<T>(), dims_.n_elems());
}
  
/*
 *  @name   AsScalar
 *  @tparam T Data type
 *  @fn     typename NDATypes<T>::ConstScalar AsScalar(void) const
 *  @brief  Access the array's data as a scalar. Used when dimensions and
 *          type of the array is known
 *  @return Array mapped as a scalar
 */
template<typename T>
typename NDATypes<T>::ConstScalar NDArray::AsScalar(void) const {
  assert(dims() == 0);
  return typename NDATypes<T>::ConstScalar(dims_, Base<const T>());

}
  
/*
 *  @name   AsVector
 *  @tparam T Data type
 *  @fn     typename NDATypes<T>::ConstVector AsVector(void) const
 *  @brief  Access the array's data as a vector. Used when dimensions and
 *          type of the array is known
 *  @return Array mapped as a vector
 */
template<typename T>
typename NDATypes<T>::ConstVector NDArray::AsVector(void) const {
  assert(dims() == 1);
  return typename NDATypes<T>::ConstVector(dims_, Base<const T>());
}
  
/*
 *  @name   AsMatrix
 *  @tparam T Data type
 *  @fn     typename NDATypes<T>::ConstMatrix AsMatrix(void) const
 *  @brief  Access the array's data as a matrix. Used when dimensions and
 *          type of the array is known
 *  @return Array mapped as a matrix
 */
template<typename T>
typename NDATypes<T>::ConstMatrix NDArray::AsMatrix(void) const {
  assert(dims() == 2);
  return typename NDATypes<T>::ConstMatrix(dims_, Base<const T>());
}
  
/*
 *  @name   AsNDArray
 *  @tparam T Data type
 *  @tparam NDIMS Array dimensions
 *  @fn     typename NDATypes<T, NDIMS>::ConstNDArray AsNDArray(void) const
 *  @brief  Access the array's data as a nd-array. Used when dimensions and
 *          type of the array is known
 *  @return Array mapped as a nd-array
 */
template<typename T, size_t NDIMS>
typename NDATypes<T, NDIMS>::ConstNDArray NDArray::AsNDArray(void) const {
  assert(dims() > 2);
  return typename NDATypes<T, NDIMS>::ConstNDArray(dims_, Base<const T>());
}
  
/*
 *  @name   AsFlat
 *  @tparam T Data type
 *  @fn     typename NDATypes<T>::ConstFlat AsFlat(void) const
 *  @brief  Access the array's data as a flatten array.
 *  @return Array mapped as a flatten array
 */
template<typename T>
typename NDATypes<T>::ConstFlat NDArray::AsFlat(void) const {
  return typename NDATypes<T>::ConstFlat(Base<const T>(), dims_.n_elems());
}
  
  
}  // namespace FaceKit
#endif /* __FACEKIT_ND_ARRAY__ */
