/**
 *  @file   tga_image.cpp
 *  @brief TGA Image object
 *  @ingroup io
 *
 *  @author Christophe Ecabert
 *  @date   06.03.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include <iostream>

#include "facekit/io/tga_image.hpp"
#include "facekit/io/image_factory.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark TGA Header
  
/**
 *  @struct TGAColorMapSpec
 *  @brief  Color map specification
 */
struct TGAColorMapSpec {
  /** Index of first color map entry that is included in the file */
  unsigned short index_first_entry;
  /** Number of entries of the color map that are included in the file */
  unsigned short n_entry;
  /** number of bits per pixel */
  unsigned char n_bit_per_pixel;
  
  /**
   *  @name TGAColorMapSpec
   *  @fn TGAColorMapSpec(void)
   *  @brief  Contructor
   */
  TGAColorMapSpec(void) : index_first_entry(0),
  n_entry(0),
  n_bit_per_pixel(0) {
  }
  
  /**
   *  @name operator>>
   *  @fn friend std::istream& operator>>(std::istream& in,
   TGAColorMapSpec& spec)
   *  @brief  Input
   *  @param[in]  in        Input binary stream
   *  @param[in,out]  spec  Color map header to fill
   */
  friend std::istream& operator>>(std::istream& in, TGAColorMapSpec& spec) {
    if (in.good()) {
      in.read(reinterpret_cast<char*>(&spec.index_first_entry),
              sizeof(index_first_entry));
      in.read(reinterpret_cast<char*>(&spec.n_entry),
              sizeof(n_entry));
      in.read(reinterpret_cast<char*>(&spec.n_bit_per_pixel),
              sizeof(n_bit_per_pixel));
    }
    return in;
  }
  
  /**
   *  @name operator<<
   *  @fn friend std::ostream& operator<<(std::ostream& out,
   TGAColorMapSpec& spec)
   *  @brief  Write
   *  @param[in]  out       Output binary stream
   *  @param[in,out]  spec  Color map header to fill
   */
  friend std::ostream& operator<<(std::ostream& out,
                                  const TGAColorMapSpec& spec) {
    if (out.good()) {
      out.write(reinterpret_cast<const char*>(&spec.index_first_entry),
                sizeof(index_first_entry));
      out.write(reinterpret_cast<const char*>(&spec.n_entry),
                sizeof(n_entry));
      out.write(reinterpret_cast<const char*>(&spec.n_bit_per_pixel),
                sizeof(n_bit_per_pixel));
    }
    return out;
  }
};

/**
 *  @struct TGAImageSpec
 *  @brief  Image specification
 */
struct TGAImageSpec {
  /** X origin */
  unsigned short x_origin;
  /** Y origin */
  unsigned short y_origin;
  /** Image width */
  unsigned short width;
  /** Image height */
  unsigned short height;
  /** Pixel depth */
  unsigned char pixel_depth;
  /** Image descriptor - bits 3-0 give the alpha channel depth,
   bits 5-4 give direction */
  unsigned char image_descriptor;
  
  /**
   *  @name operator>>
   *  @fn friend std::istream& operator>>(std::istream& in,
   TGAImageSpec& spec)
   *  @brief  Input
   *  @param[in]  in    Input binary stream
   *  @param[out] spec  Image spec to fill
   */
  friend std::istream& operator>>(std::istream& in, TGAImageSpec& spec) {
    if (in.good()) {
      in.read(reinterpret_cast<char*>(&spec.x_origin), sizeof(x_origin));
      in.read(reinterpret_cast<char*>(&spec.y_origin), sizeof(y_origin));
      in.read(reinterpret_cast<char*>(&spec.width), sizeof(width));
      in.read(reinterpret_cast<char*>(&spec.height), sizeof(height));
      in.read(reinterpret_cast<char*>(&spec.pixel_depth),
              sizeof(pixel_depth));
      in.read(reinterpret_cast<char*>(&spec.image_descriptor),
              sizeof(image_descriptor));
    }
    return in;
  }
  
  /**
   *  @name operator<<
   *  @fn friend std::ostream& operator<<(std::ostream& out,
   TGAImageSpec& spec)
   *  @brief  Write
   *  @param[in]  out  Input binary stream
   *  @param[in]  spec Image spec to write
   */
  friend std::ostream& operator<<(std::ostream& out,
                                  const TGAImageSpec& spec) {
    if (out.good()) {
      out.write(reinterpret_cast<const char*>(&spec.x_origin),
                sizeof(x_origin));
      out.write(reinterpret_cast<const char*>(&spec.y_origin),
                sizeof(y_origin));
      out.write(reinterpret_cast<const char*>(&spec.width),
                sizeof(width));
      out.write(reinterpret_cast<const char*>(&spec.height),
                sizeof(height));
      out.write(reinterpret_cast<const char*>(&spec.pixel_depth),
                sizeof(pixel_depth));
      out.write(reinterpret_cast<const char*>(&spec.image_descriptor),
                sizeof(image_descriptor));
    }
    return out;
  }
};

/**
 *  @struct TGAHeader
 *  @brief  TGA file header
 */
struct TGAHeader {
  /** ID length	Length of the image ID field */
  unsigned char id_length;
  /** Color map type - 0/1 */
  unsigned char color_map_type;
  /** Image type - compression and color type */
  unsigned char image_type;
  /** Color map specification */
  TGAColorMapSpec color_map_spec;
  /** Image specification */
  TGAImageSpec image_spec;
  
  /**
   *  @name operator>>
   *  @fn friend std::istream& operator>>(std::istream& in,
   TGAHeader& header)
   *  @brief  Input
   *  @param[in]  in  Input binary stream
   *  @param[in,out]  header Image spec to fill
   */
  friend std::istream& operator>>(std::istream& in, TGAHeader& header) {
    if (in.good()) {
      // Read data
      in.read(reinterpret_cast<char*>(&header.id_length), sizeof(id_length));
      in.read(reinterpret_cast<char*>(&header.color_map_type),
              sizeof(color_map_type));
      in.read(reinterpret_cast<char*>(&header.image_type),
              sizeof(image_type));
      // Color map spec + image spec
      in >> header.color_map_spec >> header.image_spec;
    }
    return in;
  }
  
  /**
   *  @name operator<<
   *  @fn friend std::ostream& operator<<(std::ostream& out,
   TGAHeader& header)
   *  @brief  Write
   *  @param[in]  out         Output binary stream
   *  @param[in,out] header   Image spec to fill
   */
  friend std::ostream& operator<<(std::ostream& out, const TGAHeader& header){
    if (out.good()) {
      // Read data
      out.write(reinterpret_cast<const char*>(&header.id_length),
                sizeof(id_length));
      out.write(reinterpret_cast<const char*>(&header.color_map_type),
                sizeof(color_map_type));
      out.write(reinterpret_cast<const char*>(&header.image_type),
                sizeof(image_type));
      // Color map spec + image spec
      out << header.color_map_spec;
      out << header.image_spec;
    }
    return out;
  }
};
  
#pragma mark -
#pragma mark Initialization
  
/*
 *  @name TGAImage
 *  @fn TGAImage(void)
 *  @brief  Constructor
 */
TGAImage::TGAImage(void) : header_(new TGAHeader()) {
}
  
/*
 *  @name ~TGAImage
 *  @fn ~TGAImage(void) override
 *  @brief  Destructor
 */
TGAImage::~TGAImage(void) {
  if (header_) {
    delete header_;
  }
}
  
/*
 *  @name Load
 *  @fn Status Load(std::istream& stream) override
 *  @brief  Load image from dist
 *  @param[in]  stream  Binary stream from where to load the ressource
 *  @return Operation status
 */
Status TGAImage::Load(std::istream& stream) {
  Status status;
  if (stream.good()) {
    int err = -1;
    // Read header
    stream >> *header_;
    if (header_->image_type == 2 || header_->image_type == 3) {
      // Handle only color or grayscale image for the moment
      // Define image prop
      this->width_ = static_cast<size_t>(header_->image_spec.width);
      this->height_ = static_cast<size_t>(header_->image_spec.height);
      this->format_ = static_cast<Format>(header_->image_spec.pixel_depth / 8);
      // Allocate buffer
      this->buffer_.Resize(DataType::kUInt8,
                           {this->height_, this->width_, this->format_});
      // Read data
      int bpp = (header_->image_spec.pixel_depth + 7) / 8;
      const size_t sz = (this->width_  * this->height_ * bpp);
      auto* ptr = this->data();
      stream.read(reinterpret_cast<char*>(ptr), sz);
      if (this->format_ != Format::kGrayscale) {
        // Convert image pixel format (BGR -> RGB or BGRA -> RGBA)
        auto* src = ptr;
        auto* dst = &src[2];
        uint8_t tmp;
        const size_t n_elem = this->width_ * this->height_;
        for (size_t n = 0; n < n_elem; ++n) {
          tmp = *src;
          *src = *dst;
          *dst = tmp;
          src += bpp;
          dst += bpp;
        }
      }
      // Sanity check
      err = stream.good() ? 0 : -1;
    }
    if (err != 0) {
      status = Status(Status::Type::kInternalError, "Error while reading TGA");
    }
  } else {
    status = Status(Status::Type::kInvalidArgument, "Stream has errors");
  }
  return status;
}
  
/*
 *  @name Save
 *  @fn Status Save(std::ostream& stream) const override
 *  @brief  Load image to dist
 *  @param[in]  stream  Binary stream to where to save the ressource
 *  @return Operation status
 */
Status TGAImage::Save(std::ostream& stream) const {
  Status status;
  if (stream.good() && this->data() != nullptr) {
    // Write header
    stream << header_;
    // Convert image pixel format (RGB -> BGR or RGBA -> BGRA)
    // Do copy since the method is marked as 'const'
    size_t bpp = static_cast<size_t>(this->format_);
    size_t n_pixel = this->width_ * this->height_;
    auto* ptr = this->data();
    std::vector<uint8_t> buffer(ptr, ptr + (n_pixel * bpp));
    if (this->format_ != Format::kGrayscale) {
      auto* src = buffer.data();
      auto* dst = &src[2];
      uint8_t tmp = 0;
      for (size_t n = 0; n < n_pixel; ++n) {
        tmp = *src;
        *src = *dst;
        *dst = tmp;
        src += bpp;
        dst += bpp;
      }
    }
    // Write
    stream.write(reinterpret_cast<const char*>(buffer.data()),
                 this->width_ * this->height_ * bpp);
    if (!stream.good()) {
      status = Status(Status::Type::kInternalError, "Error while writing TGA");
    }
  } else {
    status = Status(Status::Type::kInvalidArgument, "Stream has errors");
  }
  return status;
}
  
#pragma mark -
#pragma mark Registration
  
/** Add tga implementation to the image factory */
REGISTER_IMAGE_IMPL(TGAImage, "tga");
  
}  // namespace FaceKit
