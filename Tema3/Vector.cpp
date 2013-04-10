#include "Vector3D.h"

std::ostream& operator<<(std::ostream& out, Vector3D right){
		out << "(" << right.x << ", " << right.y << ", " << right.z << ")";
		return out;
}