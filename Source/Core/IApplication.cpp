//
// Created by 13973 on 25-3-15.
//

# include "PrecompiledHeader.h"
#include "IApplication.h"

int IApplication::Run() {
    if (!glfwInit()) {
        return -1;
    }

    // 窗口设置
    SetWindowHints();

    // 创建窗口
    GLFWwindow *glfwWindow = glfwCreateWindow(1024, 1024, "", nullptr, nullptr);
    if (!glfwWindow) {
        glfwTerminate();
        return -1;
    }

    // 创建上下文
    glfwMakeContextCurrent(glfwWindow);

    // 初始化glad
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        glfwTerminate();
        return -1;
    }

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
