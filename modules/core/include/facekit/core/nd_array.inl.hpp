/**
 *  @file   nd_array.inl.hpp
 *  @brief Inline implementation of NDArray
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   28.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_ND_ARRAY_INL__
#define __FACEKIT_ND_ARRAY_INL__

#pragma mark -
#pragma mark Initializer Helper

namespace internal {

template<typename T>
struct Initializer {

  static_assert(std::is_trivial<T>::value,
                "T is not trivial, specialization needed");

  /*
   * @name  FromArray
   * @fn    static void FromArray(const T* src, const size_t& n_elem, T* dst)
   * @brief Initialize array from another one
   * @param[in] src     Array storing value to initialize with
   * @param[in] n_elem  Number of element to copy
   * @param[out] dst    Array where to copy
   */
  static void FromArray(const T* src, const size_t& n_elem, T* dst) {
    std::copy_n(src, n_elem, dst);
  }

  /*
   * @name  FromVector
   * @fn    static void FromVector(const T* src, const size_t& n_elem, T* dst)
   * @brief Initialize array from another one
   * @param[in] src     Array storing value to initialize with
   * @param[in] n_elem  Number of element to copy
   * @param[out] dst    Array where to copy
   */
  static void FromVector(const std::vector<T>& src, T* dst) {
    std::copy_n(src.begin(), src.size(), dst);
  }
};

template<>
struct Initializer<bool> {

  /*
   * @name  FromArray
   * @fn    static void FromArray(const T* src, const size_t& n_elem, T* dst)
   * @brief Initialize array from another one
   * @param[in] src     Array storing value to initialize with
   * @param[in] n_elem  Number of element to copy
   * @param[out] dst    Array where to copy
   */
  static void FromArray(const bool* src, const size_t& n_elem, bool* dst) {
    std::copy_n(src, n_elem, dst);
  }

  /*
   * @name  FromVector
   * @fn    static void FromVector(const T* src, const size_t& n_elem, T* dst)
   * @brief Initialize array from another one
   * @param[in] src     Array storing value to initialize with
   * @param[in] n_elem  Number of element to copy
   * @param[out] dst    Array where to copy
   */
  static void FromVector(const std::vector<bool>& src, bool* dst) {
    for (size_t k = 0; k < src.size(); ++k, ++dst) {
      *dst = src[k];
    }
  }
};

template<>
struct Initializer<std::string> {
  /*
   * @name  FromArray
   * @fn    static void FromArray(const std::string* src, const size_t& n_elem,
   *                          std::string* dst)
   * @brief Initialize array from another one
   * @param[in] src     Array storing value to initialize with
   * @param[in] n_elem  Number of element to copy
   * @param[out] dst    Array where to copy
   */
  static void FromArray(const std::string* src,
                    const size_t& n_elem,
                    std::string* dst) {
    for (int k = 0; k < n_elem; ++k, ++src, ++dst) {
      *dst = *src;
    }
  }

  /*
   * @name  FromVector
   * @fn    static void FromVector(const std::vector<std::string>& src, std::string* dst)
   * @brief Initialize array from another one
   * @param[in] src     Array storing value to initialize with
   * @param[in] n_elem  Number of element to copy
   * @param[out] dst    Array where to copy
   */
  static void FromVector(const std::vector<std::string>& src,
                         std::string* dst) {
    for (size_t k = 0; k < src.size(); ++k, ++dst) {
      *dst = src[k];
    }
  }
};

}

#pragma mark -
#pragma mark Initialization

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
 *  @name   WithScalar
 *  @fn     static NDArray WithScalar(const T& value)
 *  @tparam T Data type
 *  @brief  Create a NDArray of rank0 from a given value
 *  @param[in] value  Scalar value to initialize with
 *  @return Scalar array (rank-0)
 */
template<typename T>
inline NDArray NDArray::WithScalar(const T &value) {
  NDArray array(DataTypeToEnum<T>::v(), {});
  array.AsScalar<T>()() = value;
  return array;
}

/*
 *  @name   WithValues
 *  @fn     static NDArray WithValues(const std::vector<T>& values)
 *  @tparam T Data type
 *  @brief  Create a NDArray of rank1 from a list of values
 *  @param[in] values List of value to initialize the array with.
 *  @return Vector array (rank-1)
 */
template<typename T>
inline NDArray NDArray::WithValues(const std::vector<T>& values) {
  NDArray array(DataTypeToEnum<T>::v(), {values.size()});
  internal::Initializer<T>::FromVector(values, array.AsVector<T>().data());
  return array;
}

/*
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
inline NDArray NDArray::WithValues(const std::vector<T>& values,
                                   const NDArrayDims& dims) {
  assert(dims.n_elems() == values.size());
  NDArray array(DataTypeToEnum<T>::v(), dims);
  internal::Initializer<T>::FromVector(values, array.AsFlat<T>().data());
  return array;
}

#pragma mark -
#pragma mark Usage

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

#pragma mark -
#pragma mark Private

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

#endif  // __FACEKIT_ND_ARRAY_INL__
