/**
 *  @file   pca_model.cpp
 *  @brief  Statistical model based on Principle Component Analysis
 *  @ingroup    model
 *
 *  @author Christophe Ecabert
 *  @date   15/08/2017
 *  Copyright (c) 2017 Christophe Ecabert. All rights reserved.
 */

#include <fstream>

#include "facekit/model/pca_model.hpp"
#include "facekit/model/pca_model_factory.hpp"
#include "facekit/core/math/linear_algebra.hpp"
#include "facekit/io/file_io.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {

#pragma mark -
#pragma mark Initialization

/*
 * @name  Load
 * @fn    virtual int Load(std::istream& stream)
 * @brief Load from a given binary \p stream
 * @param[in] stream Binary stream to load model from
 * @return    -1 if error, 0 otherwise
 */
template<typename T>
int PCAModel<T>::Load(std::istream& stream) {
  int err = -1;
  if (stream.good()) {
    // Load
    err = IO::LoadTypedMat<T>(stream, &mean_);
    err |= IO::LoadTypedMat<T>(stream, &variation_);
    err |= IO::LoadTypedMat<T>(stream, &prior_);
    // Channels
    stream.read(reinterpret_cast<char*>(&n_channels_), sizeof(n_channels_));
    // Init vars
    n_principle_component_ = variation_.cols;
    // Sanity check
    err |= stream.good() ? 0 : -1;
  }
  return err;
}

/*
 * @name  Save
 * @fn    virtual int Save(std::istream& stream) const
 * @brief Save to a given binary \p stream
 * @param[in] stream Binary stream to save model from
 * @return    -1 if error, 0 otherwise
 */
template<typename T>
int PCAModel<T>::Save(std::ostream& stream) const {
  int err = -1;
  if (stream.good()) {
    // Mean, var, prior
    err = IO::SaveMat(stream, mean_);
    err |= IO::SaveMat(stream, variation_);
    err |= IO::SaveMat(stream, prior_);
    // Channels
    stream.write(reinterpret_cast<const char*>(&n_channels_),
                 sizeof(n_channels_));
    // Sanity check
    err |= stream.good() ? 0 : -1;
  }
  return err;
}

/*
 * @name  ComputeObjectSize
 * @fn    virtual int ComputeObjectSize(void) const
 * @brief Compute object size in byte
 * @return    Object's size
 */
template<typename T>
size_t PCAModel<T>::ComputeObjectSize(void) const {
  size_t sz = 9 * sizeof(int);
  sz += mean_.total() * mean_.elemSize();
  sz += variation_.total() * variation_.elemSize();
  sz += prior_.total() * prior_.elemSize();
  sz += sizeof(n_channels_);
  return sz;
}

#pragma mark -
#pragma mark Usage

/*
 * @name  Generate
 * @fn    virtual void Generate(const cv::Mat& p, T* instance)
 * @brief Generate an instance given a set of coefficients \p p.
 * @param[in] p   Nodel's coefficients
 * @param[out] instance   Generated instance
 */
template<typename T>
void PCAModel<T>::Generate(const cv::Mat& p, T* instance) {
  using LA = typename FaceKit::LinearAlgebra<T>;
  using TType = typename FaceKit::LinearAlgebra<T>::TransposeType;
  // Init containter access
  cv::Mat out(mean_.rows,
              mean_.cols,
              cv::DataType<T>::type,
              (void*)instance);
  mean_.copyTo(out);
  // Generate instance
  static cv::Mat buff;
  LA::Sbmv(prior_, T(1.0), p, T(0.0), &buff);
  LA::Gemv(variation_,
           TType::kNoTranspose,
           T(1.0),
           buff,
           T(1.0),
           &out);
}

/*
 * @name  Generate
 * @fn    virtual void Generate(T* instance)
 * @brief Generate a random instance
 * @param[out] instance   Randomly generated instance
 */
template<typename T>
void PCAModel<T>::Generate(T* instance) {
  using LA = typename FaceKit::LinearAlgebra<T>;
  using TType = typename FaceKit::LinearAlgebra<T>::TransposeType;
  // Init containter access
  cv::Mat out(mean_.rows,
              mean_.cols,
              cv::DataType<T>::type,
              (void*)instance);
  mean_.copyTo(out);
  // Create random parameters
  cv::theRNG().state = static_cast<uint64_t>(cv::getTickCount());
  static cv::Mat p(variation_.cols, 1, cv::DataType<T>::type);
  cv::randn(p, T(0.0), T(1.0));
  // Generate instance
  static cv::Mat buff;
  LA::Sbmv(prior_, T(1.0), p, T(0.0), &buff);
  LA::Gemv(variation_,
           TType::kNoTranspose,
           T(1.0),
           buff,
           T(1.0),
           &out);
}

#pragma mark -
#pragma mark Proxy

/*
 * @name  PCAModelProxy
 * @fn    PCAModelProxy(void)
 * @brief Constructor
 */
template<typename T>
PCAModelProxy<T>::PCAModelProxy(void) {
  PCAModelFactory<T>::Get().Register(this);
}

#pragma mark -
#pragma mark Explicit instantiation

/** Float */
template class PCAModel<float>;
/** Double */
template class PCAModel<double>;

/** Float */
template class PCAModelProxy<float>;
/** Double */
template class PCAModelProxy<double>;

}  // namespace FaceKit
