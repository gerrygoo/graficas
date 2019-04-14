#pragma once

#include <memory>
#include <string>
#include <vector>

#include "scene.h"

class scene_manager {
  public:
    static void start(int argc,
                      char* argv[],
                      const std::string& name,
                      int width,
                      int height);

    static void next();
    static void prev();

  private:
    static void initialize();
    static void mainLoop(GLFWwindow* window);
    static void resize(GLFWwindow* window, int width, int height);
    static void error(int error, const char* description);

    static void keys(GLFWwindow* window,
                     int key,
                     int scancode,
                     int action,
                     int mods);
    static void keysDown(int key);
    static void keysUp(int key);
    static void passiveMotion(GLFWwindow* window, double x, double y);

    static std::vector<std::unique_ptr<scene>> sceneList;
    static int currentScene;
};
