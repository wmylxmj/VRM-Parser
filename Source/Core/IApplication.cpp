//
// Created by 13973 on 25-3-15.
//

# include "PrecompiledHeader.h"
#include "IApplication.h"

int IApplication::Run() {
    if (!glfwInit()) {
        return -1;
    }

    // 设置背景可透明
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    GLFWwindow *glfwWindow = glfwCreateWindow(1024, 1024, "", nullptr, nullptr);
    if (!glfwWindow) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(glfwWindow);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        glfwTerminate();
        return -1;
    }

    // 设置输入模式
    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // 初始化场景
    OnInit();

    while(!glfwWindowShouldClose(glfwWindow)) {
        // 更新逻辑帧
        OnUpdate();
        // 渲染
        OnRender();
        // 交换帧缓冲
        glfwSwapBuffers(glfwWindow);
        // 处理窗口事件
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
