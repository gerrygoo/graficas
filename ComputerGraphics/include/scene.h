#pragma once

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#define GLFW_DLL
#include <GLFW/glfw3.h>

// Scene is an abstract class.
class scene {
  public:
    virtual ~scene() {}

    virtual void init() = 0;
    virtual void awake() = 0;
    virtual void sleep() = 0;
    virtual void reset() = 0;
    virtual void mainLoop() = 0;
    virtual void resize(int width, int height) = 0;
    virtual void keysDown(int key) = 0;
    virtual void keysUp(int key) = 0;
    virtual void passiveMotion(float x, float y) = 0;
};
