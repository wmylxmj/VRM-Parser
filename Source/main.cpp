#include <iostream>
#include <memory>

#include "Core/Camera.h"
#include "Core/PrecompiledHeader.h"
#include "Core/IApplication.h"
#include "Model/VrmModel.h"
#include "Core//Shader.h"

class MainApp final : public IApplication {

public:
    Camera camera;
    std::unique_ptr<ShaderProgram> pShader;
    std::vector<Model> models;
    GLuint vao, vbo, ebo;

    void SetWindowHints() override;
    void OnInit() override;
    void OnUpdate() override;
    void OnRender() override;
};

void MainApp::SetWindowHints() {
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
}

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

void MainApp::OnInit() {
    glViewport(0, 0, 1024, 1024);
    glEnable(GL_DEPTH_TEST);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "OpenGL Version: " << version << std::endl;
    // 导入模型
    VrmModel model(R"(E:\vrm\20220331_1455\20220331_1455\base body\black cat base body v3.5.0.vrm)");
    std::cout << "Model size: " << model.vertices.size() << std::endl;
    // 模型上传
    GL_CHECK_ERRORS(SetUpModelToGL(model, vao, vbo, ebo));

    float xMin = FLT_MAX;
    float xMax = -FLT_MAX;
    float yMin = FLT_MAX;
    float yMax = -FLT_MAX;
    float zMin = FLT_MAX;
    float zMax = -FLT_MAX;
    for (auto vertex: model.vertices) {
        if (vertex.position.x < xMin) xMin = vertex.position.x;
        if (vertex.position.x > xMax) xMax = vertex.position.x;
        if (vertex.position.y < yMin) yMin = vertex.position.y;
        if (vertex.position.y > yMax) yMax = vertex.position.y;
        if (vertex.position.z < zMin) zMin = vertex.position.z;
        if (vertex.position.z > zMax) zMax = vertex.position.z;
    }

    models.emplace_back(std::move(model));

    std::cout << "xMin: " << xMin << std::endl;
    std::cout << "xMax: " << xMax << std::endl;
    std::cout << "yMin: " << yMin << std::endl;
    std::cout << "yMax: " << yMax << std::endl;
    std::cout << "zMin: " << zMin << std::endl;
    std::cout << "zMax: " << zMax << std::endl;

    camera.eyePosition = glm::vec3(0.5*(xMax+xMin), 0.7*(yMin+yMax), zMax+std::max(zMax-zMin, std::max(xMax-xMin, yMax-yMin)));
    camera.aspect = (float)1024/(float)1024;
    //camera.eyePosition = glm::vec3(0.5*(xMax+xMin), 0.5*(yMin+yMax), 0);

    pShader = std::make_unique<ShaderProgram>(R"(E:\VRM-Parser\Source\Shaders\Shader.vsh)", R"(E:\VRM-Parser\Source\Shaders\Shader.fsh)");
    GL_CHECK_ERRORS();
}

void MainApp::OnUpdate() {
    const ShaderProgram shader = *pShader;
    GL_CHECK_ERRORS(glUseProgram(shader.glID));
    GL_CHECK_ERRORS(shader.SetMat4("matModel", glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f))));
    shader.SetMat4("matView", camera.GetCameraMatrix());
    shader.SetMat4("matProjection", camera.GetPerspectiveMatrix());
}

void MainApp::OnRender() {

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const ShaderProgram shader = *pShader;
    //GL_CHECK_ERRORS(glUseProgram(0));

    const Model model = models.at(0);
    GL_CHECK_ERRORS(glBindVertexArray(vao));
    GL_CHECK_ERRORS(glDrawElements(GL_TRIANGLES, model.indices.size(), GL_UNSIGNED_INT, nullptr));
    glBindVertexArray(0);
}

int main() {
    MainApp app;
    return app.Run();
}
