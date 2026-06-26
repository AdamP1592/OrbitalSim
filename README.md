# OrbitalSim

## Project Description:
A particle-mesh orbital simulation written in C++ solely utilizing a library for fftw. This is paired with a general-purpose API with c-bindings to be used in a python library

## Sim:

## Scheduler:

##  ParticleStorage:

## Math

**Description:** A library for universal math objects

### Vec3: 
- A type-agnostic template implementation of vec3
- A series of type-specific functions for math.
- Overrides most standard operations(+ , - , / , *, comparisons) across both vector<T> and vector<T> as well as vector<T> and T operations
- Provides a math namespace to perform math operations with vec3s for helper functions (max, min, floor, ceil, roughEquals, is_zero)
- overrides << operator for logging
#### Example usage:
```cpp
    // multiple constructors
    
    // list-style constructor
    vec3<float> v1 = {0.0f, 1.5f, 1.1f};
    // scalar
    vec3<float> v2(0.5f);

    // empty does not initialize
    vec3<float> v3;

    // some example operations
    v1 /= 0.3f;
    v1 -= v2;
    v3 = v1 - v2;

    // example internal helper functions
    bool isLess = v1.allLessThan(0.3f);
    bool isGreater = v2.allGreaterThan(0.3f);
    
    // example math namespace helper functions
    vec3<float> maxOfAllValues = math::max(v1, v2);
    vec3<float> minOfAllValues = math::min(v1, v3);

    //type specific helpers:
    vec3<float> rounded =  math::round(v1);
    vec3<float> abs = math::abs(v1);
    bool isEqualImplicitEps = math::roughEquals(v1, v2);
    bool isEqualExplicit = math::roughEquals(v1, v2, 1e-5);

    // print direct to console
    std::cout << v1 << std::endl;
```
### Functionality:


## DataStructures:
