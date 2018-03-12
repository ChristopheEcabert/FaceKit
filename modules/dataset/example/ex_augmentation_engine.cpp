/**
 *  @file   ex_augmentation_engine.cpp
 *  @brief Example on how to run data augmentation pipeline
 *  @ingroup dataset
 *
 *  @author Christophe Ecabert
 *  @date   24.10.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#include <iostream>

#include "facekit/core/cmd_parser.hpp"
#include "facekit/core/logger.hpp"
#include "facekit/io/file_io.hpp"
#include "facekit/dataset/augmentation_engine.hpp"

#include "facekit/dataset/identity_cell.hpp"
#include "facekit/dataset/flip_cell.hpp"


int main(const int argc, const char** argv) {
  namespace FK = FaceKit;
  // Parser
  FK::CmdLineParser parser;
  parser.AddArgument("-i",
                     FaceKit::CmdLineParser::ArgState::kNeeded,
                     "Location where data are stored");
  parser.AddArgument("-o",
                     FaceKit::CmdLineParser::ArgState::kNeeded,
                     "Location where to output data");
  int err = parser.ParseCmdLine(argc, argv);
  if (err == 0) {
    // Retrieve args
    std::string root_folder, output;
    parser.HasArgument("-i", &root_folder);
    parser.HasArgument("-o", &output);
    
    // Create augmentation engine
    FaceKit::AugmentationEngine engine;
    // Scan for data
    err = engine.ScanForData(root_folder, {"png"});
    // Add different type of augmentation
    FK::AugmentationEngine::AddIdentityCell(engine);
    FK::AugmentationEngine::AddImgFlipCell(engine,
                                           FK::ImgFlipCell::Direction::kBoth);
    FK::AugmentationEngine::AddImgInPlaneRotationCell(engine, 5.0, 5);
    FK::AugmentationEngine::AddImgCornerCropCell(engine, 300, 300);
    if (err == 0) {
      // Do augmentation
      engine.Run(output);
      FACEKIT_LOG_INFO("Done");
    } else {
      FACEKIT_LOG_ERROR("No images founded in " + root_folder);
    }
  } else {
    FACEKIT_LOG_ERROR("Unable to parse command line!");
  }
  return err;
}
