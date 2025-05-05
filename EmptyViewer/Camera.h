#pragma once
#include <glm/glm.hpp>
#include <random>

class Ray;

class Camera {
public:
    //Center
    glm::vec3 e;
    //Forward
    glm::vec3 w;
    //Up
    glm::vec3 v;
    //Right
    glm::vec3 u;


    float fov;
    //ratio of Height and Width
    float aspectRatio;
    // perspective Camera Value
    float nearPlane;
    float farPlane;

    float l = -0.1f, r = 0.1f, b = -0.1f, t = 0.1f, d = 0.1f;


    Camera(glm::vec3 startPosition, glm::vec3 startUp, glm::vec3 startRight, glm::vec3 startForward, float startFov, float startAspectRatio, float startNearPlane, float startFarPlane)
        : e(startPosition), v(startUp), u(startRight), w(startForward), fov(startFov), aspectRatio(startAspectRatio), nearPlane(startNearPlane), farPlane(startFarPlane) {

    }

    //Create Ray
    Ray* getRay(int ix, int iy);

    //Create Ray
    Ray* getAntialiasingRay(int ix, int iy);

private:
    // Global random number generator
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution;
};