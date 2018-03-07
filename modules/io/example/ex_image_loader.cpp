/**
 *  @file   image_loader.cpp
 *  @brief  Example to show how to use image class
 *
 *  @author Christophe Ecabert
 *  @date   07.03.18
 *    Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#include <iostream>

#include "facekit/core/cmd_parser.hpp"
#include "facekit/core/utils/string.hpp"
#include "facekit/io/image_factory.hpp"

int main(const int argc, const char** argv) {
  namespace FK = FaceKit;
  // Define argument needed
  FK::CmdLineParser parser;
  parser.AddArgument("-i",
                     FK::CmdLineParser::ArgState::kNeeded,
                     "Input image");
  // Parse
  int err = parser.ParseCmdLine(argc, argv);
  if (!err) {
    err = -1;
    std::string imagepath;
    parser.HasArgument("-i", &imagepath);

    // Load image based on type
    std::string dir, file, ext;
    FK::Path::SplitComponent(imagepath, &dir, &file, &ext);
    auto* image = FK::ImageFactory::Get().CreateByExtension(ext);
    // Save input
    if (image && image->Load(imagepath).Good()) {
      std::string output = (dir.empty() ?
                            file + "_save." + ext :
                            dir + "/" + file + "_save." + ext);
      err = image->Save(output).Good() ? 0 : -1;
    }
    std::cout << "Done : " << (!err ? "Success" : "Fail") << std::endl;
    if (image) {
      delete image;
    }
  } else {
    std::cout << "Unable to parse cmd line" << std::endl;
  }
  return err;
}
