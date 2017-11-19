/**
 *  @file   facekit/model/camera.hpp
 *  @brief  Camera model with various projection type
 *  @ingroup    model
 *
 *  @author Christophe Ecabert
 *  @date   17/08/2017
 *  Copyright (c) 2017 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_CAMERA__
#define __FACEKIT_CAMERA__

#include <vector>

#include "facekit/core/library_export.hpp"
#include "facekit/core/math/vector.hpp"
#include "facekit/core/math/quaternion.hpp"
#include "facekit/core/math/matrix.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 * @class Camera
 * @brief   Camera behaviour abstraction
 * @author Christophe Ecabert
 * @date    17/08/2017
 * @tparam T    Data type
 * @tparam ProjType Type of camera projection
 * @ingroup model
 */
template<typename T, template<typename U> class ProjType>
class FK_EXPORTS Camera {
 public:

  /** 3D Points */
  using Point3 = typename ProjType<T>::Point3;
  /** 2D Points */
  using Point2 = typename ProjType<T>::Point2;

#pragma mark -
#pragma mark Initialization

  /**
   * @name  Camera
   * @fn    Camera(const T focal, const T width, const T height)
   * @brief Constructor
   * @param[in] focal   Focal length
   * @param[in] width   Image width
   * @param[in] height  Image height
   */
  Camera(const T focal, const T width, const T height);

  /**
   * @name  Camera
   * @fn    Camera(const Camera& other)
   * @brief Copy Constructor
   * @param[in] other Object to copy from
   */
  Camera(const Camera& other) = delete;

  /**
   * @name  operator=
   * @fn    Camera& operator=(const Camera& rhs) = delete
   * @brief Assignment operator
   * @param[in] rhs Object to assign from
   * @return Newly assigned object
   */
  Camera& operator=(const Camera& rhs) = delete;

  /**
   * @name  Camera
   * @fn    Camera(const Camera& other) = delete
   * @brief Move Constructor
   * @param[in] other Object to move from
   */
  Camera(Camera&& other) = delete;

  /**
   * @name  operator=
   * @fn    Camera& operator=(Camera&& rhs) = delete
   * @brief Assignment operator
   * @param[in] rhs Object to move assign from
   * @return Newly moved assign object
   */
  Camera& operator=(Camera&& rhs) = delete;

  /**
   * @name  ~Camera(void) = default
   * @fn    ~Camera
   * @brief Destructor
   */
  ~Camera(void) = default;

  /**
   * @name  ToVector
   * @fn    void ToVector(T* vector) const
   * @brief Export parameters to a given vector:
   *        vector = [f cx cy qx qy qz qw tx ty tz]
   * @param[in,out] vector  Vector where ot output parameters, need to be
   *                        initialized before calling
   */
  void ToVector(T* vector) const;

  /**
   * @name  FromVector
   * @fn    void FromVector(const T* vector)
   * @brief Initialize parameters from a given vector:
   *        vector = [f cx cy qx qy qz qw tx ty tz]
   * @param[in] vector  Vector holding parameters
   */
  void FromVector(const T* vector);

  /**
   * @name  From3Dto2D
   * @fn    int From3Dto2D(const std::vector<Point3>& pts,
                           const std::vector<Point2>& proj,
                           const T eps)
   * @brief Initialize camera transformation (R, T) from a set of matching pairs
   *        3D-2D.
   * @param[in] pts     List of 3D points
   * @param[in] proj    List of corresponding projected points
   * @param[in] eps     Stopping criterion
   * @return    -2 if numerical error, -1 if not converged, 0 otherwise
   */
  int From3Dto2D(const std::vector<Point3>& pts,
                 const std::vector<Point2>& proj,
                 const T eps);

  /**
   * @name  InitFrom3Dto2D
   * @fn    void From3Dto2D(const cv::Mat& pts, const cv::Mat& proj,const T eps)
   * @brief Initialize camera transformation (R, T) from a set of matching pairs
   *        3D-2D.
   * @param[in] pts     List of 3D points [3N x 1] or [1 x 3N]
   * @param[in] proj    List of corresponding projected points [2N x 1] or
   *                    [1 x 2N]
   * @param[in] eps     Stopping criterion
   * @return    -2 if numerical error, -1 if not converged, 0 otherwise
   */
  int From3Dto2D(const cv::Mat& pts,
                 const cv::Mat& proj,
                 const T eps);

#pragma mark -
#pragma mark Usage

  /**
   * @name  operator()
   * @fn    void operator()(const Point3& pts,
                            Point2* proj) const
   * @brief Project one given \p pts with complete transformation
   * @param[in] pts     3D Point to project
   * @param[out] proj   Projected 2D point
   */
  void operator()(const Point3& pts, Point2* proj) const;

  /**
   * @name  operator()
   * @fn    void operator()(const std::vector<Point3>& pts,
                            std::vector<Point2>* proj) const
   * @brief Project a list of \p pts with complete transformation
   * @param[in] pts     3D Points to project
   * @param[out] proj   Projected 2D points
   */
  void operator()(const std::vector<Point3>& pts,
                  std::vector<Point2>* proj) const;

  /**
   * @name  operator()
   * @fn    void operator()(const cv::Mat& pts,
                            cv::Mat* proj) const
   * @brief Project a list of \p pts with complete transformation
   * @param[in] pts     3D Points to project [3N x 1] or [1 x 3N]
   * @param[out] proj   Projected 2D points [2N x 1]
   */
  void operator()(const cv::Mat& pts, cv::Mat* proj) const;

#pragma mark -
#pragma mark Accessors

  /**
   * @name  get_focal_length
   * @fn    T get_focal_length(void) const
   * @brief Provide camera's focal length
   * @return    Focal length
   */
  T get_focal_length(void) const {
    return p_.get_focal_length();
  }

  /**
   * @name  get_principal_point_x
   * @fn    T get_principal_point_x(void) const
   * @brief Provide camera's principal point in X direction
   * @return    X's Principal point
   */
  T get_principal_point_x(void) const {
    return p_.get_principal_point_x();
  }

  /**
   * @name  get_principal_point_y
   * @fn    T get_principal_point_y(void) const
   * @brief Provide camera's principal point in Y direction
   * @return    Y's Principal point
   */
  T get_principal_point_y(void) const {
    return p_.get_principal_point_y();
  }

  /**
   * @name  get_rotation
   * @fn    const Matrix3x3<T>& get_rotation(void) const
   * @brief Provide view transformation (Rotation only)
   * @return  Rotation as quaternion
   */
  const Quaternion<T>& get_rotation(void) const {
    return rot_;
  }

  /**
   * @name  get_rotation
   * @fn    Quaternion<T>& get_rotation(void)
   * @brief Provide view transformation (Rotation only)
   * @return  Rotation as quaternion
   */
  Quaternion<T>& get_rotation(void) {
    return rot_;
  }

  /**
   * @name  get_rotation_matrix
   * @fn    const Matrix3<T>& get_rotation_matrix(void) const
   * @brief Provide view transformation (Rotation only)
   * @return  Rotation matrix
   */
  const Matrix3<T>& get_rotation_matrix(void) const {
    return rotm_;
  }

  /**
   * @name  get_translation
   * @fn    const Vector3<T>& get_translation(void) const
   * @brief Provide view transformation (Translation only)
   * @return  Translation
   */
  const Vector3<T>& get_translation(void) const {
    return t_;
  }

  /**
   * @name get_n_parameter
   * @fn    int get_n_parameter(void) const
   * @brief Provide the amount of parameters that can be optimized in this
   *        camera
   * @return    Number of camera parameters
   */
  int get_n_parameter(void) const {
    // 3 quaternion + 3 translation
    return 6 + p_.get_n_parameter();
  }

  /**
   * @name  set_axis_inversion
   * @fn    void set_axis_inversion(const T* axis)
   * @brief Set axis inversion for conversion between different reference system
   * @param[in] axis    Axis inversion
   */
  void set_axis_inversion(const T* axis) {
    ax_[0] = axis[0];
    ax_[1] = axis[1];
    ax_[2] = axis[2];
  }

  /**
   * @name  get_axis_inversion
   * @fn    const T* get_axis_inversion(void) const
   * @brief Get current axis inversion
   * @return Axis inversion
   */
  const T* get_axis_inversion(void) const {
    return ax_;
  }

  /**
   * @name  get_view_transform
   * @fn    Matrix4<T> get_view_transform(void) const
   * @brief Provide View tranformation for this camera T = [R | T; 0 0 0 1]
   * @return    View transform
   */
  Matrix4<T> get_view_transform(void) const;

#pragma mark -
#pragma mark Private
 private:

  /** Translation */
  Vector3<T> t_;
  /** Rotation */
  Quaternion<T> rot_;
  /** Rotation Matrix */
  Matrix3<T> rotm_;
  /** Projection */
  ProjType<T> p_;
  /** Axis inversion */
  T ax_[3];
};


}  // namespace FaceKit
#endif //__FACEKIT_CAMERA__
