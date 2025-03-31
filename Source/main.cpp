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
#include "Model/ModelDriver.h"

class MainApp final : public IApplication {

public:
    GLFWwindow* window;
    Camera camera;
    GLuint programID;
    std::unique_ptr<ModelDriver> pModelDriver;
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
    // 启用混合
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 导入模型
    pModelDriver = std::make_unique<ModelDriver>(std::make_shared<VrmModel>(R"(E:\vrm\20220331_1455\20220331_1455\base body\black cat base body v3.5.0.vrm)"));
    pJointBall = std::make_unique<JointBall>();


    // 模型上传
    GL_CHECK_ERRORS(SetupModelToGL(*pModelDriver->pModel, vao, vbo, ebo));

    pModelDriver->pModel->bones[pModelDriver->pModel->boneNameIndexMapping["J_Bip_R_LowerLeg"]].transformation =
        pModelDriver->pModel->bones[pModelDriver->pModel->boneNameIndexMapping["J_Bip_R_LowerLeg"]].bindingPoseTransformation *
        glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
    std::vector<glm::mat4> finalTransformations = CalcBonesFinalTransformations(*pModelDriver->pModel);
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)*200, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo); // 关联 UBO 到绑定点 0

    for (unsigned int i = 0; i < pModelDriver->pModel->bones.size(); i++) {
        std::cout << pModelDriver->pModel->bones[i].name << std::endl;
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
    for (auto vertex: pModelDriver->pModel->vertices) {
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

    pModelDriver->CalcBonesTransformations();
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4)*pModelDriver->finalBoneTransformations.size(), pModelDriver->finalBoneTransformations.data());
}

void MainApp::OnRender() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GL_CHECK_ERRORS(glBindVertexArray(vao));
    GL_CHECK_ERRORS(glDrawElements(GL_TRIANGLES, pModelDriver->pModel->indices.size(), GL_UNSIGNED_INT, nullptr));
    glBindVertexArray(0);

    glDisable(GL_DEPTH_TEST);
    //glClear(GL_DEPTH_BUFFER_BIT);
    std::vector<JointBall::InstanceAttributes> jointBallInstances;
    for (int i = 0; i < pModelDriver->pModel->bones.size(); i++) {
        //std::cout << pModelDriver->globalBoneTransformations[i][3][0] << " " << pModelDriver->globalBoneTransformations[i][3][1] << " " << pModelDriver->globalBoneTransformations[i][3][2] << std::endl;
        JointBall::InstanceAttributes instance{};
        instance.center = {pModelDriver->globalBoneTransformations[i][3][0], pModelDriver->globalBoneTransformations[i][3][1], pModelDriver->globalBoneTransformations[i][3][2]};
        instance.radius = 0.05;
        instance.color = {0, 1, 0, 0.1};
        jointBallInstances.push_back(instance);
    }
    pJointBall->DrawInstances(jointBallInstances,
        glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
        camera.GetCameraMatrix(), camera.GetPerspectiveMatrix());

    glfwSwapBuffers(window);
}

int main() {
    MainApp app;
    return app.Run();
}
