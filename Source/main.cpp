#include <iostream>
#include <memory>
#include <cassert>
#include <stack>

#include <cmrc/cmrc.hpp>
CMRC_DECLARE(shaderRC);

#include "Core/Camera.h"
#include "Core/PrecompiledHeader.h"
#include "Core/IApplication.h"
#include "Model/VrmModel.h"
#include "Core//Shader.h"
#include "Model/ModelUtilities.h"
#include "InteractiveGeometry/JointBall.h"

class MainApp final : public IApplication {

public:
    GLFWwindow* window;
    Camera camera;
    GLuint programID;
    std::unique_ptr<Model> pModel;
    std::unique_ptr<JointBall> pJointBall;
    GLuint vao, vbo, ebo;
    GLuint ubo;

    bool LoopCondition() override { return !glfwWindowShouldClose(window); }
    void OnInit() override;
    void OnUpdate() override;
    void OnRender() override;
};


void MainApp::OnInit() {
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
    window = glfwCreateWindow(1024, 1024, "Vrm Parser", nullptr, nullptr);
    MakeContextCurrent(window);

    glEnable(GL_DEPTH_TEST);
    // 导入模型
    pModel = std::make_unique<VrmModel>(R"(E:\vrm\20220331_1455\20220331_1455\base body\black cat base body v3.5.0.vrm)");
    pJointBall = std::make_unique<JointBall>();


    // 模型上传
    GL_CHECK_ERRORS(SetupModelToGL(*pModel, vao, vbo, ebo));

    pModel->bones[pModel->boneNameIndexMapping["J_Bip_R_LowerLeg"]].transformation =
        pModel->bones[pModel->boneNameIndexMapping["J_Bip_R_LowerLeg"]].bindingPoseTransformation *
        glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
    std::vector<glm::mat4> finalTransformations = CalcBonesFinalTransformations(*pModel);
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)*200, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo); // 关联 UBO 到绑定点 0

    for (unsigned int i = 0; i < pModel->bones.size(); i++) {
        std::cout << pModel->bones[i].name << std::endl;
        glm::mat4 transformation = finalTransformations[i];
        std::cout << transformation[0][0] << transformation[1][0] << transformation[2][0]<< transformation[3][0] << std::endl;
        std::cout << transformation[0][1] << transformation[1][1] << transformation[2][1]<< transformation[3][1] << std::endl;
        std::cout << transformation[0][2] << transformation[1][2] << transformation[2][2]<< transformation[3][2] << std::endl;
        std::cout << transformation[0][3] << transformation[1][3] << transformation[2][3]<< transformation[3][3] << std::endl;
    }


    float xMin = FLT_MAX;
    float xMax = -FLT_MAX;
    float yMin = FLT_MAX;
    float yMax = -FLT_MAX;
    float zMin = FLT_MAX;
    float zMax = -FLT_MAX;
    for (auto vertex: pModel->vertices) {
        if (vertex.position.x < xMin) xMin = vertex.position.x;
        if (vertex.position.x > xMax) xMax = vertex.position.x;
        if (vertex.position.y < yMin) yMin = vertex.position.y;
        if (vertex.position.y > yMax) yMax = vertex.position.y;
        if (vertex.position.z < zMin) zMin = vertex.position.z;
        if (vertex.position.z > zMax) zMax = vertex.position.z;
    }

    camera.eyePosition = glm::vec3(0.5*(xMax+xMin), 0.5*(yMin+yMax), zMax+std::max(zMax-zMin, std::max(xMax-xMin, yMax-yMin)));
    camera.zFar = 10000;

    const auto fileSystem = cmrc::shaderRC::get_filesystem();
    const auto vsFile = fileSystem.open(R"(Source/Shaders/Shader.vsh)");
    GLuint vsh = CompileShader(vsFile.begin(), GL_VERTEX_SHADER);
    const auto fsFile = fileSystem.open(R"(Source/Shaders/Shader.fsh)");
    GLuint fsh = CompileShader(fsFile.begin(), GL_FRAGMENT_SHADER);
    programID = LinkProgram({vsh, fsh});

    GL_CHECK_ERRORS();
}

void MainApp::OnUpdate() {
    int windowWidth, windowHeight;
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

    camera.aspect = (float)windowWidth / (float)windowHeight;

    GL_CHECK_ERRORS(glUseProgram(programID));
    SetShaderUniformMat4(programID, "matModel", glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    SetShaderUniformMat4(programID, "matView", camera.GetCameraMatrix());
    SetShaderUniformMat4(programID, "matProjection", camera.GetPerspectiveMatrix());

    std::vector<glm::mat4> finalTransformations = CalcBonesFinalTransformations(*pModel);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4)*finalTransformations.size(), finalTransformations.data());
}

void MainApp::OnRender() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const Model model = *pModel;
    GL_CHECK_ERRORS(glBindVertexArray(vao));
    GL_CHECK_ERRORS(glDrawElements(GL_TRIANGLES, model.indices.size(), GL_UNSIGNED_INT, nullptr));
    glBindVertexArray(0);

    glfwSwapBuffers(window);
}

int main() {
    MainApp app;
    return app.Run();
}
