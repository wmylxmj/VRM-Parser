//
// Created by 13973 on 25-3-15.
//

# include "PrecompiledHeader.h"
#include "IApplication.h"

int IApplication::Run() {
    if (!glfwInit()) {
        return -1;
    }

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        glfwTerminate();
        return -1;
    }

    OnInit();

    return 0;
}
