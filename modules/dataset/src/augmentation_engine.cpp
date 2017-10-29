/**
 *  @file   augmentation_engine.cpp
 *  @brief Pipeline for standardized data augmentation process
 *  @ingroup dataset
 *
 *  @author Christophe Ecabert
 *  @date   24.10.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#include "facekit/dataset/augmentation_engine.hpp"
#include "facekit/core/error.hpp"
#include "facekit/core/logger.hpp"
#include "facekit/io/file_io.hpp"
#include "facekit/dataset/identity_cell.hpp"
#include "facekit/dataset/in_plane_rotation_cell.hpp"
#include "facekit/dataset/crop_cell.hpp"


/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark Initialization
  
/*
 *  @name   AugmentationEngine
 *  @fn     AugmentationEngine(const std::string& folder,
                               const std::vector<std::string>& exts)
 *  @brief  Constructor
 *  @param[in]  folder  Root folder where to start to look for
 *  @param[in]  exts    List of extensions of interest
 */
AugmentationEngine::AugmentationEngine(const std::string& folder,
                                       const std::vector<std::string>& exts) {
  if (this->ScanForData(folder, exts) != 0) {
    throw FKError(FKError::FKError::kGeneric,
                  "No data found in " + folder,
                  FUNC_NAME);
  }
}
  
/*
 *  @name ~AugmentationEngine
 *  @fn     ~AugmentationEngine(void) = default
 *  @brief  Destructor
 */
AugmentationEngine::~AugmentationEngine(void) {
  for (auto& e : sequence_) {
    if (e.second) {
      delete e.first;
      e.first = nullptr;
    }
  }
}
  
/*
 *  @name   ScanForData
 *  @fn     void ScanForData(const std::string& folder,
 const std::vector<std::string>& exts)
 *  @brief  Search for data matching a specific extension list in a given
 *          folder
 *  @param[in]  folder  Root folder where to start to look for
 *  @param[in]  exts    List of extensions of interest
 *  @return -1 if error, 0 otherwise
 */
int AugmentationEngine::ScanForData(const std::string& folder,
                                     const std::vector<std::string>& exts) {
  // Seek for every extension
  input_.clear();
  for (const auto& e : exts) {
    IO::SearchInFolder(folder, e, &input_);
  }
  return input_.empty() ? -1 : 0;
}
  
/*
 *  @name   Register
 *  @fn     void Register(const AugmentationCell* cell,
 const bool take_ownership)
 *  @brief  Add new augmentation step to the engine
 *  @param[in]  cell    New augmentation step to add
 *  @param[in]  own_it  Indicates if the engine take the ownership of the cell
 */
void AugmentationEngine::Register(const AugmentationCell* cell,
                                  const bool own_it) {
  sequence_.emplace_back(cell, own_it);
}
  
#pragma mark -
#pragma mark Registration Utility
  
/*
 *  @name   AddIdentityCell
 *  @fn     static void AddIdentityCell(AugmentationEngine& engine)
 *  @brief  Register a new IdentityCell within a given AugmentationEngine.
 *  @param[in] engine Engine in which to add the cell
 */
void AugmentationEngine::AddIdentityCell(AugmentationEngine& engine) {
  auto* cell = new IdentityCell();
  engine.Register(cell, true);
}
  
/*
 *  @name   AddImgFlipCell
 *  @fn     static void AddImgFlipCell(AugmentationEngine& engine,
 ImgFlipCell::Direction dir)
 *  @brief  Register a new AddImgFlipCell within a given AugmentationEngine.
 *  @param[in] engine Engine in which to add the cell
 *  @param[in] dir    Flip direction
 */
void AugmentationEngine::AddImgFlipCell(AugmentationEngine& engine,
                                          ImgFlipCell::Direction dir) {
  auto* cell = new ImgFlipCell(dir);
  engine.Register(cell, true);
}
  
/*
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
void AugmentationEngine::AddImgInPlaneRotationCell(AugmentationEngine& engine,
                                                     const double range,
                                                     const size_t n_sample) {
  auto* cell = new ImgInPlaneRotationCell(range, n_sample);
  engine.Register(cell, true);
}
  
/*
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
void AugmentationEngine::AddImgCornerCropCell(AugmentationEngine& engine,
                                              const int width,
                                              const int height) {
  auto* cell = new ImageCropCell(width, height);
  engine.Register(cell, true);
}
  
#pragma mark -
#pragma mark Usage
  
/*
 *  @name   Run
 *  @fn     void Run(const std::string& output)
 *  @brief  Run data augmentation chain
 *  @param[in] output Location where to store the generated data
 */
void AugmentationEngine::Run(const std::string& output) {
  std::vector<std::string> gen;
  for (size_t i = 0; i < sequence_.size(); ++i) {
    // Get generator
    const auto* cell = sequence_[i].first;
    // Log
    FACEKIT_LOG_INFO("Performing step: " << cell->name());
    // Process
    const auto input = i == 0 ? input_ : gen;
    if (cell->Process(input, output, &gen) != 0) {
      FACEKIT_LOG_ERROR("Error while generating data");
    }
  }
}
  
}  // namespace FaceKit
