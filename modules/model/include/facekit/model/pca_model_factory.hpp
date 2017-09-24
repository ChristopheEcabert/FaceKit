/**
 *  @file   pca_model_factory.hpp
 *  @brief  Manager for PCAModel based instance
 *  @ingroup    model
 *
 *  @author Christophe Ecabert
 *  @date   15/08/2017
 *  Copyright (c) 2017 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_PCA_MODEL_FACTORY__
#define __FACEKIT_PCA_MODEL_FACTORY__

#include "facekit/core/library_export.hpp"
#include "facekit/model/pca_model.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {

/**
 * @class   PCAModelFactory
 * @brief   Manager for PCAModel based instance
 * @author  Christophe Ecabert
 * @date    15/08/2017
 * @ingroup model
 * @tparam T    Data type
 */
template<typename T>
class FK_EXPORTS PCAModelFactory {
 public:

#pragma mark -
#pragma mark Type Definition

  /** Model Type */
  using Model = PCAModel<T>;

#pragma mark -
#pragma mark Initialization

  /**
   * @name  Get
   * @fn    static PCAModelFactory<T>& Get(void)
   * @brief Factory's accessors
   * @return    Single instance of PCAModelFactory
   */
  static PCAModelFactory<T>& Get(void);

  /**
   * @name  ~PCAModelFactory
   * @fn    ~PCAModelFactory(void) = default
   * @brief Destructor
   */
  ~PCAModelFactory(void) = default;

  /**
   * @name  PCAModelFactory
   * @fn    PCAModelFactory(const PCAModelFactory& other) = delete
   * @brief Copy constructor
   * @param[in] other Object to copy from
   */
  PCAModelFactory(const PCAModelFactory& other) = delete;

  /**
   * @name  operator=
   * @fn    PCAModelFactory& operator=(const PCAModelFactory& rhs) = delete
   * @brief Assignment operator
   * @param[in] rhs Object to assign from
   * @return    Newly assigned object
   */
  PCAModelFactory& operator=(const PCAModelFactory& rhs) = delete;

  /**
   * @name  PCAModelFactory
   * @fn    PCAModelFactory(const PCAModelFactory& other) = delete
   * @brief Copy constructor
   * @param[in] other Object to copy from
   */
  PCAModelFactory(PCAModelFactory&& other) = delete;

  /**
   * @name  operator=
   * @fn    PCAModelFactory& operator=(PCAModelFactory&& rhs) = delete
   * @brief Move assignment operator
   * @param[in] rhs Object to move assign from
   * @return    Newly moved-assign object
   */
  PCAModelFactory& operator=(PCAModelFactory&& rhs) = delete;

#pragma mark -
#pragma mark Usage

  /**
   * @name  CreateByName
   * @fn    Model* CreateByName(const std::string& name) const
   * @brief Create an object that match the provided \p name
   * @param[in] name Object name
   * @return    Instance corresponding to name or nullptr if name is unknown
   */
  Model* CreateByName(const std::string& name) const;

  /**
   * @name  Register
   * @fn    void Register(const PCAModelProxy<T>* proxy)
   * @brief Register a new \p proxy with the factory
   * @param[in] proxy Proxy to register
   */
  void Register(const PCAModelProxy<T>* proxy);

#pragma mark -
#pragma mark Private
 private:

  /**
   * @name  PCAModelFactory
   * @fn    PCAModelFactory(void) = default
   * @brief Constructor
   */
  PCAModelFactory(void) = default;

  /** Proxies */
  std::vector<const PCAModelProxy<T>*> proxies_;
};

}  // namepsace FaceKit
#endif //__FACEKIT_PCA_MODEL_FACTORY__
