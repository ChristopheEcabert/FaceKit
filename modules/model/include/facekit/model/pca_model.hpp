/**
 *  @file   pca_model.hpp
 *  @brief  Statistical model based on Principle Component Analysis
 *  @ingroup model
 *
 *  @author Christophe Ecabert
 *  @date   15/08/2017
 *  Copyright (c) 2017 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_PCA_MODEL__
#define __FACEKIT_PCA_MODEL__

#include <iostream>

#include "opencv2/core/core.hpp"

#include "facekit/core/library_export.hpp"
#include "facekit/io/serializable.hpp"
#include "facekit/geometry/mesh.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Devlopment space
 */
namespace FaceKit {

/**
 * @class   PCAModel
 * @author  Christophe Ecabert
 * @brief   Interface for PCA model, used for shape/texture modelisation
 * @date    15/08/2017
 * @ingroup model
 * @tparam T    Data type.
 */
template<typename T>
class FK_EXPORTS PCAModel : public Serializable {
 public:

#pragma mark -
#pragma mark Initialization

  /**
   * @name  ~PCAModel
   * @fn    virtual ~PCAModel(void) = default
   * @brief Destructor
   */
  virtual ~PCAModel(void) = default;

  /**
   * @name  Load
   * @fn    virtual int Load(std::istream& stream)
   * @brief Load from a given binary \p stream
   * @param[in] stream Binary stream to load model from
   * @return    -1 if error, 0 otherwise
   */
  virtual int Load(std::istream& stream);

  /**
   * @name  Save
   * @fn    virtual int Save(std::ostream& stream) const
   * @brief Save to a given binary \p stream
   * @param[in] stream Binary stream to save model from
   * @return    -1 if error, 0 otherwise
   */
  virtual int Save(std::ostream& stream) const;

  /**
   * @name  ComputeObjectSize
   * @fn    virtual size_t ComputeObjectSize(void) const
   * @brief Compute object size in byte
   * @return    Object's size
   */
  virtual size_t ComputeObjectSize(void) const;

#pragma mark -
#pragma mark Usage

  /**
   * @name  Generate
   * @fn    virtual void Generate(const cv::Mat& p, T* instance)
   * @brief Generate an instance given a set of coefficients \p p.
   * @param[in] p   Nodel's coefficients
   * @param[out] instance   Generated instance
   */
  virtual void Generate(const cv::Mat& p, T* instance);

  /**
   * @name  Generate
   * @fn    void Generate(const cv::Mat& p, Mesh<T>* instance)
   * @brief Generate an instance given a set of coefficients \p p.
   * @param[in] p   Nodel's coefficients
   * @param[out] instance   Generated instance
   */
  virtual void Generate(const cv::Mat& p, Mesh<T>* instance) = 0;

  /**
   * @name  Generate
   * @fn    virtual void Generate(T* instance)
   * @brief Generate a random instance
   * @param[out] instance   Randomly generated instance
   */
  virtual void Generate(T* instance);

  /**
   * @name  Generate
   * @fn    void Generate(Mesh<T>* instance)
   * @brief Generate a random instance
   * @param[out] instance   Randomly generated instance
   */
  virtual void Generate(Mesh<T>* instance) = 0;

#pragma mark -
#pragma mark Protected

  /**
   * @name  get_n_channels
   * @fn    virtual int get_n_channels(void) const
   * @brief Indicate how many channels are used
   * @return    Amount of channels
   */
  virtual int get_n_channels(void) const {
    return n_channels_;
  }

  /**
   * @name  get_n_principle_component
   * @fn    virtual int get_n_principle_component(void) const
   * @brief Indicate how many principle components are used
   * @return    Amount of principle component
   */
  virtual int get_n_principle_component(void) const {
    return n_principle_component_;
  }

#pragma mark -
#pragma mark Protected
 protected:
  /** Mean */
  cv::Mat mean_;
  /** Variation */
  cv::Mat variation_;
  /** Prior */
  cv::Mat prior_;
  /** Channels */
  int n_channels_;
  /** Number of frincipal components */
  int n_principle_component_;

};

/**
 * @class   PCAModelProxy
 * @brief   Registration mechanism for PCAModel type
 * @author  Christophe Ecabert
 * @date    15/08/2017
 * @ingroup model
 * @tparam T    Data type
 */
template<typename T>
class PCAModelProxy {
 public:

  /**
   * @name  PCAModelProxy
   * @fn    PCAModelProxy(void)
   * @brief Constructor
   */
  PCAModelProxy(void);

  /**
   * @name  ~PCAModelProxy(void)
   * @fn    virtual ~PCAModelProxy(void) = default
   * @brief Destructor
   */
  virtual ~PCAModelProxy(void) = default;

  /**
   * @name  Create
   * @fn    virtual PCAModel<T>* Create(void) const = 0
   * @brief Create a PCAModel instance for a given type
   * @return Instance of a given class
   */
  virtual PCAModel<T>* Create(void) const = 0;

  /**
   * @name  Name
   * @fn    virtual const char* Name(void) const = 0
   * @brief Provide the name represented by this proxy
   * @return    Object name
   */
  virtual const char* Name(void) const = 0;
};

}  // namepsace FaceKit

#endif //__FACEKIT_PCA_MODEL__
