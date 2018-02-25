/**
 *  @file   nd_array.cpp
 *  @brief Class for ND-Array implementation. Inspired by Tensorflow
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   23.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include "nd_array.pb.h"

#include "facekit/core/nd_array.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
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
  size_t size = dims_.n_elems() * sizeof(int);
  size_t wanted_size = dims.n_elems() * sizeof(char);
  if (buffer_ == nullptr || (size != wanted_size || type_ != type)) {
    // Release current buffer if any
    if (buffer_) {
      buffer_->Dec();
    }
    // Allocate buffer
    
    
    
    
    // Define type + size
    type_ = type;
    dims_ = dims;
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

