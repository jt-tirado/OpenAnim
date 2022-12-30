%module openanim
%{
#include "angle.h"
#include "attribute.h"
#include "curve.h"
#include "dagobject.h"
#include "debug.h"
#include "euler.h"
#include "frame.h"
#include "keyframe.h"
#include "matrix4.h"
#include "node.h"
#include "object.h"
#include "path.h"
#include "quaternion.h"
#include "space.h"
#include "string.h"
#include "time.h"
#include "vector3.h"
%}

%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"
%include "std_unordered_map.i"

%template(IntVector) std::vector<int>;
%template(DoubleVector) std::vector<double>;
%template(FloatVector) std::vector<float>;
%template(StringVector) std::vector<std::string>;
%template(ConstCharVector) std::vector<const char*>;
%template(unordered_map_string_string) std::unordered_map<std::string, std::string>;
%template(vector_vector_double) std::vector<std::vector<double>>;
%template(vector_vector_float) std::vector<std::vector<float>>;

%exception {
  try {
    $action
  } catch (const std::runtime_error& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
}

%include "angle.h"
%include "attribute.h"
%include "curve.h"
%include "dagobject.h"
%include "debug.h"
%include "euler.h"
%include "frame.h"
%include "keyframe.h"
%include "matrix4.h"
%include "node.h"
%include "object.h"
%include "path.h"
%include "quaternion.h"
%include "space.h"
%include "string.h"
%include "time.h"
%include "vector3.h"
