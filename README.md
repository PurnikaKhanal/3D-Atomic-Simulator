# 3D Atomic Simulator
## Perspective Projection & Z-Buffer Implementation

[![Language](https://img.shields.io/badge/language-C%2FC%2B%2B-blue.svg)](https://isocpp.org/)
[![Graphics](https://img.shields.io/badge/graphics-3D%20Rendering-green.svg)](https://www.khronos.org/opengl/)
[![Branch](https://img.shields.io/badge/branch-PerspectiveProjection__zBuffer-orange.svg)](https://github.com/PurnikaKhanal/3D-Atomic-Simulator/tree/PerspectivceProjection_zBuffer)

---

## Overview

This branch implements fundamental 3D computer graphics algorithms for realistic atomic structure visualization. The codebase demonstrates the mathematical and computational principles underlying modern 3D rendering pipelines, specifically focusing on perspective transformation and hidden surface removal techniques.

### Repository Structure

```
PerspectiveProjection_zBuffer/
├── perspectiveProjection.c    # Core perspective transformation implementation
└── pers&zbuffer.cpp           # Integrated perspective projection with depth buffering
```

---

## Technical Implementation

### 1. Perspective Projection

Perspective projection is a geometric transformation that maps three-dimensional points onto a two-dimensional plane while preserving the illusion of depth. This implementation simulates the pinhole camera model used in computer graphics.

#### Mathematical Framework

The perspective transformation follows the principle of similar triangles:

```
x' = (f × x) / z
y' = (f × y) / z
```

Where:
- `(x, y, z)` represents the 3D world coordinates
- `(x', y')` represents the 2D screen coordinates
- `f` is the focal length (distance from camera to projection plane)
- `z` is the depth coordinate

#### Transformation Pipeline

The complete projection process involves multiple coordinate system transformations:

1. **World Space → Camera Space**: Object coordinates relative to camera position
2. **Camera Space → Clip Space**: Perspective projection matrix application
3. **Clip Space → NDC**: Perspective division by homogeneous w-coordinate
4. **NDC → Screen Space**: Viewport transformation to pixel coordinates

#### Key Features

- Depth-aware coordinate transformation
- Field of view (FOV) control for camera perspective
- Aspect ratio correction for non-square viewports
- Near and far plane clipping

### 2. Z-Buffer Algorithm

The Z-buffer (depth buffer) algorithm provides efficient hidden surface determination through per-pixel depth testing. This technique has become the industry standard for real-time 3D graphics rendering.

#### Algorithm Overview

The Z-buffer maintains a two-dimensional array storing the depth value of the closest surface at each pixel location. During rasterization, the algorithm performs the following operations:

```
For each fragment (x, y) with depth z:
    if (z < Z_BUFFER[x][y]):
        FRAME_BUFFER[x][y] = fragment_color
        Z_BUFFER[x][y] = z
    end if
end for
```

#### Implementation Details

**Initialization**
```c
// Z-buffer initialization to maximum depth
for (int i = 0; i < width; i++)
    for (int j = 0; j < height; j++)
        z_buffer[i][j] = INFINITY;
```

**Depth Testing**
- Comparison operation: `LESS` (closer objects occlude distant ones)
- Depth value interpolation across polygon surfaces
- Sub-pixel precision for accurate depth determination

#### Advantages

| Feature | Benefit |
|---------|---------|
| Order Independence | No requirement for depth-sorted rendering |
| Simplicity | Straightforward implementation and debugging |
| Hardware Acceleration | Native GPU support in modern graphics APIs |
| Efficiency | O(n) complexity for n fragments |

---

## Build Instructions

### System Requirements

- **Compiler**: GCC 7.0+ or Clang 6.0+ (C11 standard)
- **C++ Compiler**: G++ 7.0+ or Clang++ 6.0+ (C++11 standard)
- **Graphics Library**: OpenGL 3.3+ or equivalent
- **Platform**: Linux, macOS, or Windows (with MinGW)

### Compilation

#### perspectiveProjection.c

```bash
# Standard compilation
gcc -std=c11 -O2 perspectiveProjection.c -o perspectiveProjection -lm

# With OpenGL (if required)
gcc -std=c11 -O2 perspectiveProjection.c -o perspectiveProjection \
    -lGL -lGLU -lglut -lm

# Execution
./perspectiveProjection
```

#### pers&zbuffer.cpp

```bash
# Standard compilation
g++ -std=c++11 -O2 "pers&zbuffer.cpp" -o perspective_zbuffer -lm

# With OpenGL (if required)
g++ -std=c++11 -O2 "pers&zbuffer.cpp" -o perspective_zbuffer \
    -lGL -lGLU -lglut -lm

# Execution
./perspective_zbuffer
```

### Compiler Flags Explained

- `-std=c11` / `-std=c++11`: Enforce modern language standards
- `-O2`: Enable optimization level 2 for better performance
- `-lm`: Link mathematics library
- `-lGL -lGLU -lglut`: Link OpenGL libraries (platform-dependent)

---

## Technical Specifications

### Viewing Frustum Parameters

The viewing frustum defines the visible 3D region:

| Parameter | Description | Typical Range |
|-----------|-------------|---------------|
| **Near Plane** | Closest visible distance | 0.1 - 10.0 units |
| **Far Plane** | Farthest visible distance | 100.0 - 10000.0 units |
| **FOV (Vertical)** | Vertical field of view angle | 45° - 90° |
| **Aspect Ratio** | Width/Height of viewport | 4:3, 16:9, 16:10 |

### Depth Buffer Precision

#### Non-Linear Depth Distribution

Z-buffer precision follows a hyperbolic distribution:

```
Precision ∝ 1 / (z × (far - near))
```

This results in:
- **Near objects**: High precision, minimal z-fighting
- **Far objects**: Lower precision, potential artifacts

#### Optimization Strategies

1. **Near Plane Positioning**: Maximize near plane distance
2. **Depth Range Ratio**: Keep `far/near < 1000` when possible
3. **Reversed Z-Buffer**: Use reversed depth testing for improved precision
4. **Logarithmic Depth**: Apply logarithmic depth encoding for extreme ranges

---

## Application Domain

### Atomic Structure Visualization

This implementation enables scientific visualization of molecular and atomic structures with:

#### Rendering Capabilities

- **Spatial Relationships**: Accurate depth perception for molecular geometry
- **Occlusion Handling**: Proper visibility determination for overlapping atoms
- **Interactive Navigation**: Real-time rotation, translation, and zoom
- **Scale Representation**: Perspective cues for atomic size comparison

#### Use Cases

- Protein structure analysis
- Crystal lattice visualization
- Chemical bond representation
- Molecular dynamics simulation display

---

## Performance Considerations

### Computational Complexity

| Operation | Time Complexity | Space Complexity |
|-----------|-----------------|------------------|
| Perspective Transform | O(n) | O(1) |
| Z-Buffer Update | O(w × h) | O(w × h) |
| Fragment Processing | O(f) | O(1) |

Where:
- `n` = number of vertices
- `w × h` = screen resolution
- `f` = number of fragments

### Optimization Techniques

1. **Early Depth Testing**: Reject fragments before expensive shading
2. **Hierarchical Z-Buffer**: Multi-resolution depth testing
3. **Occlusion Culling**: Skip invisible objects entirely
4. **Frustum Culling**: Eliminate out-of-view geometry

---

## Coordinate System Transformations

### Complete Transformation Matrix

The full perspective projection matrix in homogeneous coordinates:

```
⎡  f/aspect    0         0              0      ⎤
⎢     0        f         0              0      ⎥
⎢     0        0    -(f+n)/(f-n)  -2fn/(f-n) ⎥
⎣     0        0        -1              0      ⎦
```

Where:
- `f` = far plane distance
- `n` = near plane distance
- `aspect` = viewport aspect ratio

---

## Known Issues & Limitations

### Current Limitations

1. **Z-Fighting**: May occur with coplanar or near-coplanar surfaces
2. **Depth Precision**: Limited by floating-point representation (typically 24-bit)
3. **Transparency**: Standard Z-buffer incompatible with alpha blending (requires order-dependent rendering)
4. **Far Plane Clipping**: Objects beyond far plane are culled entirely

### Mitigation Strategies

- Implement polygon offset for coplanar geometry
- Use 32-bit depth buffers where available
- Consider depth peeling for transparent surfaces
- Apply logarithmic depth buffer for extended range

---

## Future Development Roadmap

### Planned Enhancements

- [ ] **Reversed Z-Buffer**: Implement reversed depth testing (1.0 to 0.0)
- [ ] **Orthographic Projection**: Add parallel projection mode for technical visualization
- [ ] **Early Z-Culling**: Optimize fragment rejection pipeline
- [ ] **Frustum Culling**: Implement view volume intersection testing
- [ ] **MSAA Support**: Multi-sample anti-aliasing for edge quality
- [ ] **Order-Independent Transparency**: Depth peeling or A-buffer implementation
- [ ] **Shader Integration**: Modern programmable pipeline support
- [ ] **Performance Profiling**: Comprehensive benchmarking suite

---

## Academic References

### Foundational Works

1. **Catmull, E.** (1974). "A Subdivision Algorithm for Computer Display of Curved Surfaces." *PhD Thesis, University of Utah*.

2. **Foley, J. D., van Dam, A., Feiner, S. K., & Hughes, J. F.** (1995). *Computer Graphics: Principles and Practice* (2nd ed.). Addison-Wesley.

3. **Akenine-Möller, T., Haines, E., & Hoffman, N.** (2018). *Real-Time Rendering* (4th ed.). CRC Press.

### Technical Standards

- **OpenGL Specification**: Khronos Group, OpenGL 4.6 Core Profile
- **IEEE 754**: Standard for Floating-Point Arithmetic
- **Computer Graphics Principles**: ACM SIGGRAPH educational resources

---

## Contributing Guidelines

### Code Quality Standards

When contributing to this branch, please adhere to:

1. **Documentation**: Comprehensive inline comments for mathematical operations
2. **Variable Naming**: Descriptive identifiers following standard notation
3. **Matrix Operations**: Clear indexing and transformation order
4. **Testing**: Validation across multiple viewing angles and depth ranges
5. **Performance**: Profile code for computational bottlenecks
6. **Precision**: Verify numerical stability with edge cases

### Testing Requirements

- Test with overlapping geometry (z-fighting scenarios)
- Validate with extreme near/far plane ratios
- Verify perspective correctness at viewport edges
- Profile with varying atomic structure complexities

---

## License

Refer to the main repository for licensing information.

---

## Contact & Support

**Repository**: [3D-Atomic-Simulator](https://github.com/PurnikaKhanal/3D-Atomic-Simulator)  
**Branch**: [PerspectiveProjection_zBuffer](https://github.com/PurnikaKhanal/3D-Atomic-Simulator/tree/PerspectivceProjection_zBuffer)

---

*This implementation serves as an educational reference for 3D graphics programming and scientific visualization techniques.*
