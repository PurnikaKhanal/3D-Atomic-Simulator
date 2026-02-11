#include "graphics_transformations.h"
#include <iostream>

// ==================== HELPER FUNCTIONS ====================

// Initialize identity matrix
void createIdentityMatrix(Matrix4x4& mat) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

// Multiply two 4x4 matrices: result = a * b
void multiplyMatrices(const Matrix4x4& a, const Matrix4x4& b, Matrix4x4& result) {
    Matrix4x4 temp;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp[i][j] = 0.0f;
            for (int k = 0; k < 4; k++) {
                temp[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    // Copy result back
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i][j] = temp[i][j];
        }
    }
}

// Normalize a 3D vector
Vector3 normalize(const Vector3& v) {
    float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length > 0.0f) {
        return {v.x / length, v.y / length, v.z / length};
    }
    return v;
}

// Cross product of two 3D vectors
Vector3 cross(const Vector3& a, const Vector3& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

// Dot product of two 3D vectors
float dot(const Vector3& a, const Vector3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// ==================== 3D TRANSFORMATION FUNCTIONS ====================

// Create Translation Matrix
// T = [[1, 0, 0, tx],
//      [0, 1, 0, ty],
//      [0, 0, 1, tz],
//      [0, 0, 0,  1]]

void createTranslationMatrix(float tx, float ty, float tz, Matrix4x4& mat) {
    createIdentityMatrix(mat);
    mat[0][3] = tx;
    mat[1][3] = ty;
    mat[2][3] = tz;
}

// Create Rotation Matrix around Z-axis
// Rz (θ) = [[cos(θ), -sin(θ), 0, 0],
//          [sin(θ),  cos(θ), 0, 0],
//          [0,       0,      1, 0],
//          [0,       0,      0, 1]]

void createRotationMatrixZ(float angleInRadians, Matrix4x4& mat) {
    createIdentityMatrix(mat);
    
    float cosA = std::cos(angleInRadians);
    float sinA = std::sin(angleInRadians);
    
    mat[0][0] = cosA;
    mat[0][1] = -sinA;
    mat[1][0] = sinA;
    mat[1][1] = cosA;
}

// Create Scale Matrix
// S = [[sx,  0,  0, 0],
//      [0,  sy,  0, 0],
//      [0,  0,  sz, 0],
//      [0,  0,  0,  1]]

void createScaleMatrix(float sx, float sy, float sz, Matrix4x4& mat) {
    createIdentityMatrix(mat);
    mat[0][0] = sx;
    mat[1][1] = sy;
    mat[2][2] = sz;
}

// Multiply a 4x4 matrix by a 4D vector: result = mat * vec
Vector4 multiplyMatrixVector(const Matrix4x4& mat, const Vector4& vec) {
    Vector4 result;
    result.x = mat[0][0] * vec.x + mat[0][1] * vec.y + mat[0][2] * vec.z + mat[0][3] * vec.w;
    result.y = mat[1][0] * vec.x + mat[1][1] * vec.y + mat[1][2] * vec.z + mat[1][3] * vec.w;
    result.z = mat[2][0] * vec.x + mat[2][1] * vec.y + mat[2][2] * vec.z + mat[2][3] * vec.w;
    result.w = mat[3][0] * vec.x + mat[3][1] * vec.y + mat[3][2] * vec.z + mat[3][3] * vec.w;
    return result;
}

// ==================== COMPOSITE TRANSFORMATION ====================

// Create composite transformation matrix: M = T * R * S
// (translation * rotation * scale)

void createCompositeMatrix(const Vector3& scale, float rotationAngle, const Vector3& translation, Matrix4x4& result) {
    // Create individual matrices
    Matrix4x4 S, R, T, TR;
    
    createScaleMatrix(scale.x, scale.y, scale.z, S);
    createRotationMatrixZ(rotationAngle, R);
    createTranslationMatrix(translation.x, translation.y, translation.z, T);
    
    // Composite: M = T * R * S
    multiplyMatrices(T, R, TR);
    multiplyMatrices(TR, S, result);
}

// ==================== PROJECTION AND VIEWING TRANSFORMATIONS ====================

// Create View Matrix from camera parameters
// Uses the camera's eye point, target point, and up vector to construct
// a view matrix that transforms from world space to view space
void createViewMatrix(const Camera& cam, Matrix4x4& mat) {

    // 1. Calculate forward (negative Z) vector: n = normalize(eye - target)
    
    Vector3 n = normalize({
        cam.eye.x - cam.target.x,
        cam.eye.y - cam.target.y,
        cam.eye.z - cam.target.z
    });
    
    // 2. Calculate right vector: u = normalize(up × n)
    Vector3 u = normalize(cross(cam.up, n));
    
    // 3. Calculate true up vector: v = n × u
    Vector3 v = cross(n, u);
    
    // 4. Construct view matrix
    //[ux  uy  uz  -u·eye]
    // [vx  vy  vz  -v·eye]
    // [nx  ny  nz  -n·eye]
    // [0   0   0      1   ]

    createIdentityMatrix(mat);
    
    mat[0][0] = u.x;
    mat[0][1] = u.y;
    mat[0][2] = u.z;
    mat[0][3] = -dot(u, cam.eye);
    
    mat[1][0] = v.x;
    mat[1][1] = v.y;
    mat[1][2] = v.z;
    mat[1][3] = -dot(v, cam.eye);
    
    mat[2][0] = n.x;
    mat[2][1] = n.y;
    mat[2][2] = n.z;
    mat[2][3] = -dot(n, cam.eye);
}

// Create Perspective Projection Matrix
// Standard OpenGL perspective projection matrix
// fov: field of view in radians
// aspectRatio: width / height
// nearPlane: distance to near clipping plane
// farPlane: distance to far clipping plane
void createPerspectiveMatrix(float fov, float aspectRatio, float nearPlane, float farPlane, Matrix4x4& mat) {
    createIdentityMatrix(mat);
    
    float f = 1.0f / std::tan(fov / 2.0f);
    float rangeInv = 1.0f / (nearPlane - farPlane);
    
    mat[0][0] = f / aspectRatio;
    mat[1][1] = f;
    mat[2][2] = (nearPlane + farPlane) * rangeInv;
    mat[2][3] = 2.0f * nearPlane * farPlane * rangeInv;
    mat[3][2] = -1.0f;
    mat[3][3] = 0.0f;
}

// Viewport Transform: Convert NDC coordinates to screen pixel coordinates
// NDC range:[-1, 1] for both x and y
// Screen range: [0, screenWidth] and [0, screenHeight]
// Note: Y-axis is inverted (NDC's -1 maps to screen height, NDC's 1 maps to 0)
void viewportTransform(float ndcX, float ndcY, int screenWidth, int screenHeight, int& pixelX, int& pixelY) {
    // Map from NDC[-1, 1] to screen [0, width/height]
    pixelX = (int)((ndcX + 1.0f) * 0.5f * (screenWidth - 1));
    pixelY = (int)((1.0f - ndcY) * 0.5f * (screenHeight - 1));
    
    // Clamp to screen bounds
    if (pixelX < 0) pixelX = 0;
    if (pixelX >= screenWidth) pixelX = screenWidth - 1;
    if (pixelY < 0) pixelY = 0;
    if (pixelY >= screenHeight) pixelY = screenHeight - 1;
}
