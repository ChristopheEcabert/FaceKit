/**
 *  @file   aabb.hpp
 *  @brief  Axis Aligned Bounding Box
 *  @ingroup geometry
 *
 *  @author Christophe Ecabert
 *  @date   02/08/16
 *  Copyright © 2016 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_AABB__
#define __FACEKIT_AABB__

#include <iostream>

#include "facekit/core/library_export.hpp"
#include "facekit/core/math/vector.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class   AABB
 *  @brief   Axis Aligned Bouding Box
 *  @author  Christophe Ecabert
 *  @date    02/08/16
 *  @ingroup geometry
 *  @tparam T Data type
 */
template<typename T>
class FK_EXPORTS AABB {
 public:
#pragma mark -
#pragma mark Type definition

  /**
   * @enum  AxisType
   * @brief Type of Axis
   */
  enum AxisType {
    /** X Axis */
    kX,
    /** Y Axis */
    kY,
    /** Z Axis */
    kZ
  };

#pragma mark -
#pragma mark Initialization

  /**
   * @name  AABB
   * @fn  AABB(void)
   * @brief Constructor
   */
  AABB(void) : index_(-1) {}

  /**
   * @name  AABB
   * @fn  AABB(const AABB<T>& other)
   * @brief Constructor
   * @param[in] other Other object to copy from
   */
  AABB(const AABB<T>& other) {
    this->min_ = other.min_;
    this->max_ = other.max_;
    this->index_ = other.index_;
    this->center_ = other.center_;
  }

  /**
   * @name  operator=
   * @fn  AABB& operator=(const AABB<T>& rhs)
   * @brief Assignment operator
   * @param[in] rhs AABB to assign from
   * @return  New assigned object
   */
  AABB& operator=(const AABB<T>& rhs) {
    if (this != &rhs) {
      this->min_ = rhs.min_;
      this->max_ = rhs.max_;
      this->index_ = rhs.index_;
      this->center_ = rhs.center_;
    }
    return *this;
  }

  /**
   * @name  AABB
   * @fn  AABB(const T xmin,const T xmax,const T ymin,const T ymax,
               const T zmin,const T zmax, const int index)
   * @brief Constructor
   * @param[in] xmin  Minimum X value
   * @param[in] xmax  Maximum X value
   * @param[in] ymin  Minimum Y value
   * @param[in] ymax  Maximum Y value
   * @param[in] zmin  Minimum Z value
   * @param[in] zmax  Maximum Z value
   */
  AABB(const T xmin, const T xmax,
       const T ymin, const T ymax,
       const T zmin, const T zmax) : min_(xmin, ymin, zmin),
                                     max_(xmax, ymax, zmax),
                                     index_(-1) {
    center_ = (min_ + max_) * T(0.5);
  }

  /**
   * @name  AABB
   * @fn  AABB(const T xmin,const T xmax,const T ymin,const T ymax,
               const T zmin,const T zmax, const int index)
   * @brief Constructor
   * @param[in] xmin  Minimum X value
   * @param[in] xmax  Maximum X value
   * @param[in] ymin  Minimum Y value
   * @param[in] ymax  Maximum Y value
   * @param[in] zmin  Minimum Z value
   * @param[in] zmax  Maximum Z value
   * @param[in] index Index of the bounding box
   */
  AABB(const T xmin, const T xmax,
       const T ymin, const T ymax,
       const T zmin, const T zmax,
       const int index) : min_(xmin, ymin, zmin),
                          max_(xmax, ymax, zmax),
                          index_(index) {
    center_ = (min_ + max_) * T(0.5);
  }

  /**
   * @name  ~AABB
   * @fn  ~AABB(void)
   * @brief Destructor
   */
  ~AABB(void) {}

#pragma mark -
#pragma mark Operator

  /**
   * @name  operator==
   * @fn  bool operator==(const AABB<T>& rhs)
   * @brief Egal operator
   * @param[in] rhs AABB to compare from
   * @return  True if AABBs are egal
   */
  bool operator==(const AABB<T>& rhs) {
    return ((this->min_ == rhs.min_) && (this->max_ == rhs.max_));
  }

  /**
   * @name  operator!=
   * @fn  bool operator!=(const AABB<T>& rhs)
   * @brief Egal operator
   * @param[in] rhs AABB to compare from
   * @return  True if AABBs are egal
   */
  bool operator!=(const AABB<T>& rhs) {
    return ((this->min_ != rhs.min_) || (this->max_ != rhs.max_));
  }

  /**
   * @name  operator+
   * @fn  AABB<T> operator+(const AABB<T>& rhs)
   * @brief Union operator
   * @param[in] rhs Right hand sign
   * @return  Union of this with rhs
   */
  AABB<T> operator+(const AABB<T>& rhs) {
    return AABB(std::min(min_.x_, rhs.min_.x_),
                std::max(max_.x_, rhs.max_.x_),
                std::min(min_.y_, rhs.min_.y_),
                std::max(max_.y_, rhs.max_.y_),
                std::min(min_.z_, rhs.min_.z_),
                std::max(max_.z_, rhs.max_.z_), -1);
  }

  /**
   * @name  operator+
   * @fn  AABB<T>& operator+(const AABB<T>& rhs)
   * @brief Union operator
   * @param[in] rhs Right hand sign
   * @return  Union of this
   */
  AABB<T>& operator+=(const AABB<T>& rhs) {
    // Update bbox
    this->min_.x_ = std::min(min_.x_, rhs.min_.x_);
    this->max_.x_ = std::max(max_.x_, rhs.max_.x_);
    this->min_.y_ = std::min(min_.y_, rhs.min_.y_);
    this->max_.y_ = std::max(max_.y_, rhs.max_.y_);
    this->min_.z_ = std::min(min_.z_, rhs.min_.z_);
    this->max_.z_ = std::max(max_.z_, rhs.max_.z_);
    // Update center
    this->center_ = (min_ + max_) * T(0.5);
    return *this;
  }

  /**
   * @name  operator<<
   * @fn  friend std::ofstream& operator<<(std::ofstream& os,
   *                                        const AABB<T>& bbox)
   * @brief Output operator
   * @param[in] os    Output stream
   * @param[in] bbox  Bounding box to print
   * @return  Output stream
   */
  friend std::ostream& operator<<(std::ostream& os, const AABB<T>& bbox) {
    const T dx = bbox.max_.x_ - bbox.min_.x_;
    const T dy = bbox.max_.y_ - bbox.min_.y_;
    const T dz = bbox.max_.z_ - bbox.min_.z_;
    os << "center: " << bbox.center_ << std::endl;
    os << "dx : " << dx << std::endl;
    os << "dy : " << dy << std::endl;
    os << "dz : " << dz;
    return os;
  }

#pragma mark -
#pragma mark Usage

  /**
   * @name  Overlap
   * @fn  static bool Overlap(const AABB<T>& bb1, const AABB<T>& bb2)
   * @brief Check if the two given bounding boxes overlap
   * @param[in] bb1 First Bounding Box
   * @param[in] bb2 Second Bounding Box
   * @return  True if boxes overlap, false otherwise
   */
  static bool Overlap(const AABB<T>& bb1, const AABB<T>& bb2) {
    if (bb1.max_.x_ < bb2.min_.x_ || bb2.max_.x_ < bb1.min_.x_)
      return false;
    if (bb1.max_.y_ < bb2.min_.y_ || bb2.max_.y_ < bb1.min_.y_)
      return false;
    if (bb1.max_.z_ < bb2.min_.z_ || bb2.max_.z_ < bb1.min_.z_)
      return false;
    return true;
  }

  /**
   * @name  ObjectBoundingBox
   * @fn  static AABB ObjectBoundingBox(AABBIterator first,
   AABBIterator last)
   * @brief Compute object bounding box
   * @param[in] first First object Bounding Box
   * @param[in] last  Last object Bounding Box
   * @return  Bounding box for the range [first, last[.
   */
  template<class Iterator>
  static AABB<T> ObjectBoundingBox(Iterator first,
                                Iterator last) {
    AABB<T> bbox = *first;
    for (++first ; first != last ; ++first) {
      bbox += *first;
    }
    return bbox;
  }

  /**
   * @name  LongestAxis
   * @fn  static AxisType LongestAxis(const AABB<T>& aabb)
   * @brief Detect which axis is the longest for a given AABB.
   * @param[in] aabb    Bounding box to determine longest axis
   * @return  Which axis is the longest
   */
  static AxisType LongestAxis(const AABB<T>& aabb) {
    const T dx = aabb.max_.x_ - aabb.min_.x_;
    const T dy = aabb.max_.y_ - aabb.min_.y_;
    const T dz = aabb.max_.z_ - aabb.min_.z_;
    if (dx >= dy) {
      // dx >= dy
      if (dx >= dz) {
        return AxisType::kX;
      } else {
        return AxisType::kZ;
      }
    } else {
      // dy > dx
      if (dy >= dz) {
        return AxisType::kY;
      } else {
        return AxisType::kZ;
      }
    }
  }

  /**
   * @name  SquaredDistanceToPoint
   * @fn  static T SquaredDistanceToPoint(const Vector3<T>& p,
   *                                      const AABB<T>& bbox)
   * @brief Compute distance between a given point and an axis aligned bounding
   *        box
   * @param[in] p     Point to query distance from
   * @param[in] bbox  Bounding box to query distance to
   * @return  Squared distance
   */
  static T SquaredDistanceToPoint(const Vector3<T>& p,
                                  const AABB<T>& bbox) {
    T sq_dist = T(0.0);
    const T* p_ptr = &p.x_;
    const T* min_ptr = &bbox.min_.x_;
    const T* max_ptr = &bbox.max_.x_;
    for (int i = 0; i < 3; ++i) {
      T v = p_ptr[i];
      if (v < min_ptr[i]) sq_dist += (min_ptr[i] - v) * (min_ptr[i] - v);
      if (v > max_ptr[i]) sq_dist += (v - max_ptr[i]) * (v - max_ptr[i]);
    }
    return sq_dist;
  }

  /**
   * @name  intersection
   * @fn  bool IntersectObject(const Vector3<T>& p, const Vector3<T>& dir,
   *                           const AABB<T>& bbox, T* t)
   * @brief Test if a given bounding box intersect with a ray p + t*dir
   * @param[in]   p     Starting point of a given ray
   * @param[in]   dir   Direction of a given ray
   * @param[in]   bbox  Bounding box to test again
   * @param[out]  t     Location of the intersection
   *
   * @see "Real-Time Collision Detection" book
   */
  static bool IntersectObject(const Vector3<T>& p,
                              const Vector3<T>& dir,
                              const AABB<T>& bbox,
                              T* t) {
    // Check for intersection
    T t_enter = 0.f;
    T t_exit = 1.f;
    // For all three slabs
    const T* dir_ptr = &dir.x_;
    const T* p_ptr = &p.x_;
    const T* bbox_min_ptr = &bbox.min_.x_;
    const T* bbox_max_ptr = &bbox.max_.x_;
    for (int i = 0; i < 3; ++i) {
      if (std::abs(*dir_ptr) < 1e-9) {
        // Ray is parallel to slab. No hit if origin not within slab
        if ((*p_ptr < *bbox_min_ptr) || (*p_ptr > *bbox_max_ptr)) {
          return false;
        }
      } else {
        // Compute intersection t value of ray with near and far plane of slab
        T ood = 1.0f / *dir_ptr;
        T t_slab_enter = (*bbox_min_ptr - *p_ptr) * ood;
        T t_slab_exit = (*bbox_max_ptr - *p_ptr) * ood;
        // Make t_slab_enter be intersection with near plane, t_slab_exit with
        // far plane
        if (t_slab_enter > t_slab_exit) {
          std::swap(t_slab_enter, t_slab_exit);
        }
        // make sure the slab interval and the current box intersection
        // interval overlap
        if (t_slab_enter > t_exit || t_slab_exit < t_enter) {
          // nope. Ray missed the box.
          return false;
        } else {
          // yep, the slab and current intersection interval overlap
          // update the intersection interval
          // Compute the intersection of slab intersection intervals
          if (t_slab_enter > t_enter) { t_enter = t_slab_enter; }
          if (t_slab_exit < t_exit) { t_exit = t_slab_exit; }
          // Exit with no collision as soon as slab intersection becomes empty
          if (t_enter > t_exit) { return false; }
        }
      }
      // Inc ptr
      ++dir_ptr;
      ++p_ptr;
      ++bbox_min_ptr;
      ++bbox_max_ptr;
    }
    // Ray intersects all 3 slabs. Return point (q) and intersection t
    // value (tmin)
    *t = t_enter;
    return true;
  }

  /**
   * @name  IntersectPoint
   * @fn  static bool IntersectPoint(const AABB<T>& bbox, const Vector3<T>& point)
   * @brief Detect if a given point lies within the bounding box
   * @param[in]  bbox    Bounding box to test against
   * @param[in]  point   Point to test
   * @return True if intersect, false otherwise
   */
  static bool IntersectPoint(const AABB<T>& bbox,
                             const Vector3<T>& point) {
    return ((point.x_ >= bbox.min_.x_) && (point.x_ <= bbox.max_.x_) &&
            (point.y_ >= bbox.min_.y_) && (point.y_ <= bbox.max_.y_) &&
            (point.z_ >= bbox.min_.z_) && (point.z_ <= bbox.max_.z_));
  }

#pragma mark -
#pragma mark Comparators

  /**
   * @name LessX
   * @fn  static bool LessX(const AABB<T>& a, const AABB<T>& b)
   * @brief Compare two given bounding box X axis size
   * @param[in] a First bounding box
   * @param[in] b Second bounding box
   * @return  True if first bbox x axis is smaller than second x axis bbox,
   *          false otherwise
   */
  static bool LessX(const AABB<T>& a, const AABB<T>& b) {
    //return (a.max_.x_ - a.min_.x_) < (b.max_.x_ - b.min_.x_);
    return a.center_.x_ < b.center_.x_;
  }

  /**
   * @name LessY
   * @fn  static bool LessY(const AABB<T>& a, const AABB<T>& b)
   * @brief Compare two given bounding box Y axis size
   * @param[in] a First bounding box
   * @param[in] b Second bounding box
   * @return  True if first bbox y axis is smaller than second y axis bbox,
   *          false otherwise
   */
  static bool LessY(const AABB<T>& a, const AABB<T>& b) {
    //return (a.max_.y_ - a.min_.y_) < (b.max_.y_ - b.min_.y_);
    return a.center_.y_ < b.center_.y_;
  }

  /**
   * @name LessZ
   * @fn  static bool LessZ(const AABB<T>& a, const AABB<T>& b)
   * @brief Compare two given bounding box Z axis size
   * @param[in] a First bounding box
   * @param[in] b Second bounding box
   * @return  True if first bbox z axis is smaller than second z axis bbox,
   *          false otherwise
   */
  static bool LessZ(const AABB<T>& a, const AABB<T>& b) {
    //return (a.max_.z_ - a.min_.z_) < (b.max_.z_ - b.min_.z_);
    return a.center_.z_ < b.center_.z_;
  }

#pragma mark -
#pragma mark Members

  /** Minimum corner (i.e. top left) */
  Vector3<T> min_;
  /** Maximum corner (i.e. bottom right) */
  Vector3<T> max_;
  /** Index */
  int index_;
  /** Center */
  Vector3<T> center_;
};
  
}  // namespace FaceKit
#endif /* __FACEKIT_AABB__ */
