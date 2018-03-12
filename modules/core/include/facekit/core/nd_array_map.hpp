/**
 *  @file   nd_array_map.hpp
 *  @brief Interface with NDArray from raw buffers, similar to `Eigen::Map()`.
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   18.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_ND_ARRAY_MAP__
#define __FACEKIT_ND_ARRAY_MAP__

#include <cstdlib>
#include <array>
#include <initializer_list>
#include <type_traits>


#include "facekit/core/library_export.hpp"
#include "facekit/core/nd_array_dims.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  NDArrayMap
 *  @brief  Raw buffer interface for convenient element access
 *  @author Christophe Ecabert
 *  @date   18.02.18
 *  @tparam T Data type
 *  @tparam NDIMS Number of dimensions in the nd-array
 *  @ingroup core
 */
template<typename T, size_t NDIMS>
class FK_EXPORTS NDArrayMap {
 public:
  
#pragma mark -
#pragma mark Initialization
  
  /**
   *  @name   NDArrayMap
   *  @fn     NDArrayMap(const NDArrayDims& dims, T* ptr)
   *  @brief  Constructor
   *  @param[in] dims NDArray dimensions descriptor
   *  @param[in] ptr  Pointer to the raw buffer to map
   */
  NDArrayMap(const NDArrayDims& dims, T* ptr) : n_dims_(NDIMS), n_elem_(1), data_(ptr) {
    assert(dims.dims() == n_dims_);
    for (size_t i = 0; i < dims.dims(); ++i) {
      dims_[i] = dims.dim_size(i);
    }
    ComputeSteps();
  }
  
  /**
   *  @name   NDArrayMap
   *  @fn     NDArrayMap(T* ptr, Dims&&... dims)
   *  @brief  Constructor
   *  @param[in] ptr  Pointer to the raw buffer to map
   *  @param[in] dims NDArray's dimensions for each axis
   *  @tparam Dims  List of dimensions
   */
  template<typename... Dims>
  NDArrayMap(T* ptr, Dims&&... dims) : dims_{std::forward<size_t>(dims)...},
    n_dims_(NDIMS), n_elem_(1), data_(ptr) {
    static_assert(sizeof...(dims) == NDIMS, "Dimensions does not match");
    ComputeSteps();
  }
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name   operator()
   *  @fn     T& operator()()
   *  @brief  Access element at position `index`
   *  @return Scalar value
   */
  T& operator()();
  
  /**
   *  @name   operator()
   *  @fn     T& operator()(const size_t& index)
   *  @brief  Access element at position `index`
   *  @param[in]  index Position to access
   *  @return Value stored at `index`
   */
  T& operator()(const size_t& index);
  
  /**
   *  @name   operator()
   *  @fn     T& operator()(const size_t& index, const Indexes... indexes)
   *  @brief  Access element at position indexed: i,j,k,...
   *  @param[in] index  First index i (head)
   *  @param[in] indexes List of remaining indexes j,k,... (tail)
   */
  template<typename... Indexes>
  T& operator()(const size_t& index, const Indexes... indexes);
  
#pragma mark -
#pragma mark Accessors
  
  /**
   *  @name   rank
   *  @fn     size_t rank(void) const
   *  @brief  Give mapped array rank
   *  @return NDArray's rank
   */
  size_t rank(void) const {
    return n_dims_;
  }
  
  /**
   *  @name   dim
   *  @fn     size_t dim_size(const size_t& axis) const
   *  @brief  Dimension for a given `axis`
   *  @return Axis dimension
   */
  size_t dim_size(const size_t& axis) const {
    return dims_[axis];
  }
  
  /**
   *  @name   size
   *  @fn     size_t size(void) const
   *  @brief  Give NDArray total size/elements
   *  @return Total number of element in the mapped buffer
   */
  size_t size(void) const {
    return n_elem_;
  }
  
  /**
   *  @name   data
   *  @fn     Type* data(void)
   *  @brief  Access to the mapped raw buffer
   *  @return Mapped pointer
   */
  T* data(void) {
    return data_;
  }
  
  /**
   *  @name   data
   *  @fn     const Type* data(void) const
   *  @brief  Access to the mapped raw buffer
   *  @return Mapped pointer
   */
  T* data(void) const {
    return data_;
  }
  
#pragma mark -
#pragma mark Private
 private:
  
  /**
   *  @name   ComputeSteps
   *  @fn     void ComputeSteps(void)
   *  @brief  Initialize axis steps
   */
  void ComputeSteps(void);
  
  /**
   *  @name   FlatIndex
   *  @fn     size_t FlatIndex(const size_t& axis, const size_t& index) const
   *  @brief  Compute flat index for the mapped raw buffer
   *  @param[in] axis   Axis of interest
   *  @param[in] index  Dimension's index for a given `axis`
   *  @return Flat index
   */
  size_t FlatIndex(const size_t& axis, const size_t& index) const;
  
  /**
   *  @name   FlatIndex
   *  @fn     size_t FlatIndex(const size_t& axis, const size_t& index,
                               const Indexes... indexes) const
   *  tparam  Indexes Packed parameters
   *  @brief  Compute flat index for the mapped raw buffer
   *  @param[in] axis   Axis of interest
   *  @param[in] index  Dimension's index for a given `axis`
   *  @param[in] indexes  List of remaining index for other axis
   *  @return Flat index
   */
  template<typename... Indexes>
  size_t FlatIndex(const size_t& axis,
                   const size_t& index,
                   const Indexes... indexes) const;
  
  /** Dimensions */
  std::array<size_t, NDIMS> dims_;
  /** Step */
  std::array<size_t, NDIMS> steps_;
  /** Number of dimensions */
  const size_t n_dims_;
  /** Number total of element */
  size_t n_elem_;
  /** Reference to buffer */
  T* data_;
};
  
/*
 *  @name   ComputeSteps
 *  @fn     void ComputeSteps(void)
 *  @brief  Initialize axis steps
 */
template<typename T, size_t NDIMS>
inline void NDArrayMap<T, NDIMS>::ComputeSteps(void) {
  auto it = steps_.rbegin();
  for (auto dit = dims_.rbegin(); it != steps_.rend(); ++it, ++dit) {
    *it = n_elem_;
    n_elem_ *= *dit;
  }
}
  
/*
 *  @name   FlatIndex
 *  @fn     size_t FlatIndex(const size_t& index, const size_t& axis) const
 *  @brief  Compute flat index for the mapped raw buffer
 *  @param[in] index  Dimension's index for a given `axis`
 *  @param[in] axis   Axis of interest
 *  @return Flat index
 */
template<typename T, size_t NDIMS>
inline size_t NDArrayMap<T, NDIMS>::FlatIndex(const size_t& axis,
                                              const size_t& index) const {
  assert(index >= 0 && index < dims_[axis]);
  return index * steps_[axis];
}
  
/*
 *  @name   FlatIndex
 *  @fn     template<typename... Indexes>
            size_t FlatIndex(const size_t& axis, const size_t& index,
            const Indexes... indexes) const
 *  @brief  Compute flat index for the mapped raw buffer
 *  @param[in] axis   Axis of interest
 *  @param[in] index  Dimension's index for a given `axis`
 *  @param[in] indexes  List of remaining index for other axis
 *  @return Flat index
 */
template<typename T, size_t NDIMS>
template<typename... Indexes>
inline size_t NDArrayMap<T, NDIMS>::FlatIndex(const size_t& axis,
                                              const size_t& index,
                                              const Indexes... indexes) const {
  return FlatIndex(axis, index) + FlatIndex(axis + 1, indexes...);
}
  
/*
 *  @name   operator()
 *  @fn     T& operator()()
 *  @brief  Access element at position `index`
 *  @return Scalar value
 */
template<typename T, size_t NDIMS>
inline T& NDArrayMap<T, NDIMS>::operator()() {
  static_assert(NDIMS == 0,
                "Scalar operator does not work with more than one element");
  return data_[0];
}
  
/*
 *  @name   operator()
 *  @fn     T& operator()(const size_t& index)
 *  @brief  Access element at position `index`
 *  @param[in]  index Position to access
 *  @return Value stored at `index`
 */
template<typename T, size_t NDIMS>
inline T& NDArrayMap<T, NDIMS>::operator()(const size_t& index) {
  static_assert(NDIMS == 1,
                "Single index does not work with dimensions different that 1");
  size_t idx = FlatIndex(0, index);
  return data_[idx];
}
  
/*
 *  @name   operator()
 *  @fn     T& operator()(const size_t& index, const Indexes... indexes)
 *  @brief  Access element at position indexed: i,j,k,...
 *  @param[in] index  First index i (head)
 *  @param[in] indexes List of remaining indexes j,k,... (tail)
 */
template<typename T, size_t NDIMS>
template<typename... Indexes>
inline T& NDArrayMap<T, NDIMS>::operator()(const size_t& index,
                                           const Indexes... indexes) {
  static_assert(NDIMS == sizeof...(indexes) + 1,
                "Number of indices used to access a NDArray coefficient must be"
                "equal to the rank of the NDArray.");
  size_t idx = FlatIndex(0, index) + FlatIndex(1, indexes...);
  return data_[idx];
}

}  // namespace FaceKit
#endif /* __FACEKIT_ND_ARRAY_MAP__ */
