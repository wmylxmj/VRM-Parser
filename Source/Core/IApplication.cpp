//
// Created by 13973 on 25-3-15.
//

#include "PrecompiledHeader.h"
#include "IApplication.h"

int IApplication::MakeContextCurrent(GLFWwindow *window) {
    // 创建当前上下文
    glfwMakeContextCurrent(window);
    // 从上下文中加载函数指针
    return gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
}

int IApplication::Run() {
    // 初始化glfw
    if (!glfwInit()) {
        return -1;
    }

    // 初始化场景
    OnInit();

    while(LoopCondition()) {
        // 更新逻辑帧
        OnUpdate();
        // 渲染
        OnRender();
        // 处理窗口事件
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
