/**
 *  @file   camera.cpp
 *  @brief  Camera model with various projection type
 *  @ingroup    model
 *
 *  @author Christophe Ecabert
 *  @date   17/08/2017
 *  Copyright (c) 2017 Christophe Ecabert. All rights reserved.
 */

#include "opencv2/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui.hpp"

#include "facekit/core/logger.hpp"
#include "facekit/model/camera.hpp"
#include "facekit/model/orthographic_projection.hpp"
#include "facekit/model/weak_projection.hpp"
#include "facekit/model/perspective_projection.hpp"
#include "facekit/core/math/linear_algebra.hpp"
#include "facekit/io/file_io.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark Initialization

/*
 * @name  Camera
 * @fn    Camera(const T focal, const T width, const T height)
 * @brief Constructor
 * @param[in] focal   Focal length
 * @param[in] width   Image width
 * @param[in] height  Image height
 */
template<typename T, template<typename U> class ProjType>
Camera<T, ProjType>::Camera(const T focal,
                            const T width,
                            const T height) : p_(focal, width, height),
                                              ax_{T(1.0), T(1.0), T(1.0)} {
  rot_.ToRotationMatrix(&rotm_);
  t_.z_ = focal;
}


/*
 * @name  ToVector
 * @fn    void ToVector(T* vector)
 * @brief Export parameters to a given vector:
 *        vector = [f cx cy qx qy qz qw tx ty tz]
 * @param[in,out] vector  Vector where ot output parameters, need to be
 *                        initialized before calling
 */
template<typename T, template<typename U> class ProjType>
void Camera<T, ProjType>::ToVector(T* vector) const {
  p_.ToVector(vector);
  // Rotation
  vector[3] = rot_.v_.x_;
  vector[4] = rot_.v_.z_;
  vector[5] = rot_.v_.y_;
  vector[6] = rot_.q_;
  // T
  vector[7] = t_.x_;
  vector[8] = t_.y_;
  vector[9] = t_.z_;
}

/*
 * @name  FromVector
 * @fn    void FromVector(const T* vector)
 * @brief Initialize parameters from a given vector:
 *        vector = [f cx cy qx qy qz qw tx ty tz]
 * @param[in] vector  Vector holding parameters
 */
template<typename T, template<typename U> class ProjType>
void Camera<T, ProjType>::FromVector(const T* vector) {
  p_.FromVector(vector);
  // Rot
  rot_.v_.x_ = vector[3];
  rot_.v_.y_ = vector[4];
  rot_.v_.z_ = vector[5];
  rot_.q_ = vector[6];
  rot_.Normalize();
  rot_.ToRotationMatrix(&rotm_);
  // T
  t_.x_ = vector[7];
  t_.y_ = vector[8];
  t_.z_ = vector[9];
}

/*
 * @name  From3Dto2D
 * @fn    int From3Dto2D(const std::vector<Point3>& pts,
                         const std::vector<Point2>& proj)
 * @brief Initialize camera transformation (R, T) from a set of matching pairs
 *        3D-2D.
 * @param[in] pts     List of 3D points
 * @param[in] proj    List of corresponding projected points
 * @param[in] eps     Stopping criterion
 * @return    -2 if numerical error, -1 if not converged, 0 otherwise
 */
template<typename T, template<typename U> class ProjType>
int Camera<T, ProjType>::From3Dto2D(const std::vector<Point3>& pts,
                                    const std::vector<Point2>& proj,
                                    const T eps) {
  // Init accessor for data
  cv::Mat p3s(static_cast<int>(pts.size() * 3),
              1,
              cv::DataType<T>::type,
              (void*)pts.data());
  cv::Mat p2s(static_cast<int>(proj.size() * 2),
              1,
              cv::DataType<T>::type,
              (void*)proj.data());
  return this->From3Dto2D(p3s, p2s, eps);
}

/*
 * @struct  JacobianHelper
 * @brief   Helper functor to compute projection jacobian matrix
 * @tparam T Data type
 * @tparam ProjType Projection type (ortho, weak, perspective)
 */
template<typename T, template<typename U> class ProjType>
struct JacobianHelper {
  void operator()(const cv::Mat& pts,
                  const Camera<T, ProjType>* camera,
                  cv::Mat* j_proj) {}
};

/*
 * @struct  JacobianHelper
 * @brief   Helper functor to compute projection jacobian matrix
 *          Specialization for orthographic projection
 * @tparam T Data type
 */
template<typename T>
struct JacobianHelper<T, OrthographicProjection> {
  void operator()(const cv::Mat& pts,
                  const Camera<T, OrthographicProjection>* camera,
                  cv::Mat* j_proj) {
    // Access vertex
    const auto* ptr3d = reinterpret_cast<const Vector3<T>*>(pts.data);
    const int n3 = std::max(pts.cols, pts.rows) / 3;
    // Access camera properties
    const int nc = camera->get_n_parameter();
    const auto& q = camera->get_rotation();
    const auto& rot = camera->get_rotation_matrix();
    const auto& t = camera->get_translation();
    const T f = camera->get_focal_length();
    const T* ax = camera->get_axis_inversion();
    // Fill Jacobian
    j_proj->create(2 * n3, nc - 1, cv::DataType<T>::type);
    for (int i = 0; i < n3; i++) {
      int ix = 2 * i;
      int iy = ix + 1;
      // Untransformed vertex
      auto v = ptr3d[i];
      v.x_ *= ax[0];
      v.y_ *= ax[1];
      v.z_ *= ax[2];
      // Transformed vertex
      const auto vx = (rot * v) + t;
      // Q1 derivative
      T dvx_dq = T(2.0) * (q.v_.y_ * v.y_ + q.v_.z_ * v.z_);
      T dvy_dq = T(2.0) * (q.v_.y_ * v.x_ - T(2.0) * q.v_.x_ * v.y_ - q.q_ * v.z_);
      j_proj->at<T>(ix, 0) = f * dvx_dq;
      j_proj->at<T>(iy, 0) = f * dvy_dq;
      // Q2 derivative
      dvx_dq = T(2.0) * (-T(2.0) * q.v_.y_ * v.x_ + q.v_.x_ * v.y_ + q.q_ * v.z_);
      dvy_dq = T(2.0) * (q.v_.x_ * v.x_ + q.v_.z_ * v.z_);
      j_proj->at<T>(ix, 1) = f * dvx_dq;
      j_proj->at<T>(iy, 1) = f * dvy_dq;
      // Q3 derivative
      dvx_dq = T(2.0) * (-T(2.0) * q.v_.z_ * v.x_ - q.q_ * v.y_ + q.v_.x_ * v.z_);
      dvy_dq = T(2.0) * (q.q_ * v.x_ - T(2.0) * q.v_.z_ * v.y_ + q.v_.y_ * v.z_);
      j_proj->at<T>(ix, 2) = f * dvx_dq;
      j_proj->at<T>(iy, 2) = f * dvy_dq;
      // Tx derivative
      j_proj->at<T>(ix, 3) = T(1.0);
      j_proj->at<T>(iy, 3) = T(0.0);
      // Ty derivative
      j_proj->at<T>(ix, 4) = T(0.0);
      j_proj->at<T>(iy, 4) = T(1.0);
    }
  }
};

/**
 * @struct  JacobianHelper
 * @brief   Helper functor to compute projection jacobian matrix
 *          Specialization for weak projection
 * @tparam T Data type
 */
template<typename T>
struct JacobianHelper<T, WeakProjection> {
  void operator()(const cv::Mat& pts,
                  const Camera<T, WeakProjection>* camera,
                  cv::Mat* j_proj) {
    // Access vertex
    const auto* ptr3d = reinterpret_cast<const Vector3<T>*>(pts.data);
    const int n3 = std::max(pts.cols, pts.rows) / 3;
    // Access camera properties
    const int nc = camera->get_n_parameter();
    const auto& q = camera->get_rotation();
    const auto& rot = camera->get_rotation_matrix();
    const auto& t = camera->get_translation();
    const T f = camera->get_focal_length();
    const T* ax = camera->get_axis_inversion();
    // Fill Jacobian
    j_proj->create(2 * n3, nc - 1, cv::DataType<T>::type);
    for (int i = 0; i < n3; i++) {
      int ix = 2 * i;
      int iy = ix + 1;
      // Untransformed vertex
      auto v = ptr3d[i];
      v.x_ *= ax[0];
      v.y_ *= ax[1];
      v.z_ *= ax[2];
      // Transformed vertex
      const auto vx = (rot * v) + t;
      // f derivative
      j_proj->at<T>(ix, 0) = vx.x_;
      j_proj->at<T>(iy, 0) = vx.y_;
      // Q1 derivative
      T dvx_dq = T(2.0) * (q.v_.y_ * v.y_ + q.v_.z_ * v.z_);
      T dvy_dq = T(2.0) * (q.v_.y_ * v.x_ - T(2.0) * q.v_.x_ * v.y_ - q.q_ * v.z_);
      j_proj->at<T>(ix, 1) = f * dvx_dq;
      j_proj->at<T>(iy, 1) = f * dvy_dq;
      // Q2 derivative
      dvx_dq = T(2.0) * (-T(2.0) * q.v_.y_ * v.x_ + q.v_.x_ * v.y_ + q.q_ * v.z_);
      dvy_dq = T(2.0) * (q.v_.x_ * v.x_ + q.v_.z_ * v.z_);
      j_proj->at<T>(ix, 2) = f * dvx_dq;
      j_proj->at<T>(iy, 2) = f * dvy_dq;
      // Q3 derivative
      dvx_dq = T(2.0) * (-T(2.0) * q.v_.z_ * v.x_ - q.q_ * v.y_ + q.v_.x_ * v.z_);
      dvy_dq = T(2.0) * (q.q_ * v.x_ - T(2.0) * q.v_.z_ * v.y_ + q.v_.y_ * v.z_);
      j_proj->at<T>(ix, 3) = f * dvx_dq;
      j_proj->at<T>(iy, 3) = f * dvy_dq;
      // Tx derivative
      j_proj->at<T>(ix, 4) = f;
      j_proj->at<T>(iy, 4) = T(0.0);
      // Ty derivative
      j_proj->at<T>(ix, 5) = T(0.0);
      j_proj->at<T>(iy, 5) = f;
    }
  }
};

/**
 * @struct  JacobianHelper
 * @brief   Helper functor to compute projection jacobian matrix
 *          Specialization for perspective projection
 * @tparam T Data type
 */
template<typename T>
struct JacobianHelper<T, PerspectiveProjection> {
  void operator()(const cv::Mat& pts,
                  const Camera<T, PerspectiveProjection>* camera,
                  cv::Mat* j_proj) {
    // Access vertex
    const auto* ptr3d = reinterpret_cast<const Vector3<T>*>(pts.data);
    const int n3 = std::max(pts.cols, pts.rows) / 3;
    // Access camera properties
    const int nc = camera->get_n_parameter();
    const auto& q = camera->get_rotation();
    const auto& rot = camera->get_rotation_matrix();
    const auto& t = camera->get_translation();
    const T f = camera->get_focal_length();
    const T* ax = camera->get_axis_inversion();
    // Fill Jacobian
    j_proj->create(2 * n3, nc, cv::DataType<T>::type);
    for (int i = 0; i < n3; i++) {
      int ix = 2 * i;
      int iy = ix + 1;
      // Untransformed vertex
      auto v = ptr3d[i];
      v.x_ *= ax[0];
      v.y_ *= ax[1];
      v.z_ *= ax[2];
      // Transformed vertex
      const auto vx = (rot * v) + t;
      const T ivzz = T(1.0) / (vx.z_ * vx.z_);
      // f derivative
      j_proj->at<T>(ix, 0) = vx.x_ / vx.z_;
      j_proj->at<T>(iy, 0) = vx.y_ / vx.z_;
      // Q1 derivative
      T dvx_dq = T(2.0) * (q.v_.y_ * v.y_ + q.v_.z_ * v.z_);
      T dvy_dq = T(2.0) * (q.v_.y_ * v.x_ - T(2.0) * q.v_.x_ * v.y_ - q.q_ * v.z_);
      T dvz_dq = T(2.0) * (q.v_.z_ * v.x_ + q.q_ * v.y_  - T(2.0) * q.v_.x_ * v.z_);
      j_proj->at<T>(ix, 1) = f * (dvx_dq * vx.z_ - vx.x_ * dvz_dq) * ivzz;
      j_proj->at<T>(iy, 1) = f * (dvy_dq * vx.z_ - vx.y_ * dvz_dq) * ivzz;
      // Q2 derivative
      dvx_dq = T(2.0) * (-T(2.0) * q.v_.y_ * v.x_ + q.v_.x_ * v.y_ + q.q_ * v.z_);
      dvy_dq = T(2.0) * (q.v_.x_ * v.x_ + q.v_.z_ * v.z_);
      dvz_dq = T(2.0) * (-q.q_ * v.x_ + q.v_.z_ * v.y_ - T(2.0) * q.v_.y_ * v.z_);
      j_proj->at<T>(ix, 2) = f * (dvx_dq * vx.z_ - vx.x_ * dvz_dq) * ivzz;
      j_proj->at<T>(iy, 2) = f * (dvy_dq * vx.z_ - vx.y_ * dvz_dq) * ivzz;
      // Q3 derivative
      dvx_dq = T(2.0) * (-T(2.0) * q.v_.z_ * v.x_ - q.q_ * v.y_ + q.v_.x_ * v.z_);
      dvy_dq = T(2.0) * (q.q_ * v.x_ - T(2.0) * q.v_.z_ * v.y_ + q.v_.y_ * v.z_);
      dvz_dq = T(2.0) * (q.v_.x_ * v.x_ + q.v_.y_ * v.y_);
      j_proj->at<T>(ix, 3) = f * (dvx_dq * vx.z_ - vx.x_ * dvz_dq) * ivzz;
      j_proj->at<T>(iy, 3) = f * (dvy_dq * vx.z_ - vx.y_ * dvz_dq) * ivzz;
      // Tx derivative
      j_proj->at<T>(ix, 4) = f / vx.z_;
      j_proj->at<T>(iy, 4) = T(0.0);
      // Ty derivative
      j_proj->at<T>(ix, 5) = T(0.0);
      j_proj->at<T>(iy, 5) = f / vx.z_;
      // Tz derivative
      j_proj->at<T>(ix, 6) = -f * (vx.x_ * ivzz);
      j_proj->at<T>(iy, 6) = -f * (vx.y_ * ivzz);
    }
  }
};




/*
 * @name  InitFrom3Dto2D
 * @fn    void From3Dto2D(const cv::Mat& pts,
                          const cv::Mat& proj)
 * @brief Initialize camera transformation (R, T) from a set of matching pairs
 *        3D-2D.
 * @param[in] pts     List of 3D points [3N x 1] or [1 x 3N]
 * @param[in] proj    List of corresponding projected points [2N x 1] or
 *                    [1 x 2N]
 * @param[in] eps     Stopping criterion
 * @return    -2 if numerical error, -1 if not converged, 0 otherwise
 */
template<typename T, template<typename U> class ProjType>
int Camera<T, ProjType>::From3Dto2D(const cv::Mat& pts,
                                    const cv::Mat& proj,
                                    const T eps) {
  using LA = LinearAlgebra<T>;
  using Solver = typename LinearAlgebra<T>::SquareLinearSolver;
  using TType = typename LinearAlgebra<T>::TransposeType;
  int err = -1;
  const int N = 100;
  const int n3 = std::max(pts.rows, pts.cols) / 3;
  assert((std::max(proj.rows, proj.cols) / 2) == n3);
  // Ensure the object lie in front of the camera while before optimizing.
  T min_z = std::numeric_limits<T>::max();
  T max_z = -min_z;
  const auto* ptr3d = reinterpret_cast<Vector3<T>*>(pts.data);
  for (int i = 0; i < n3; ++i) {
    const auto& p3d = ptr3d[i];
    // Minimum 3D
    min_z = p3d.z_ < min_z ? p3d.z_ : min_z;
    // Maximum 3D
    max_z = p3d.z_ > max_z ? p3d.z_ : max_z;
  }
  min_z *= ax_[2];
  max_z *= ax_[2];
  this->t_.z_ = max_z > T(0.0) ? max_z : -max_z;
  this->t_.z_ *= T(2.0);
  // Init solver
  const int nc = this->get_n_parameter();
  const int idx_q_update = this->p_.get_n_parameter() == 1 ? 1 : 0;
  T p_cam[10];
  cv::Mat j_proj;
  cv::Mat err_proj;   // Projection error
  cv::Mat curr_proj;  // Current projection (2d pts)
  cv::Mat hessian;    // Hessian
  cv::Mat sd;
  cv::Mat update;     // Parameter update
  int iter = 0;
  static Solver solver;
  T res = std::numeric_limits<T>::max();
  T prev_res = std::numeric_limits<T>::min();
  const T cx = p_.get_principal_point_x();
  const T cy = p_.get_principal_point_y();
  // Helper functor for Jacobian computation
  JacobianHelper<T, ProjType> jhelper;
  while (iter < N && std::abs(res - prev_res) > eps) {
    // Updated focal
    const T f = this->get_focal_length();
    // Project
    this->operator()(pts, &curr_proj);
    // Compute err
    err_proj = proj - curr_proj;
    // Fill jacobian focal + rotation + T
    jhelper(pts, this, &j_proj);
    // Compute Hessian + Sd
    LA::Gemm(j_proj, TType::kTranspose, T(1.0),
             j_proj, TType::kNoTranspose, T(0.0), &hessian);
    LA::Gemv(j_proj, TType::kTranspose, T(1.0), err_proj, T(0.0), &sd);
    // Compute increment
    solver.Solve(hessian, sd, &update);
    if (!update.empty()) {
      // update camera
      auto dq = *(update.ptr<const Quaternion<T>>(idx_q_update));
      dq.q_ = T(1.0);
      dq.Normalize();
      auto &q = reinterpret_cast<Quaternion<T>*>(&p_cam[3])[0];
      p_cam[0] = idx_q_update == 1 ? f + update.at<T>(0) : f;
      p_cam[1] = cx;
      p_cam[2] = cy;
      q = dq * rot_;
      p_cam[7] = this->t_.x_ + update.at<T>(idx_q_update + 3);
      p_cam[8] = this->t_.y_ + update.at<T>(idx_q_update + 4);
      p_cam[9] = update.rows == nc ?
                 this->t_.z_ + update.at<T>(idx_q_update + 5) :
                 this->t_.z_;
      // Push update to camera
      this->FromVector(&p_cam[0]);
      // Inc counter
      ++iter;
      // Compute residual
      prev_res = res;
      res = LA::L2Norm(err_proj);
    } else {
      err = -2;
      break;
    }
  }
  err = (err == -2) ? err : iter == N ? -1 : 0;
  return err;
};


#pragma mark -
#pragma mark Usage

/*
 * @name  operator()
 * @fn    void operator()(const Point3& pts,
                          const T* axis,
                          Point2* proj) const
 * @brief Project one given \p pts with complete transformation
 * @param[in] pts     3D Point to project
 * @param[out] proj   Projected 2D point
 */
template<typename T, template<typename U> class ProjType>
void Camera<T, ProjType>::operator()(const Point3& pts,
                                     Point2* proj) const {
  // Transform pts
  auto q = pts;
  q.x_ *= ax_[0];
  q.y_ *= ax_[1];
  q.z_ *= ax_[2];
  q = (rotm_ * q) + t_;
  // Projection
  p_(q, proj);
}

/*
 * @name  operator()
 * @fn    void operator()(const std::vector<Point3>& pts,
                          const T* axis,
                          std::vector<Point2>* proj) const
 * @brief Project a list of \p pts with complete transformation
 * @param[in] pts     3D Points to project
 * @param[out] proj   Projected 2D points
 */
template<typename T, template<typename U> class ProjType>
void Camera<T, ProjType>::operator()(const std::vector<Point3>& pts,
                                     std::vector<Point2>* proj) const {
  // Init
  proj->resize(pts.size());
  std::vector<Point3> ps(pts.size());
  cv::Mat pts3(int(pts.size() * 3),
               1,
               cv::DataType<T>::type,
               (void*)pts.data());
  cv::Mat pts2(int(pts.size() * 2),
               1,
               cv::DataType<T>::type,
               (void*)proj->data());
  this->operator()(pts3, &pts2);
}

/*
 * @name  operator()
 * @fn    void operator()(const cv::Mat& pts,
                          const T* axis,
                          cv::Mat* proj) const
 * @brief Project a list of \p pts with complete transformation
 * @param[in] pts     3D Points to project [3N x 1] or [1 x 3N]
 * @param[out] proj   Projected 2D points [2N x 1]
 */
template<typename T, template<typename U> class ProjType>
void Camera<T, ProjType>::operator()(const cv::Mat& pts,
                                     cv::Mat* proj) const {
  // Init
  assert(pts.rows % 3 == 0 || pts.cols % 3 == 0);
  const int n = std::max(pts.cols, pts.rows) / 3;
  proj->create(2 * n, 1, cv::DataType<T>::type);
  const auto* src = reinterpret_cast<const Point3*>(pts.data);
  auto* dst = reinterpret_cast<Point2*>(proj->data);
  for (int i = 0; i < n; ++i) {
    auto v = src[i];
    v.x_ *= ax_[0];
    v.y_ *= ax_[1];
    v.z_ *= ax_[2];
    const auto vx = (rotm_ * v) + t_;
    p_(vx, &dst[i]);
  }
}

#pragma mark -
#pragma mark Accessors

/*
 * @name  get_view_transform
 * @fn    cv::Mat get_view_transform(void) const
 * @brief Provide View tranformation for this camera T = [R | T; 0 0 0 1]
 * @return    View transform
 */
template<typename T, template<typename U> class ProjType>
Matrix4<T> Camera<T, ProjType>::get_view_transform(void) const {
  Matrix4<T> n_mat;
  n_mat[0] = rotm_[0];
  n_mat[4] = rotm_[3];
  n_mat[8] = rotm_[6];
  n_mat[12] = t_.x_;
  n_mat[1] = rotm_[1];
  n_mat[5] = rotm_[4];
  n_mat[9] = rotm_[7];
  n_mat[13] = t_.y_;
  n_mat[2] = rotm_[2];
  n_mat[6] = rotm_[5];
  n_mat[10] = rotm_[8];
  n_mat[14] = t_.z_;
  return n_mat;
}

#pragma mark -
#pragma mark Explicit Instantiation

/** Float - Ortho */
template class Camera<float, OrthographicProjection>;
/** Double - Ortho */
template class Camera<double, OrthographicProjection>;

/** Float - Weak */
template class Camera<float, WeakProjection>;
/** Double - Weak */
template class Camera<double, WeakProjection>;

/** Float - Perspective */
template class Camera<float, PerspectiveProjection>;
/** Double - Perspective */
template class Camera<double, PerspectiveProjection>;

}  // namespace FaceKit
