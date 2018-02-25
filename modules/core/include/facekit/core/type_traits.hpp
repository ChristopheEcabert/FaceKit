/**
 *  @file   type_traits.hpp
 *  @brief Templated traits
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   24.02.18
 *    Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_TYPE_TRAITS__
#define __FACEKIT_TYPE_TRAITS__

#include "facekit/core/library_export.hpp"
#include "facekit/core/nd_array_map.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark Base type detection
  
  
  
#pragma mark -
#pragma mark Mapping type
  
/**
 *  @struct  NDATypes
 *  @brief  Helper structure defining common NDArray sub-types
 *  @author Christophe Ecabert
 *  @date   24.02.18
 *  @ingroup core
 *  @tparam T Data type
 *  @tparam NDIMS  NDArray dimensions
 */
template<typename T, size_t NDIMS = 1>
struct NDATypes {
  /** Rank <NDIMS> NDArray map of T type */
  using NDArray = NDArrayMap<T, NDIMS>;
  
  /** Rank <NDIMS> NDArray map ot const T type */
  using ConstNDArray = NDArrayMap<const T, NDIMS>;
  
  /** Rank 0 - Scalar of T type */
  using Scalar = NDArrayMap<T, 0>;
  /** Rank 0 - Scalar of const T type */
  using ConstScalar = NDArrayMap<const T, 0>;
  
  /** Rank 1 - flatten NDArray of T type */
  using Flat = NDArrayMap<T, 1>;
  /** Rank 1 - flatten NDArray of const T type */
  using ConstFlat = NDArrayMap<T, 1>;
  
  /** Rank 1 - Vector of T type */
  using Vector = NDArrayMap<T, 1>;
  /** Rank 1 - Vector of const T type */
  using ConstVector = NDArrayMap<T, 1>;
  
  /** Rank 2 - Matrix of T type */
  using Matrix = NDArrayMap<T, 2>;
  /** Rank 2 - Matrix of const T type */
  using ConstMatrix = NDArrayMap<T, 2>;
};

}  // namespace FaceKit
#endif /* __FACEKIT_type_traits__ */
