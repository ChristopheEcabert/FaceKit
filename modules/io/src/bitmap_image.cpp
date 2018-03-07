/**
 *  @file   bitmap_image.cpp
 *  @brief Bitmap Object Image
 *  @ingroup io
 *
 *  @author Christophe Ecabert
 *  @date   07.03.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include <iostream>
#include <algorithm>
#include <cassert>

#include "facekit/io/bitmap_image.hpp"
#include "facekit/io/image_factory.hpp"
#include "facekit/core/logger.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark Type Definition
  
/**
 *  @class BMPHeader
 *  @brief  Store data present into BMP header
 *  @author Christophe Ecabert
 *  @date   07.03.18
 *  @ingroup io
 *  @see https://en.wikipedia.org/wiki/BMP_file_format
 */
class DIBHeader {
 public:
  
  enum CompType {
    /** Standard */
    kRGB = 0,
    kRLE8 = 1,
    kRLE4 = 2
  };
  
  /** DIB size */
  int32_t size;
  /** Image Width */
  int32_t width;
  /** Image Height */
  int32_t height;
  /** Number of plane */
  int32_t plane;
  /** Bits per pixel (1, 4, 8, 16, 24, 32) */
  int32_t bpp;
  /** Compression type */
  CompType comp;
  /** Raw image size */
  int32_t raw_img_size;
  /** Horizontal resolution */
  int32_t h_res;
  /** Vertical resolution */
  int32_t v_res;
  /** Number of color in color table */
  int32_t n_color_palette;
  /** Number of important color */
  int32_t n_important_color;
  /** Loading was successfult */
  bool success;
  
  /** Read from stream */
  void Load(std::istream& is);
  /** Write to stream */
  void Save(std::ostream& os) const;
  /** Re-init value */
  void Clear(void);
  
 private:
  // 12bytes header
  void LoadHdr12(std::istream& is);
  // 40bytes header
  void LoadHdr40(std::istream& is);
};
  
/** Load */
void DIBHeader::Load(std::istream &is) {
  // Read DIB header size -> then load based on that since there not all the same
  is.read(reinterpret_cast<char*>(&size), sizeof(size));
  switch (size) {
    // 12 bytes
    case 12: this->LoadHdr12(is);
      break;
      
    // 40 bytes
    case 40: this->LoadHdr40(is);
      break;
      
    default: this->success = false;
             FACEKIT_LOG_ERROR("Unsupported bitmap format");
      break;
  }
}
  
// 12bytes
void DIBHeader::LoadHdr12(std::istream &is) {
  int16_t dummy;
  // width
  is.read(reinterpret_cast<char*>(&dummy), sizeof(dummy));
  this->width = dummy;
  // height
  is.read(reinterpret_cast<char*>(&dummy), sizeof(dummy));
  this->height = dummy;
  // #plane
  is.read(reinterpret_cast<char*>(&dummy), sizeof(dummy));
  this->plane = dummy;
  // Bits per pixels
  is.read(reinterpret_cast<char*>(&dummy), sizeof(dummy));
  this->bpp = dummy;
  // Indicate loading went file, support only color at the moment
  this->success = (this->bpp == 24) || (this->bpp == 32);
}
  
// 40bytes header
void DIBHeader::LoadHdr40(std::istream& is) {
  int32_t dummy4;
  int16_t dummy2;
  // width
  is.read(reinterpret_cast<char*>(&dummy4), sizeof(dummy4));
  this->width = dummy4;
  // height
  is.read(reinterpret_cast<char*>(&dummy4), sizeof(dummy4));
  this->height = dummy4;
  // #plane
  is.read(reinterpret_cast<char*>(&dummy2), sizeof(dummy2));
  this->plane = dummy2;
  // Bits per pixels
  is.read(reinterpret_cast<char*>(&dummy2), sizeof(dummy2));
  this->bpp = dummy2;
  // Compression
  is.read(reinterpret_cast<char*>(&dummy4), sizeof(dummy4));
  this->comp = static_cast<CompType>(dummy4);
  // Raw image size
  is.read(reinterpret_cast<char*>(&dummy4), sizeof(dummy4));
  this->raw_img_size = dummy4;
  // Horizontal res
  is.read(reinterpret_cast<char*>(&dummy4), sizeof(dummy4));
  this->h_res = dummy4;
  // Vertiacl res
  is.read(reinterpret_cast<char*>(&dummy4), sizeof(dummy4));
  this->v_res = dummy4;
  // #Color palette
  is.read(reinterpret_cast<char*>(&dummy4), sizeof(dummy4));
  this->n_color_palette = dummy4 == 0 ? (0x01 << bpp) : dummy4 ;
  // Number of important color
  is.read(reinterpret_cast<char*>(&dummy4), sizeof(dummy4));
  this->n_important_color = dummy4;
  // Indicate loading went file
  this->success = ((this->bpp == 24) || (this->bpp == 32)) && this->comp == kRGB;
}
  
/** Write to stream - Save size 40 only */
void DIBHeader::Save(std::ostream& os) const {
  // Read DIB header size -> then load based on that since there not all the same
  os.write(reinterpret_cast<const char*>(&size), sizeof(size));
  // width
  os.write(reinterpret_cast<const char*>(&this->width), sizeof(this->width));
  // height
  os.write(reinterpret_cast<const char*>(&this->height), sizeof(this->height));
  // #plane
  int16_t dummy2 = this->plane;
  os.write(reinterpret_cast<char*>(&dummy2), sizeof(dummy2));
  // Bits per pixels
  dummy2 = this->bpp;
  os.write(reinterpret_cast<char*>(&dummy2), sizeof(dummy2));
  // Compression
  int32_t dummy4 = static_cast<int32_t>(this->comp);
  os.write(reinterpret_cast<const char*>(&dummy4), sizeof(dummy4));
  // Raw image size
  os.write(reinterpret_cast<const char*>(&this->raw_img_size),
           sizeof(this->raw_img_size));
  // Horizontal res
  os.write(reinterpret_cast<const char*>(&this->h_res), sizeof(this->h_res));
  // Vertiacl res
  os.write(reinterpret_cast<const char*>(&this->v_res), sizeof(this->v_res));
  // #Color palette
  os.write(reinterpret_cast<const char*>(&this->n_color_palette),
           sizeof(this->n_color_palette));
  // Number of important color
  os.write(reinterpret_cast<const char*>(&this->n_important_color),
           sizeof(this->n_important_color));
}
  
/** Re-init value */
void DIBHeader::Clear(void) {
  /** DIB size */
  size = 0;
  /** Image Width */
  width = 0;
  /** Image Height */
  height = 0;
  /** Number of plane */
  plane = 0;
  /** Bits per pixel (1, 4, 8, 16, 24, 32) */
  bpp = 0;
  /** Compression type */
  comp = CompType::kRGB;
  /** Raw image size */
  raw_img_size = 0;
  /** Horizontal resolution */
  h_res = 0;
  /** Vertical resolution */
  v_res = 0;
  /** Number of color in color table */
  n_color_palette = 0;
  /** Number of important color */
  n_important_color = 0;
  /** Loading was successfult */
  success = false;
}
  

/**
 *  @class  BMPHeader
 *  @brief  Store data present into BMP header
 *  @author Christophe Ecabert
 *  @date   07.03.18
 *  @ingroup io
 */
class BMPHeader {
 public:
  /** Header field, usually equal to 'BM' */
  int8_t hdr_field[2];
  /** File size in byte */
  int32_t fsize;
  /** Offset to pixel array */
  int32_t offset;
  /** DIB Header */
  DIBHeader dib;
  /** Color table - BGRx */
  std::vector<uint32_t> table;
  
  /** Read */
  Status Load(std::istream& is);
  /** Write */
  Status Save(std::ostream& os) const;
  /** Clear */
  void Clear(void);
};
  
Status BMPHeader::Load(std::istream& is) {
  Status status;
  if (is.good()) {
    // hdr field
    is.read(reinterpret_cast<char*>(&this->hdr_field[0]),
            sizeof(this->hdr_field));
    // File size
    is.read(reinterpret_cast<char*>(&this->fsize), sizeof(this->fsize));
    // Reserved (skip)
    int32_t dummy;
    is.read(reinterpret_cast<char*>(&dummy), sizeof(dummy));
    // offset
    is.read(reinterpret_cast<char*>(&this->offset), sizeof(this->offset));
    // Read DIB
    this->dib.Load(is);
    // So far so good ?
    if (this->dib.success) {
      // Check color table
      if (this->dib.bpp <= 8) {
        this->table.resize(this->dib.n_color_palette);
        uint32_t color;
        for (int32_t k = 0; k < this->dib.n_color_palette; ++k) {
          // Read entry B G R A -> Not clear what format is used
          is.read(reinterpret_cast<char*>(&color), sizeof(color));
          this->table[k] = (color & 0xFFFFFFFF);
        }
      }
    } else {
      status = Status(Status::Type::kInternalError,
                      "Error while decoding BMP header");
    }
  } else {
    status = Status(Status::Type::kInvalidArgument, "Stream has errors");
  }
  return status;
}
  
/** Write */
Status BMPHeader::Save(std::ostream& os) const {
  Status status;
  if (os.good()) {
    os.write(reinterpret_cast<const char*>(&hdr_field[0]), sizeof(hdr_field));
    // File size
    os.write(reinterpret_cast<const char*>(&this->fsize), sizeof(this->fsize));
    // Reserved (skip)
    int32_t dummy = 0;
    os.write(reinterpret_cast<const char*>(&dummy), sizeof(dummy));
    // offset
    os.write(reinterpret_cast<const char*>(&this->offset), sizeof(this->offset));
    // Write DIB
    this->dib.Save(os);
    // Write color table
    os.write(reinterpret_cast<const char*>(table.data()),
             table.size() * sizeof(uint32_t));
  } else {
    status = Status(Status::Type::kInvalidArgument, "Stream has errors");
  }
  return status;
}
  
/** Clear */
void BMPHeader::Clear(void) {
  /** Header field, usually equal to 'BM' */
  hdr_field[0] = 0;
  hdr_field[1] = 0;
  /** File size in byte */
  fsize = 0;
  /** Offset to pixel array */
  offset = 0;
  /** DIB Header */
  dib.Clear();
  /** Color table - BGRx */
  table.clear();
}
  
/** 3bytes pixel */
struct Pixel3 {
  uint8_t b;
  uint8_t g;
  uint8_t r;
  
  Pixel3(void) = default;
  Pixel3(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
};
/** 4bytes pixel */
struct Pixel4 {
  uint8_t b;
  uint8_t g;
  uint8_t r;
  uint8_t a;
  
  Pixel4(void) = default;
  Pixel4(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
};
  
/** Swap B<->R channel */
struct SwapChannel {
  Pixel3 operator()(Pixel3 p) const {
    return Pixel3(p.b, p.g, p.r);
  }
  Pixel4 operator()(Pixel4 p) const {
    return Pixel4(p.b, p.g, p.r, p.a);
  }
};
  
/** Convert to grayscale */
struct ConvertToGray {
#define CV_DESCALE(x, n) (((x) + (1 << ((n) -1))) >> (n))
  uint8_t operator()(Pixel3 p) const {
    uint8_t v = (1868 * p.b) + (9617 * p.g) + (4899 * p.r);
    return CV_DESCALE(v, 14);
  }
  uint8_t operator()(Pixel4 p) const {
    uint8_t v = (1868 * p.b) + (9617 * p.g) + (4899 * p.r);
    return CV_DESCALE(v, 14);
  }
#undef CV_DESCALE
};
  
  struct ConvertRGB565 {
    
  };
  
#pragma mark -
#pragma mark Initialization
  
/*
 *  @name BMPImage
 *  @fn BMPImage(void)
 *  @brief  Constructor
 */
BMPImage::BMPImage(void) : header_(new BMPHeader()) {}
  
/*
 *  @name ~BMPImage
 *  @fn ~BMPImage(void)
 *  @brief  Destructor
 */
BMPImage::~BMPImage(void) {
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
Status BMPImage::Load(std::istream& stream) {
  Status status;
  if (stream.good()) {
    // Save stream pos, in order to move to pixel array later on
    auto p = stream.tellg();
    // Read header
    header_->Clear();
    status = header_->Load(stream);
    if (status.Good()) {
      // Define image prop
      auto bpp = header_->dib.bpp;
      this->width_ = header_->dib.width;
      this->height_ = std::abs(header_->dib.height);
      this->format_ = bpp == 32 ? Format::kRGBA : Format::kRGB;
      // Allocate buffer
      buffer_.Resize(DataType::kUInt8,
                     {this->height_, this->width_, this->format_});
      // Move to begining of pixel arry
      auto pa = p + static_cast<std::streamoff>(header_->offset);
      stream.seekg(pa);
      
      // Define some prop
      int step = (int)(buffer_.dim_size(1) * buffer_.dim_size(2));
      const size_t src_pitch = (((this->width_ * bpp) + 31) / 32) * 4;
      auto* ptr = buffer_.AsFlat<uint8_t>().data();
      if (this->header_->dib.height > 0) {
        // Origin at bottom left
        ptr += (this->height_ - 1) * step;
        step = -step;
      }
      // Start decoding
      std::vector<uint8_t> buff(src_pitch);
      switch (bpp) {
        // 24 bit per pixel
        case 24: {
          auto* bptr = reinterpret_cast<Pixel3*>(buff.data());
          auto bstep = this->width_;
          for (size_t k = 0; k < this->height_; ++k, ptr += step) {
            // Read data to buffer
            stream.read(reinterpret_cast<char*>(buff.data()), src_pitch);
            // Copy to buffer
            std::transform(bptr,
                           bptr + bstep,
                           reinterpret_cast<Pixel3*>(ptr),
                           SwapChannel());
          }
        }
          break;
          
        // 32bit per pixel
        case 32: {
          auto* bptr = reinterpret_cast<Pixel3*>(buff.data());
          auto bstep = this->width_;
          for (size_t k = 0; k < this->height_; ++k, ptr += step) {
            // Read data to buffer
            stream.read(reinterpret_cast<char*>(buff.data()), src_pitch);
            // Copy to buffer
            std::transform(bptr,
                           bptr + bstep,
                           reinterpret_cast<Pixel3*>(ptr),
                           SwapChannel());
          }
        }
          break;
          
        default: status = Status(Status::Type::kInternalError,
                                 "Invalid pixel format");
      } 
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
Status BMPImage::Save(std::ostream& stream) const {
  Status status;
  if (stream.good() && this->data() != nullptr) {
    // Proper header
    header_->Clear();
    // Define properties
    int32_t w = static_cast<int32_t>(this->width_);
    int32_t h = static_cast<int32_t>(this->height_);
    int32_t chan = static_cast<int32_t>(this->format_);
    int32_t fileStep = (w * chan + 3) & -4;
    uint8_t zeropad[] = "\0\0\0\0";
    int32_t bmpHdrSize =  40;
    int32_t palette_sz = (this->format_ == Format::kGrayscale ?
                          0x01 << this->format_ :
                          0);
    int32_t hdr_sz = 14 + bmpHdrSize + palette_sz;
    int32_t fileSize = h * fileStep + hdr_sz;
    
    // Fill BMP Header
    header_->hdr_field[0] = 'B';
    header_->hdr_field[1] = 'M';
    header_->fsize = fileSize;
    header_->offset = hdr_sz;
    // Fill DIB Header
    DIBHeader dib;
    dib.size = 40;
    dib.width = w;
    dib.height = h;
    dib.plane = 1;
    dib.bpp = this->format_ << 0x03;
    dib.comp = DIBHeader::CompType::kRGB;
    dib.raw_img_size = (w * h * dib.bpp) / 8;
    dib.h_res = 0;
    dib.v_res = 0;
    dib.n_color_palette = 0;    // Default
    dib.n_important_color = 0;  // all are important
    //TODO: (Christophe) Handle color table HERE
    header_->table.clear();
  
    
    
    
    // Add DIB to header
    header_->dib = dib;
    // Dump to stream
    header_->Save(stream);
    // Copy data
    auto step = this->width_ * this->format_;
    auto* ptr = this->data();
    ptr += (this->height_ - 1) * step;
    for (int k = (int)this->height_ - 1; k >= 0; --k, ptr -= step) {
      std::vector<uint8_t> buff(ptr, ptr + step);
      if (this->format_ == Format::kGrayscale) {
        void(0);
        // TODO: (Christophe) Handle grayscale + color table here
        
        
        
      } else {
        auto* src = buff.data();
        auto* dst = &src[2];
        uint8_t tmp;
        for (size_t n = 0; n < this->width_; ++n) {
          tmp = *src;
          *src = *dst;
          *dst = tmp;
          src += this->format_;
          dst += this->format_;
        }
      }
      // Write
      stream.write(reinterpret_cast<const char*>(buff.data()), step);
      // Add padding ?
      if (step < fileSize) {
        stream.write(reinterpret_cast<const char*>(zeropad), fileStep - step);
      }
    }
    if (!stream.good()) {
      status = Status(Status::Type::kInternalError, "Error while writing BMP");
    }
  } else {
    status = Status(Status::Type::kInvalidArgument, "Stream has errors");
  }
  return status;
}
  
  
#pragma mark -
#pragma mark Registration
  
/** Add bmp implementation to the image factory */
REGISTER_IMAGE_IMPL(BMPImage, "bmp");
  
}  // namespace FaceKit
