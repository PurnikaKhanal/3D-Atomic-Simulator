#ifndef GRAPHICS_TRANSFORMATIONS_H
#define GRAPHICS_TRANSFORMATIONS_H

#include <cmath>

// ==================== DATA STRUCTURES ====================

// 3D Vector
struct Vector3 {
    float x, y, z;
};

// 4D Vector (Homogeneous Coordinates)
struct Vector4 {
    float x, y, z, w;
};

// 4x4 Matrix
typedef float Matrix4x4[4][4];

// Camera Structure
struct Camera {
    Vector3 eye;      // Camera position
    Vector3 target;   // Look-at point
    Vector3 up;       // Up vector
};

// ==================== HELPER FUNCTIONS ====================

// Initialize identity matrix
void createIdentityMatrix(Matrix4x4& mat);

// Multiply two 4x4 matrices: result = a * b
void multiplyMatrices(const Matrix4x4& a, const Matrix4x4& b, Matrix4x4& result);

// Normalize a 3D vector
Vector3 normalize(const Vector3& v);

// Cross product of two 3D vectors
Vector3 cross(const Vector3& a, const Vector3& b);

// Dot product of two 3D vectors
float dot(const Vector3& a, const Vector3& b);

// ==================== 3D TRANSFORMATION FUNCTIONS ====================

// Create Translation Matrix
void createTranslationMatrix(float tx, float ty, float tz, Matrix4x4& mat);

// Create Rotation Matrix around Z-axis
void createRotationMatrixZ(float angleInRadians, Matrix4x4& mat);

// Create Scale Matrix
void createScaleMatrix(float sx, float sy, float sz, Matrix4x4& mat);

// Multiply a 4x4 matrix by a 4D vector: result = mat * vec
Vector4 multiplyMatrixVector(const Matrix4x4& mat, const Vector4& vec);

// ==================== COMPOSITE TRANSFORMATION ====================

// Create composite transformation matrix: M = T * R * S
void createCompositeMatrix(const Vector3& scale, float rotationAngle, const Vector3& translation, Matrix4x4& result);

// ==================== PROJECTION AND VIEWING TRANSFORMATIONS ====================

// Create View Matrix from camera parameters
void createViewMatrix(const Camera& cam, Matrix4x4& mat);

// Create Perspective Projection Matrix
void createPerspectiveMatrix(float fov, float aspectRatio, float nearPlane, float farPlane, Matrix4x4& mat);

// Viewport Transform: Convert NDC coordinates to screen pixel coordinates
void viewportTransform(float ndcX, float ndcY, int screenWidth, int screenHeight, 
                       int& pixelX, int& pixelY);

#endif // GRAPHICS_TRANSFORMATIONS_H
