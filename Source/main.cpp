#include <iostream>
#include <memory>
#include <queue>

#include "Core/Camera.h"
#include "Core/PrecompiledHeader.h"
#include "Core/IApplication.h"
#include "Model/VrmModel.h"
#include "Core//Shader.h"

class MainApp final : public IApplication {

public:
    GLFWwindow* window;
    Camera camera;
    std::unique_ptr<ShaderProgram> pShader;
    std::unique_ptr<Model> pModel;
    GLuint vao, vbo, ebo;

    bool LoopCondition() override { return !glfwWindowShouldClose(window); }
    void OnInit() override;
    void OnUpdate() override;
    void OnRender() override;
};

void SetUpModelToGL(const Model& model, GLuint &vao,GLuint &vbo, GLuint &ebo) {
    // 创建缓冲区对象
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // 将VBO绑定到VAO
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // 传输顶点数据
    glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(Vertex), model.vertices.data(), GL_STATIC_DRAW);

    // 传输索引数据
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(unsigned int), model.indices.data(), GL_STATIC_DRAW);

    // 顶点位置
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, position)));
    // 顶点法线
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, normal)));
    // 顶点骨骼索引
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, NUM_BONES_PER_VERTEX, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, boneIndices)));
    // 顶点骨骼权重
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, NUM_BONES_PER_VERTEX, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, boneWeights)));

    glBindVertexArray(0);
}



std::vector<glm::mat4> GetBonesFinalTransformations(const Model& model) {
    // 缓存，避免重复计算
    std::vector<glm::mat4> finalTransformations(model.bones.size());
    std::vector<glm::mat4> globalTransformations(model.bones.size());
    std::vector<bool> bonesCalculated(model.bones.size(), false);

    for (int i = 0; i < model.bones.size(); i++) {
        // 如果已计算，则跳过
        if (bonesCalculated[i]) continue;

        Bone bone(model.bones[i]);

        unsigned int parentIndex = bone.parentIndex;
        std::queue<unsigned int> parentChain;

        // 溯源到已计算的父节点或根节点
        while (parentIndex != INVALID_PARENT && !bonesCalculated[parentIndex]) {
            parentChain.push(parentIndex);
            parentIndex = model.bones[parentIndex].parentIndex;
        }

        while (!parentChain.empty()) {
            parentIndex = parentChain.front();
            parentChain.pop();

            Bone parentBone = model.bones[parentIndex];

            if (parentBone.parentIndex == INVALID_PARENT) {
                globalTransformations[parentIndex] = bone.transformation;
            }
            else {
                globalTransformations[parentIndex] = globalTransformations[parentBone.parentIndex] * bone.transformation;
            }
            finalTransformations[parentIndex] = globalTransformations[parentIndex] * parentBone.offsetMatrix;
            bonesCalculated[parentIndex] = true;
        }

        // 计算当前骨骼变换矩阵
        if (bone.parentIndex == INVALID_PARENT) {
            globalTransformations[i] = bone.transformation;
        }
        else {
            globalTransformations[i] = globalTransformations[bone.parentIndex] * bone.transformation;

        }
        finalTransformations[i] = bone.transformation * bone.offsetMatrix;
        bonesCalculated[i] = true;
    }

    return finalTransformations;
}

void MainApp::OnInit() {
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
    window = glfwCreateWindow(1024, 1024, "Vrm Parser", nullptr, nullptr);
    MakeContextCurrent(window);

    glEnable(GL_DEPTH_TEST);
    // 导入模型
    pModel = std::make_unique<VrmModel>(R"(E:\vrm\20220331_1455\20220331_1455\base body\black cat base body v3.5.0.vrm)");
    // 模型上传
    GL_CHECK_ERRORS(SetUpModelToGL(*pModel, vao, vbo, ebo));
    std::vector<glm::mat4> finalTransformations = GetBonesFinalTransformations(*pModel);

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

    pShader = std::make_unique<ShaderProgram>(R"(E:\VRM-Parser\Source\Shaders\Shader.vsh)", R"(E:\VRM-Parser\Source\Shaders\Shader.fsh)");
    GL_CHECK_ERRORS();
}

void MainApp::OnUpdate() {
    int windowWidth, windowHeight;
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

    camera.aspect = (float)windowWidth / (float)windowHeight;
    const ShaderProgram shader = *pShader;
    GL_CHECK_ERRORS(glUseProgram(shader.glID));
    GL_CHECK_ERRORS(shader.SetMat4("matModel", glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f))));
    shader.SetMat4("matView", camera.GetCameraMatrix());
    shader.SetMat4("matProjection", camera.GetPerspectiveMatrix());
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
