/**
 *  @file   pca_model_factory.cpp
 *  @brief
 *  @ingroup
 *
 *  @author Christophe Ecabert
 *  @date   15/08/2017
 *  Copyright (c) 2017 Christophe Ecabert. All rights reserved.
 */

#include "facekit/model/pca_model_factory.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark Initialization

/*
 * @name  Get
 * @fn    static PCAModelFactory<T>& Get(void)
 * @brief Factory's accessors
 * @return    Single instance of PCAModelFactory
 */
template<typename T>
PCAModelFactory<T>& PCAModelFactory<T>::Get(void) {
  static PCAModelFactory<T> factory;
  return factory;
}

#pragma mark -
#pragma mark Usage

/*
 * @name  CreateByName
 * @fn    Model* CreateByName(const std::string& name) const
 * @brief Create an object that match the provided \p name
 * @param[in] name Object name
 * @return    Instance corresponding to name or nullptr if name is unknown
 */
template<typename T>
typename PCAModelFactory<T>::Model*
PCAModelFactory<T>::CreateByName(const std::string& name) const {
  Model* model = nullptr;
  const char* cname = name.c_str();
  for (auto& p : proxies_) {
    if (strcmp(cname, p->Name()) == 0) {
      model = p->Create();
      break;
    }
  }
  return model;
}

/*
 * @name  Register
 * @fn    void Register(const PCAModelProxy<T>* proxy)
 * @brief Register a new \p proxy with the factory
 * @param[in] proxy Proxy to register
 */
template<typename T>
void PCAModelFactory<T>::Register(const PCAModelProxy<T>* proxy) {
  proxies_.push_back(proxy);
}

#pragma mark -
#pragma mark Explicit instantiation

/** Float */
template class PCAModelFactory<float>;
/** Double */
template class PCAModelFactory<double>;

}  // namespace FaceKit
