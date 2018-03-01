/**
 *  @file   nd_array_dims.hpp
 *  @brief Representation for NDArray's dimensions. Inspired from Tensorflow
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   14.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_ND_ARRAY_DIMS__
#define __FACEKIT_ND_ARRAY_DIMS__

#include <initializer_list>
#include <array>
#include <vector>

#include "facekit/core/library_export.hpp"
#include "facekit/core/status.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/** Forward NDArrayDimsProto */
class NDArrayDimsProto;
  
/**
 *  @class  NDArrayDims
 *  @brief  Dimensions representation for NDArray
 *  @author Christophe Ecabert
 *  @date   14.02.18
 *  @ingroup core
 */
class FK_EXPORTS NDArrayDims {
 public:
  
#pragma mark -
#pragma mark Initialization
  
  /**
   *  @name   NDArrayDims
   *  @fn     NDArrayDims(void)
   *  @brief  Constructor
   */
  NDArrayDims(void);
  
  /**
   *  @name   NDArrayDims
   *  @fn     NDArrayDims(const std::initializer_list<size_t>& dims)
   *  @brief  Constructor
   *  @param[in] dims List of dimensions
   */
  NDArrayDims(const std::initializer_list<size_t>& dims);
  
  /**
   *  @name   NDArrayDims
   *  @fn     explicit NDArrayDims(const NDArrayDimsProto& proto)
   *  @brief  Constructor from protobuf object
   */
  explicit NDArrayDims(const NDArrayDimsProto& proto);
  
  /**
   *  @name   NDArrayDims
   *  @fn     NDArrayDims(const NDArrayDims& other) = default;
   *  @brief  Copy constructor
   *  @param[in]  other Object to copy from
   */
  NDArrayDims(const NDArrayDims& other) = default;
  
  /**
   *  @name   NDArrayDims
   *  @fn     NDArrayDims(NDArrayDims&& other) = default;
   *  @brief  Move constructor
   *  @param[in]  other Object to move from
   */
  NDArrayDims(NDArrayDims&& other) = default;
  
  /**
   *  @name   operator=
   *  @fn     NDArrayDims& operator=(const NDArrayDims& rhs) = default;
   *  @brief  Assignment operator
   *  @param[in] rhs  Object to assign from
   *  @return newly assigned object
   */
  NDArrayDims& operator=(const NDArrayDims& rhs) = default;
  
  /**
   *  @name   operator=
   *  @fn     NDArrayDims& operator=(NDArrayDims&& rhs)
   *  @brief  Move-assignment operator
   *  @param[in] rhs  Object to move-assign from
   *  @return newly moved-assign object
   */
  NDArrayDims& operator=(NDArrayDims&& rhs) = default;
  
  /**
   *  @name   ~NDArrayDims
   *  @fn     ~NDArrayDims(void) = default
   *  @brief  Destructor
   */
  ~NDArrayDims(void) = default;
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name   IsValid
   *  @fn     static bool IsValid(const NDArrayDimsProto& proto)
   *  @brief  Check if a given protobuf message is valid
   *  @param[in]  proto Message to check
   *  @return true if valid, false otherwise
   */
  static bool IsValid(const NDArrayDimsProto& proto);
  
  /**
   *  @name   ToProto
   *  @fn     void ToProto(NDArrayDimsProto* proto) const
   *  @brief  Export to a corresponding Protobuf object
   *  @param[out] proto Protobuf message object
   */
  void ToProto(NDArrayDimsProto* proto) const;
  
  /**
   *  @name   FromProto
   *  @fn     Status FromProto(const NDArrayDimsProto& proto)
   *  @brief  Initialize from protobuf message
   *  @param[in] proto  Message from which to initialize
   *  @return -1 if message is not valid, 0 otherwise
   */
  Status FromProto(const NDArrayDimsProto& proto);
  
  /**
   *  @name   AddDim
   *  @fn void AddDim(const size_t& dim)
   *  @brief  Add dimension at the end
   */
  void AddDim(const size_t& dim);
  
  /**
   *  @name   RemoveDim
   *  @fn     void RemoveDim(const size_t& axis)
   *  @brief  Remove dimension along a given `axis`
   *  @param[in]  axis  Dimension to remove
   */
  void RemoveDim(const size_t& axis);
  
  /**
   *  @name   Clear
   *  @fn     void Clear(void)
   *  @brief  Clear dimensions stored
   */
  void Clear(void);
  
#pragma mark -
#pragma mark Accessors
  
  /**
   *  @name   dims
   *  @fn     size_t dims(void) const
   *  @brief  Number of dimension in the array. 0 <= dims() < kMaxDim
   *  @return Number of dimensions
   */
  size_t dims(void) const {
    return dims_[kMaxDim];
  }
  
  /**
   *  @name   set_dim
   *  @fn     void set_dim(const size_t& axis, const size_t& dim)
   *  @brief  Set dimension for a given axis
   *  @param[in] axis Axis on which to set dimensions
   *  @param[in] dim  Dimension
   */
  void set_dim(const size_t& axis, const size_t& dim);
  
  /**
   *  @name   dim_size
   *  @fn     size_t dim_size(const size_t& dim) const
   *  @brief  Give the dimensions along a given `axis`
   *  @return Array dimension for a specific axis
   */
  size_t dim_size(const size_t& axis) const {
    return axis < dims() ? dims_[axis] : std::numeric_limits<size_t>::max();
  }
  
  /**
   *  @name   dim_sizes
   *  @fn     std::vector<size_t> dim_sizes(void) const
   *  @brief  Give all dimensions for this array
   *  @return List of dimensions
   */
  std::vector<size_t> dim_sizes(void) const {
    return std::vector<size_t>(dims_.begin(), dims_.begin() + kMaxDim);
  }
  
  /**
   *  @name   n_elems
   *  @fn     size_t n_elems(void) const
   *  @brief  Give the number of element in the array
   *  @return Number of element
   */
  size_t n_elems(void) const {
    return n_elem_;
  }
  
#pragma mark -
#pragma mark Private
 private:
  
  /**
   *  @name   ComputeNElement
   *  @fn     void ComputeNElement(void)
   *  @brief  Establish how many element are store in the array
   */
  void ComputeNElement(void);
  
  /** Maximum number of dimensions */
  static constexpr size_t kMaxDim = 4;
  /** Actual dimensions */
  std::array<size_t, 5> dims_;
  /** Number of element in the array */
  size_t n_elem_;
};
  
  
  
}  // namespace FaceKit
#endif /* __FACEKIT_ND_ARRAY_DIMS__ */
