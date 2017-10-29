/**
 *  @file   augmentation_engine.hpp
 *  @brief Pipeline for standardized data augmentation process
 *  @ingroup dataset
 *
 *  @author Christophe Ecabert
 *  @date   24.10.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_AUGMENTATION_ENGIN__
#define __FACEKIT_AUGMENTATION_ENGIN__

#include <string>
#include <vector>
#include <utility>

#include "facekit/core/library_export.hpp"
#include "facekit/dataset/augmentation_cell.hpp"
#include "facekit/dataset/flip_cell.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  AugmentationEngine
 *  @brief  Pipeline for standardized data augmentation process
 *  @author Christophe Ecabert
 *  @date   24.10.17
 *  @ingroup dataset
 */
class FK_EXPORTS AugmentationEngine {
 public:
  
#pragma mark -
#pragma mark Initialization
  
  /**
   *  @name   AugmentationEngine
   *  @fn     AugmentationEngine(void) = default
   *  @brief  Constructor
   */
  AugmentationEngine(void) = default;
  
  /**
   *  @name   AugmentationEngine
   *  @fn     AugmentationEngine(const std::string& folder,
                                 const std::vector<std::string>& exts)
   *  @brief  Constructor
   *  @param[in]  folder  Root folder where to start to look for
   *  @param[in]  exts    List of extensions of interest
   */
  AugmentationEngine(const std::string& folder,
                     const std::vector<std::string>& exts);
  
  /**
   *  @name ~AugmentationEngine
   *  @fn     ~AugmentationEngine(void) = default
   *  @brief  Destructor
   */
  ~AugmentationEngine(void);
  
  /**
   *  @name   AugmentationEngine
   *  @fn     AugmentationEngine(const AugmentationEngine& other) = delete
   *  @brief  Copy constructor
   *  @param[in] other  Object to copy from
   */
  AugmentationEngine(const AugmentationEngine& other) = delete;
  
  /**
   *  @name   AugmentationEngine
   *  @fn     AugmentationEngine(AugmentationEngine&& other) = delete
   *  @brief  Move constructor
   *  @param[in] other  Object to move from
   */
  AugmentationEngine(AugmentationEngine&& other) = delete;
  
  /**
   *  @name   operator=
   *  @fn     AugmentationEngine& operator=(const AugmentationEngine& rhs) = delete
   *  @brief  Copy assignment operator
   *  @param[in] rhs  Object to copy from
   *  @return newly assigned object
   */
  AugmentationEngine& operator=(const AugmentationEngine& rhs) = delete;
  
  /**
   *  @name   operator=
   *  @fn     AugmentationEngine& operator=(AugmentationEngine&& rhs) = delete
   *  @brief  Move assignment operator
   *  @param[in] rhs  Object to move from
   *  @return newly moved-assign object
   */
  AugmentationEngine& operator=(AugmentationEngine&& rhs) = delete;
  
  /**
   *  @name   ScanForData
   *  @fn     void ScanForData(const std::string& folder,
                               const std::vector<std::string>& exts)
   *  @brief  Search for data matching a specific extension list in a given
   *          folder
   *  @param[in]  folder  Root folder where to start to look for
   *  @param[in]  exts    List of extensions of interest
   *  @return -1 if error, 0 otherwise
   */
  int ScanForData(const std::string& folder,
                   const std::vector<std::string>& exts);
  
  /**
   *  @name   Register
   *  @fn     void Register(const AugmentationCell* cell, 
                            const bool own_it)
   *  @brief  Add new augmentation step to the engine
   *  @param[in]  cell    New augmentation step to add
   *  @param[in]  own_it  Indicates if the engine take the ownership of the cell
   */
  void Register(const AugmentationCell* cell, const bool own_it);
  
#pragma mark -
#pragma mark Registration Utility
  
  /**
   *  @name   AddIdentityCell
   *  @fn     static void AddIdentityCell(AugmentationEngine& engine)
   *  @brief  Register a new IdentityCell within a given AugmentationEngine.
   *  @param[in] engine Engine in which to add the cell
   */
  static void AddIdentityCell(AugmentationEngine& engine);
  
  /**
   *  @name   AddImgFlipCell
   *  @fn     static void AddImgFlipCell(AugmentationEngine& engine,
                                           ImgFlipCell::Direction dir)
   *  @brief  Register a new ImgFlipCell within a given AugmentationEngine.
   *  @param[in] engine Engine in which to add the cell
   *  @param[in] dir    Flip direction
   */
  static void AddImgFlipCell(AugmentationEngine& engine,
                               ImgFlipCell::Direction dir);
  
  /**
   *  @name   AddImgInPlaneRotationCell
   *  @fn     static void AddImgInPlaneRotationCell(AugmentationEngine& engine,
                                                       const double range,
                                                       const size_t n_sample)
   *  @brief  Register a new ImgInPlaneRotationCell within a given
   *          AugmentationEngine.
   *  @param[in]  engine    Engine in which to add the cell
   *  @param[in]  range     Angle range in degree
   *  @param[in]  n_sample  Number of sample to generate per image
   */
  static void AddImgInPlaneRotationCell(AugmentationEngine& engine,
                                          const double range,
                                          const size_t n_sample);
  
  /**
   *  @name   AddImgCornerCropCell
   *  @fn     static void AddImgCornerCropCell(AugmentationEngine& engine,
                                               const int width,
                                               const int height)
   *  @brief  Register a new ImgInPlaneRotationCell within a given
   *          AugmentationEngine.
   *  @param[in]  engine    Engine in which to add the cell
   *  @param[in]  width     Patch width
   *  @param[in]  height    Patch height
   */
  static void AddImgCornerCropCell(AugmentationEngine& engine,
                                   const int width,
                                   const int height);
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name   Run
   *  @fn     void Run(const std::string& output)
   *  @brief  Run data augmentation chain
   *  @param[in] output Location where to store the generated data
   */
  void Run(const std::string& output);
  
  
#pragma mark -
#pragma mark Private
  
 private:
  /** Squential step for data generation */
  std::vector<std::pair<const AugmentationCell*, bool>> sequence_;
  /** Input data */
  std::vector<std::string> input_;
};
  
}  // namespace FaceKit
#endif /* __FACEKIT_AUGMENTATION_ENGIN__ */
