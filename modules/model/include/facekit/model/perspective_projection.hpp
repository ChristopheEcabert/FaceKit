/**
 *  @file   weak_projection.hpp
 *  @brief  Abstraction for perspective geometric projection
 *  @ingroup model
 *
 *  @author Christophe Ecabert
 *  @date   16/08/2017
 *  Copyright (c) 2017 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_PERSPECTIVE_PROJECTION__
#define __FACEKIT_PERSPECTIVE_PROJECTION__

#include <vector>

#include "opencv2/core/core.hpp"

#include "facekit/core/library_export.hpp"
#include "facekit/core/math/vector.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 * @class   PerspectiveProjection
 * @brief   Abstraction for orthographic geometric projection
 * @author  Christophe Ecabert
 * @date    16/08/2017
 * @ingroup model
 * @tparam T  Data type
 */
template<typename T>
class FK_EXPORTS PerspectiveProjection {
 public:

  /** Points */
  using Point3 = Vector3<T>;
  /** Points */
  using Point2 = Vector2<T>;

#pragma mark -
#pragma mark Initialization

  /**
   * @name  PerspectiveProjection
   * @fn    PerspectiveProjection(const T focal, const T width, const T height)
   * @brief Constructor
   * @param[in] focal   Focal length
   * @param[in] width   Image width
   * @param[in] height  Image height
   */
  PerspectiveProjection(const T focal, const T width, const T height);

  /**
   * @name  PerspectiveProjection
   * @fn    PerspectiveProjection(const PerspectiveProjection& other)
   * @brief Copy Constructor
   * @param[in] other Object to copy from
   */
  PerspectiveProjection(const PerspectiveProjection& other) = delete;

  /**
   * @name  operator=
   * @fn    PerspectiveProjection& operator=(const PerspectiveProjection& rhs) = delete
   * @brief Assignment operator
   * @param[in] rhs Object to assign from
   * @return Newly assigned object
   */
  PerspectiveProjection& operator=(const PerspectiveProjection& rhs) = delete;

  /**
   * @name  PerspectiveProjection
   * @fn    PerspectiveProjection(const PerspectiveProjection& other) = delete
   * @brief Move Constructor
   * @param[in] other Object to move from
   */
  PerspectiveProjection(PerspectiveProjection&& other) = delete;

  /**
   * @name  operator=
   * @fn    PerspectiveProjection& operator=(PerspectiveProjection&& rhs) = delete
   * @brief Assignment operator
   * @param[in] rhs Object to move assign from
   * @return Newly moved assign object
   */
  PerspectiveProjection& operator=(PerspectiveProjection&& rhs) = delete;

  /**
   * @name  ~PerspectiveProjection(void) = default
   * @fn    ~PerspectiveProjection
   * @brief Destructor
   */
  ~PerspectiveProjection(void) = default;

  /**
   * @name  ToVector
   * @fn    void ToVector(T* vector) const
   * @brief Export parameters to a given vector:
   *        vector = [f, cx, cy]
   * @param[in,out] vector  Vector where ot output parameters, need to be
   *                        initialized before calling
   */
  void ToVector(T* vector) const;

  /**
   * @name  FromVector
   * @fn    void FromVector(const T* vector)
   * @brief Initialize parameters from a given vector:
   *        vector = [f, cx, cy]
   * @param[in] vector  Vector holding parameters
   */
  void FromVector(const T* vector);

#pragma mark -
#pragma mark Usage

  /**
   * @name  operator()
   * @fn    void operator()(const Point3& pts, Point2* proj) const
   * @brief Project a given points
   * @param[in] pts     3D point to project
   * @param[out] proj   2D projected point
   */
  void operator()(const Point3& pts, Point2* proj) const;

  /**
   * @name  operator()
   * @fn    void operator()(const std::vector<Point3>& pts,
                            std::vector<Point2>* proj) const
   * @brief Project a list of given points
   * @param[in] pts     List of 3D points to project
   * @param[out] proj   2D projected points
   */
  void operator()(const std::vector<Point3>& pts,
                  std::vector<Point2>* proj) const;

#pragma mark -
#pragma mark Accessors

  /**
   * @name  get_n_parameter
   * @fn    int get_n_parameter(void) const
   * @brief Provide the number of free parameters for the projection
   * @return    Number of parameter for the projection
   */
  int get_n_parameter(void) const {
    return n_param_;
  }

  /**
   * @name  get_focal_length
   * @fn    T get_focal_length(void) const
   * @brief Provide camera's focal length
   * @return    Focal length
   */
  T get_focal_length(void) const {
    return focal_;
  }

  /**
   * @name  get_principal_point_x
   * @fn    T get_principal_point_x(void) const
   * @brief Provide camera's principal point in X direction
   * @return    X's Principal point
   */
  T get_principal_point_x(void) const {
    return cx_;
  }

  /**
   * @name  get_principal_point_y
   * @fn    T get_principal_point_y(void) const
   * @brief Provide camera's principal point in Y direction
   * @return    Y's Principal point
   */
  T get_principal_point_y(void) const {
    return cy_;
  }

#pragma mark -
#pragma mark Private
 private:
  /** Focal length */
  T focal_;
  /** Image Center - X */
  T cx_;
  /** Image Center - X */
  T cy_;
  /** Numer of parameters */
  int n_param_;
};

}  // namespace FaceKit
#endif //__FACEKIT_PERSPECTIVE_PROJECTION__
