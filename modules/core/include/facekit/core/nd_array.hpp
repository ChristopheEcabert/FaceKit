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
#include <vector>
#include <type_traits>
#include <string>
#include <algorithm>

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
   *  @fn     NDArray(const DataType& type, const NDArrayDims& dims, Allocator* allocator)
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
   *  @name   DeepCopy
   *  @fn     void DeepCopy(NDArray* other) const
   *  @brief  Perform a deep copy into an `other` array. Underlying buffer will
   *          not be shared.
   *  @param[out] other Where to copy the array
   */
  void DeepCopy(NDArray* other) const;

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
   *  @param[in] proto  Protobuf object holding NDArray
   *  @return Operation status
   */
  Status FromProto(const NDArrayProto& proto);

  /**
   *  @name   FromProto
   *  @fn     Status FromProto(const NDArrayProto& proto, Allocator* allocator)
   *  @brief  Fill this NDArray from a given Protocol Buffer Object.
   *  @param[in] proto  Protobuf object holding NDArray
   *  @param[in] allocator  Memory allocator to use
   *  @return Operation status
   */
  Status FromProto(const NDArrayProto& proto, Allocator* allocator);

  /**
   *  @name   WithScalar
   *  @fn     static NDArray WithScalar(const T& value)
   *  @tparam T Data type
   *  @brief  Create a NDArray of rank0 from a given value
   *  @param[in] value  Scalar value to initialize with
   *  @return Scalar array (rank-0)
   */
  template<typename T>
  static NDArray WithScalar(const T& value);

  /**
   *  @name   WithValues
   *  @fn     static NDArray WithValues(const std::vector<T>& values)
   *  @tparam T Data type
   *  @brief  Create a NDArray of rank1 from a list of values
   *  @param[in] values List of value to initialize the array with.
   *  @return Vector array (rank-1)
   */
  template<typename T>
  static NDArray WithValues(const std::vector<T>& values);

  /**
   *  @name   WithValues
   *  @fn     static NDArray WithValues(const std::vector<T>& values,
                                        const NDArrayDims& dims)
   *  @tparam T Data type
   *  @brief  Create a NDArray of rank1 from a list of values
   *  @param[in] values List of value to initialize the array with.
   *  @param[in] dims   Array's dimensions, should match `values` size.
   *  @return Vector array (rank-N)
   */
  template<typename T>
  static NDArray WithValues(const std::vector<T>& values,
                            const NDArrayDims& dims);

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
   *  @name   ShareBuffer
   *  @fn     bool ShareBuffer(const NDArray& other) const
   *  @brief  Check if two arrays share the same underlying buffer
   */
  bool ShareBuffer(const NDArray& other) const;

  /**
   *  @name   Slice
   *  @fn     NDArray Slice(const size_t& start, const size_t& stop) const
   *  @brief  Create an array being a  subregion of this array.
   *  @param[in] start  Begining of the subregion
   *  @param[in] stop   End of the subregion algon first dimension (i.e. dim0)
   *  @return Subregion's array.
   */
  NDArray Slice(const size_t& start, const size_t& stop) const;

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
   *  @name   dimensions
   *  @fn     const NDArrayDims& dimensions(void) const
   *  @brief  Provide array's dimensions
   *  @return Dimensions
   */
  const NDArrayDims& dimensions(void) const {
    return dims_;
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

// Add implementation
#include "facekit/core/nd_array.inl.hpp"

}  // namespace FaceKit
#endif /* __FACEKIT_ND_ARRAY__ */
