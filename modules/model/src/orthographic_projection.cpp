/**
 *  @file   orthographic_projection.cpp
 *  @brief
 *  @ingroup
 *
 *  @author Christophe Ecabert
 *  @date   16/08/2017
 *  Copyright (c) 2017 Christophe Ecabert. All rights reserved.
 */

#include "facekit/model/orthographic_projection.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark Initialization

/*
 * @name  OrthographicProjection
 * @fn    OrthographicProjection(const T focal, const T width, const T height)
 * @brief Constructor
 * @param[in] focal   Focal length
 * @param[in] width   Image width
 * @param[in] height  Image height
 */
template<typename T>
OrthographicProjection<T>::OrthographicProjection(const T focal,
                                                  const T width,
                                                  const T height) {
  this->focal_ = T(1.0);
  this->cx_ = width / T(2.0);
  this->cy_ = height / T(2.0);
  n_param_ = 0;
}

/*
 * @name  ToVector
 * @fn    void ToVector(T* vector)
 * @brief Export parameters to a given vector:
 *        vector = [f, cx, cy]
 * @param[in,out] vector  Vector where ot output parameters, need to be
 *                        initialized before calling
 */
template<typename T>
void OrthographicProjection<T>::ToVector(T* vector)  const {
  vector[0] = this->focal_;
  vector[1] = this->cx_;
  vector[2] = this->cy_;
}

/*
 * @name  FromVector
 * @fn    void FromVector(const T* vector)  const
 * @brief Initialize parameters from a given vector:
 *        vector = [f, cx, cy]
 * @param[in] vector  Vector holding parameters
 */
template<typename T>
void OrthographicProjection<T>::FromVector(const T* vector) {
  this->cx_ = vector[1];
  this->cy_ = vector[2];
}

#pragma mark -
#pragma mark Usage

/*
 * @name  operator()
 * @fn    void operator()(const Point3& pts, Point2* proj) const
 * @brief Project a given points
 * @param[in] pts     3D point to project
 * @param[out] proj   2D projected point
 */
template<typename T>
void OrthographicProjection<T>::operator()(const Point3& pts,
                                           Point2* proj) const {
  // Project pts
  proj->x_ = this->focal_ * pts.x_ + this->cx_;
  proj->y_ = this->focal_ * pts.y_ + this->cy_;
}

/*
 * @name  operator()
 * @fn    void operator()(const std::vector<Point3>& pts,
                          std::vector<Point2>* proj) const
 * @brief Project a list of given points
 * @param[in] pts     List of 3D points to project
 * @param[out] proj   2D projected points
 */
template<typename T>
void OrthographicProjection<T>::operator()(const std::vector<Point3>& pts,
                                           std::vector<Point2>* proj) const {
  // Init output
  proj->resize(pts.size());
  for (size_t p = 0; p < pts.size(); ++p) {
    proj->at(p).x_ = this->focal_ * pts[p].x_ + this->cx_;
    proj->at(p).y_ = this->focal_ * pts[p].y_ + this->cy_;
  }
}

#pragma mark -
#pragma mark Explicit Instantiation

/** Float */
template class OrthographicProjection<float>;
/** Double */
template class OrthographicProjection<double>;

}  // namespace FaceKit
