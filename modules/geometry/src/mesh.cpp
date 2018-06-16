/**
 *  @file   mesh.cpp
 *  @brief  3D Mesh container
 *
 *  @author Christophe Ecabert
 *  @date   02/08/16
 *  Copyright © 2016 Christophe Ecabert. All rights reserved.
 */

#include <assert.h>
#include <fstream>
#include <sstream>
#ifdef __APPLE__
#include <dispatch/dispatch.h>
#endif

#include "ply.h"

#include "facekit/geometry/mesh.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {

#pragma mark -
#pragma mark Type definition
  
/**
 *  @struct PLYVertex
 *  @brief  Data object to read ply file
 */
template<typename T>
struct PLYVertex {
  /** Vertex typedef */
  using Vertex = typename Mesh<T>::Vertex;
  /** Normal typedef */
  using Normal = typename Mesh<T>::Normal;
  
  /** Position */
  Vertex vertex;
  /** Normal */
  Normal normal;
  
  /**
   *  @name PLYVertex
   *  @fn PLYVertex(void)
   *  @brief  Constructor
   */
  PLYVertex(void) : vertex(-1.0, -1.0, -1.0),
                    normal(-1.0, -1.0, -1.0) {}
};
  
/**
 *  @struct PLYFace
 *  @brief  Data object to read ply file
 */
template<typename T>
struct PLYFace {
  /** Triangle typedef */
  using Triangle = typename Mesh<T>::Triangle;
  /** Texture coordinate typedef */
  using TCoord = typename Mesh<T>::TCoord;
  
  /** number of vertex in the face */
  int n_vertex;
  /** List of vertex indices */
  Triangle* list_idx;
  /** number of tcoord */
  int n_tcoord;
  /** List of texture coordinate */
  TCoord* list_tcoord;
  
  /**
   *  @name PLYFace(void)
   *  @fn PLYFace(void)
   *  @brief  Constructor
   */
  PLYFace(void) : list_idx(nullptr),
                  list_tcoord(nullptr) {}
  
  ~PLYFace(void) {
    if (list_idx) {
      free(list_idx);
      list_idx = nullptr;
    }
    if (list_tcoord) {
      free(list_tcoord);
      list_tcoord = nullptr;
    }
  }
};

#pragma mark -
#pragma mark Initialization

/*
 *  @name
 *  @fn
 *  @brief  Constructor
 */
template<typename T>
Mesh<T>::Mesh(void) : bbox_is_computed_(false) {
}

/*
 *  @name Mesh
 *  @fn Mesh(const std::string& filename)
 *  @brief  Load mesh from supported file :
 *            .obj, .ply, .tri
 */
template<typename T>
Mesh<T>::Mesh(const std::string& filename) : bbox_is_computed_(false){
  if (this->Load(filename)) {
    std::cout << "Error while loading mesh from file : " + filename << std::endl;
  }
}

/*
 *  @name Mesh  @fn Meshid)
 *  @brief  Destructor
 */
template<typename T>
Mesh<T>::~Mesh(void) {
}

/*
 *  @name Load
 *  @fn int Load(const std::string& filename)
 *  @brief  Load mesh from supported file :
 *            .obj, .ply, .tri
 *  @return -1 if error, 0 otherwise
 */
template<typename T>
int Mesh<T>::Load(const std::string& filename) {
  // Load data
  int err = -1;
  size_t pos = filename.rfind(".");
  if (pos != std::string::npos) {
    // Ensure empty containter
    vertex_.clear();
    normal_.clear();
    tex_coord_.clear();
    tri_.clear();
    vertex_con_.clear();
    //memset(bbox_, 0, sizeof(bbox_));
    std::string ext = filename.substr(pos + 1, filename.length());
    FileExt file_ext = this->HashExt(ext);
    switch (file_ext) {
      // OBJ
      case kObj: {
        err = this->LoadOBJ(filename);
      }
        break;
      // PLY
      case kPly: {
        err = this->LoadPLY(filename);
      }
        break;
      // Not supported yet
      case kUndef:
      default:  std::cout << "Error, unsported extension type : " << ext;
        std::cout << std::endl;
        err = -1;
        break;
    }
    if (!err) {
      this->PlaceToOrigin();
      this->BuildConnectivity();
    }
  }
  if (!err && !bbox_is_computed_) {
    this->ComputeBoundingBox();
  }
  return err;
}

/*
 *  @name Save
 *  @fn int Save(const std::string& filename)
 *  @brief  Save mesh to supported file format:
 *            .ply
 *  @return -1 if error, 0 otherwise
 */
template<typename T>
int Mesh<T>::Save(const std::string& filename) {
  // Load data
  int err = -1;
  size_t pos = filename.rfind(".");
  if (pos != std::string::npos) {
    std::string ext = filename.substr(pos + 1, filename.length());
    FileExt file_ext = this->HashExt(ext);
    switch (file_ext) {
        // PLY
      case kPly: {
        err = this->SavePLY(filename);
      }
        break;
        // OBJ
      case kObj: {
        err = this->SaveOBJ(filename);
      }
        break;

        break;
        // Undef
        case kUndef:
      default:  std::cout << "Error, unsported extension type : " << ext;
        std::cout << std::endl;
        err = -1;
        break;
    }
  }
  return err;
}

/*
 *  @name BuildConnectivity
 *  @fn void BuildConnectivity(void)
 *  @brief  Construct vertex connectivity (vertex connection) used later
 *          in normal computation
 */
template<typename T>
void Mesh<T>::BuildConnectivity(void) {
  // Init outter containter
  assert(vertex_.size() != 0 && tri_.size() != 0);
  vertex_con_.clear();
  vertex_con_ = std::vector<std::vector<int>>(vertex_.size(),
                                              std::vector<int>(0));
  // Loop over all triangle
  const int n_tri = static_cast<int>(tri_.size());
  for (int i = 0; i < n_tri; ++i) {
    int* tri_idx_ptr = &(tri_[i].x_);
    for (int e = 0; e < 3; ++e) {
      int idx_in = tri_idx_ptr[e];
      int idx_out_1 = tri_idx_ptr[(e + 1) % 3];
      int idx_out_2 = tri_idx_ptr[(e + 2) % 3];
      // Add to connectivity list
      vertex_con_[idx_in].push_back(idx_out_1);
      vertex_con_[idx_in].push_back(idx_out_2);
    }
  }
}

/*
 *  @name HashExt
 *  @fn FileExt HashExt(const std::string& ext)
 *  @brief  Provide the type of extension
 *  @param[in]  ext Name of the extension
 *  @return Hash of the given extension if know by the object
 */
template<typename T>
typename Mesh<T>::FileExt Mesh<T>::HashExt(const std::string& ext) {
  FileExt fext = kUndef;
  if (ext == "obj") {
    fext = kObj;
  } else if (ext == "ply") {
    fext = kPly;
  }
  return fext;
}

/*
 *  @name LoadOBJ
 *  @fn int LoadOBJ(const std::string& path)
 *  @brief  Load mesh from .obj file
 *  @param[in]  path  Path to obj file
 *  @return -1 if error, 0 otherwise
 */
template<typename T>
int Mesh<T>::LoadOBJ(const std::string& path) {
  int error = -1;
  std::ifstream stream(path, std::ios_base::in);
  if (stream.is_open()) {
    // Init bbox
    bbox_.min_.x_ = std::numeric_limits<T>::max();
    bbox_.max_.x_ = std::numeric_limits<T>::lowest();
    bbox_.min_.y_ = std::numeric_limits<T>::max();
    bbox_.max_.y_ = std::numeric_limits<T>::lowest();
    bbox_.min_.z_ = std::numeric_limits<T>::max();
    bbox_.max_.z_ = std::numeric_limits<T>::lowest();
    // Start to read
    std::string line, key;
    std::stringstream str_stream;
    Vertex vertex;
    Normal normal;
    TCoord tcoord;
    Triangle tri;
    while (!stream.eof()) {
      // Scan line
      std::getline(stream, line);
      if (!line.empty()) {
        str_stream.str(line);
        str_stream >> key;
        if (key == "v") {
          // Vertex
          str_stream >> vertex;
          vertex_.push_back(vertex);
          // Compute boundary box
          bbox_.min_.x_ = bbox_.min_.x_ < vertex.x_ ? bbox_.min_.x_ : vertex.x_;
          bbox_.max_.x_ = bbox_.max_.x_ > vertex.x_ ? bbox_.max_.x_ : vertex.x_;
          bbox_.min_.y_ = bbox_.min_.y_ < vertex.y_ ? bbox_.min_.y_ : vertex.y_;
          bbox_.max_.y_ = bbox_.max_.y_ > vertex.y_ ? bbox_.max_.y_ : vertex.y_;
          bbox_.min_.z_ = bbox_.min_.z_ < vertex.z_ ? bbox_.min_.z_ : vertex.z_;
          bbox_.max_.z_ = bbox_.max_.z_ > vertex.z_ ? bbox_.max_.z_ : vertex.z_;
        } else if (key == "vn") {
          // Normal
          str_stream >> normal;
          normal_.push_back(normal);
        } else if (key == "vt") {
          // Texture coordinate
          str_stream >> tcoord;
          tex_coord_.push_back(tcoord);
        } else if (key == "f") {
          // Faces
          str_stream >> tri;
          tri -= 1;
          tri_.push_back(tri);
        }
        str_stream.clear();
      }
    }
    bbox_.center_ = (bbox_.min_ + bbox_.max_) * T(0.5);
    bbox_is_computed_ = true;
    error = 0;
  }
  return error;
}

/*
 *  @name LoadPLY
 *  @fn int LoadPLY(const std::string& path)
 *  @brief  Load mesh from .obj file
 *  @param[in]  path  Path to ply file
 *  @return -1 if error, 0 otherwise
 */
template<typename T>
int Mesh<T>::LoadPLY(const std::string& path) {
  int error = -1;
  // Read ply file
  PlyFile* ply_file;
  int n_elem;
  char ** elem_list;
  int file_type;
  float version;
  /* open a PLY file for reading */
  ply_file = ply_open_for_reading(const_cast<char*>(path.c_str()),
                                  &n_elem,
                                  &elem_list,
                                  &file_type, &version);
  if (ply_file) {
    // list of property information for a vertex
    int type = sizeof(T) == 4 ? PLY_FLOAT : PLY_DOUBLE;
    PlyProperty vertex_prop[]  = {
      {"x", PLY_FLOAT, type, offsetof(PLYVertex<T>,vertex.x_), 0, 0, 0, 0},
      {"y", PLY_FLOAT, type, offsetof(PLYVertex<T>,vertex.y_), 0, 0, 0, 0},
      {"z", PLY_FLOAT, type, offsetof(PLYVertex<T>,vertex.z_), 0, 0, 0, 0},
      {"nx", PLY_FLOAT, type, offsetof(PLYVertex<T>,normal.x_), 0, 0, 0, 0},
      {"ny", PLY_FLOAT, type, offsetof(PLYVertex<T>,normal.y_), 0, 0, 0, 0},
      {"nz", PLY_FLOAT, type, offsetof(PLYVertex<T>,normal.z_), 0, 0, 0, 0}
    };
    PlyProperty face_prop[] = {
      {"vertex_indices", PLY_INT, PLY_INT, offsetof(PLYFace<T>, list_idx),
        1, PLY_INT, PLY_INT, offsetof(PLYFace<T>, n_vertex)},
      {"texcoord", PLY_FLOAT, type, offsetof(PLYFace<T>, list_tcoord),
        1, PLY_INT, PLY_INT, offsetof(PLYFace<T>, n_tcoord)}
    };
    
    // go through each kind of element that we learned is in the file
    // and read them
    for (int i = 0; i < n_elem; ++i) {
      // get the description of the first element
      int num_elems;
      int n_props;
      char* elem_name = elem_list[i];
      ply_get_element_description(ply_file,
                                  elem_name,
                                  &num_elems,
                                  &n_props);
      // if we're on vertex elements, read them in
      if (equal_strings ("vertex", elem_name)) {
        // Get position
        ply_get_property(ply_file, elem_name, &vertex_prop[0]);
        ply_get_property(ply_file, elem_name, &vertex_prop[1]);
        ply_get_property(ply_file, elem_name, &vertex_prop[2]);
        // Get normal
        ply_get_property(ply_file, elem_name, &vertex_prop[3]);
        ply_get_property(ply_file, elem_name, &vertex_prop[4]);
        ply_get_property(ply_file, elem_name, &vertex_prop[5]);
        
        // Init bbox
        bbox_.min_.x_ = std::numeric_limits<T>::max();
        bbox_.max_.x_ = std::numeric_limits<T>::lowest();
        bbox_.min_.y_ = std::numeric_limits<T>::max();
        bbox_.max_.y_ = std::numeric_limits<T>::lowest();
        bbox_.min_.z_ = std::numeric_limits<T>::max();
        bbox_.max_.z_ = std::numeric_limits<T>::lowest();
        
        /* grab all the vertex elements */
        for (int j = 0; j < num_elems; ++j) {
          PLYVertex<T> vert;
          ply_get_element(ply_file, reinterpret_cast<void*>(&vert));
          // Push data into structure
          vertex_.push_back(vert.vertex);
          // Compute boundary box
          bbox_.min_.x_ = (bbox_.min_.x_ < vert.vertex.x_ ?
                           bbox_.min_.x_ :
                           vert.vertex.x_);
          bbox_.max_.x_ = (bbox_.max_.x_ > vert.vertex.x_ ?
                           bbox_.max_.x_ :
                           vert.vertex.x_);
          bbox_.min_.y_ = (bbox_.min_.y_ < vert.vertex.y_ ?
                           bbox_.min_.y_ :
                           vert.vertex.y_);
          bbox_.max_.y_ = (bbox_.max_.y_ > vert.vertex.y_ ?
                           bbox_.max_.y_ :
                           vert.vertex.y_);
          bbox_.min_.z_ = (bbox_.min_.z_ < vert.vertex.z_ ?
                           bbox_.min_.z_ :
                           vert.vertex.z_);
          bbox_.max_.z_ = (bbox_.max_.z_ > vert.vertex.z_ ?
                           bbox_.max_.z_ :
                           vert.vertex.z_);
          // Normal if present
          if ((vert.normal.x_ != -1.0f) &&
              (vert.normal.y_ != -1.0f) &&
              (vert.normal.z_ != -1.0f)) {
            normal_.push_back(vert.normal);
          }
        }
        bbox_.center_ = (bbox_.min_ + bbox_.max_) * T(0.5);
        bbox_is_computed_ = true;
      }
      // if we're on face elements, read them in
      if (equal_strings ("face", elem_name)) {
        // set up for getting face elements
        ply_get_property (ply_file, elem_name, &face_prop[0]);
        ply_get_property (ply_file, elem_name, &face_prop[1]);
        // grab all the face elements
        for (int j = 0; j < num_elems; j++) {
          PLYFace<T> face;
          ply_get_element(ply_file, reinterpret_cast<void*>(&face));
          if (face.n_vertex != 3) {
            std::cout << "Support only triangle mesh !" << std::endl;
            return error;
          }
          if (face.list_idx != nullptr) {
            tri_.push_back(face.list_idx[0]);
          }
          if (face.list_tcoord != nullptr) {
            assert(face.n_tcoord == 6);
            tex_coord_.push_back(face.list_tcoord[0]);
            tex_coord_.push_back(face.list_tcoord[1]);
            tex_coord_.push_back(face.list_tcoord[2]);
          }
        }
      }
    }
    // close the PLY file
    ply_close (ply_file);
    // Shall we free elem_list ?
    free(elem_list);
    // Done
    error = 0;
  }
  return error;
}

/*
 *  @name SaveOBJ
 *  @fn int SaveOBJ(const std::string path) const
 *  @brief SAve mesh to a .obj file
 *  @param[in]  path  Path to .obj file
 *  @return -1 if error, 0 otherwise
 */
template<typename T>
int Mesh<T>::SaveOBJ(const std::string& path) {
  int err = -1;
  std::ofstream stream(path, std::ios_base::out);
  if (stream.is_open()) {
    // Header
    stream << "# Wavefront file written by FaceKit c++ library" << std::endl;

    // Vertex
    if (vertex_.size() > 0) {
      size_t n_vert = vertex_.size();
      for (size_t i = 0; i < n_vert; ++i) {
        const Vertex& v = vertex_[i];
        stream << "f " << v.x_ << " " << v.y_ << " " << v.z_ << std::endl;
      }
    }
    // Normal
    if (normal_.size() > 0) {
      size_t n_norm = normal_.size();
      for (size_t i = 0; i < n_norm; ++i) {
        const Normal& n = normal_[i];
        stream << "vn " << n.x_ << " " << n.y_ << " " << n.z_ << std::endl;
      }
    }
    // Texture coordinate
    if (tex_coord_.size() > 0) {
      size_t n_tcoord = tex_coord_.size();
      for (size_t i = 0; i < n_tcoord; ++i) {
        const TCoord& tc = tex_coord_[i];
        stream << "vt " << tc.x_ << " " << tc.y_ << std::endl;
      }
    }
    // Tri
    if (tri_.size() > 0) {
      size_t n_tri = tri_.size();
      for (size_t i = 0; i < n_tri; ++i) {
        const Triangle & tri = tri_[i];
        stream << "f " << tri.x_ << " " << tri.y_ << " " << tri.z_ << std::endl;
      }
    }
    // Done
    err = stream.good() ? 0 : -1;
    stream.close();
  }
  return err;
}

/*
 *  @name SavePLY
 *  @fn int SavePLY(const std::string& path) const
 *  @brief SAve mesh to a .ply file
 *  @param[in]  path  Path to .ply file
 *  @return -1 if error, 0 otherwise
 */
template<typename T>
int Mesh<T>::SavePLY(const std::string& path) const {
  int error = -1;
  // TODO: Need to be implemented !
  return error;
}

#pragma mark -
#pragma mark Usage

  /*
   *  @name ComputeVertexNormal
   *  @fn void ComputeVertexNormal(void)
   *  @brief  Compute normal for each vertex in the object
   */
template<typename T>
void Mesh<T>::ComputeVertexNormal(void) {
  // Loop over all vertex
  assert(vertex_con_.size() > 0);
  const int n_vert = static_cast<int>(vertex_.size());
  normal_.resize(n_vert, Mesh::Normal());
#if defined(__APPLE__)
  dispatch_apply(n_vert,
                 dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT,
                                           (unsigned long)NULL), ^(size_t v) {
#else
  #pragma omp parallel for
  for (int v = 0; v < n_vert; ++v) {
#endif
   // Loop over all connect vertex
   std::vector<int>& conn = vertex_con_[v];
   const Vertex& A = vertex_[v];
   const int n_conn = static_cast<int>(conn.size());
   Normal weighted_n;
   for (int j = 0; j < n_conn; j += 2) {
     const Vertex& B = vertex_[conn[j]];
     const Vertex& C = vertex_[conn[j+1]];
     // Define edges AB, AC
     Edge AB = B - A;
     Edge AC = C - A;
     // Compute surface's normal (triangle ABC)
     Normal n = AB ^ AC;
     n.Normalize();
     // Stack each face contribution and weight with angle
     AB.Normalize();
     AC.Normalize();
     const T angle = std::acos(AB * AC);
     weighted_n += (n * angle);
   }
   // normalize and set
   weighted_n.Normalize();
   normal_[v] = weighted_n;
 }
#if defined(__APPLE__)
  );
#endif
}

/*
 *  @name ComputeBoundingBox
 *  @fn void ComputeBoundingBox(void)
 *  @brief  Compute the bounding box of the mesh
 */
template<typename T>
void Mesh<T>::ComputeBoundingBox(void) {
 // Init bbox
 bbox_.min_.x_ = std::numeric_limits<T>::max();
 bbox_.max_.x_ = std::numeric_limits<T>::lowest();
 bbox_.min_.y_ = std::numeric_limits<T>::max();
 bbox_.max_.y_ = std::numeric_limits<T>::lowest();
 bbox_.min_.z_ = std::numeric_limits<T>::max();
 bbox_.max_.z_ = std::numeric_limits<T>::lowest();

 for (auto v_it = this->vertex_.begin(); v_it != this->vertex_.end(); ++v_it) {
   // Compute boundary box
   bbox_.min_.x_ = bbox_.min_.x_ < v_it->x_ ? bbox_.min_.x_ : v_it->x_;
   bbox_.max_.x_ = bbox_.max_.x_ > v_it->x_ ? bbox_.max_.x_ : v_it->x_;
   bbox_.min_.y_ = bbox_.min_.y_ < v_it->y_ ? bbox_.min_.y_ : v_it->y_;
   bbox_.max_.y_ = bbox_.max_.y_ > v_it->y_ ? bbox_.max_.y_ : v_it->y_;
   bbox_.min_.z_ = bbox_.min_.z_ < v_it->z_ ? bbox_.min_.z_ : v_it->z_;
   bbox_.max_.z_ = bbox_.max_.z_ > v_it->z_ ? bbox_.max_.z_ : v_it->z_;
 }
 bbox_.center_ = (bbox_.min_ + bbox_.max_) * T(0.5);
 bbox_is_computed_ = true;
}
                 
/*
 *  @name   PlaceToOrigin
 *  @fn     void PlaceToOrigin(void)
 *  @brief  Place mest to world origin (i.e. remove center of graviaty).
 */
template<typename T>
void Mesh<T>::PlaceToOrigin(void) {
  // Get center of gravity
  Vertex cog;
  for (const auto& v : this->vertex_) {
    cog += v;
  }
  cog /= static_cast<T>(this->vertex_.size());
  // Center all vertex
  for (auto& v : this->vertex_) {
    v -= cog;
  }
  // Update bbox as well
  if (bbox_is_computed_) {
    bbox_.min_ -= cog;
    bbox_.max_ -= cog;
    bbox_.center_ -= cog;
  }
}

#pragma mark -
#pragma mark Declaration

/** Float Mesh */
template class Mesh<float>;
/** Double Mesh */
template class Mesh<double>;


}  // namespace FaceKit
