#include <iostream>

#include "Core/Camera.h"
#include "Core/PrecompiledHeader.h"
#include "Core/IApplication.h"
#include "Model/VrmModel.h"

class MainApp final : public IApplication {

public:
    Camera camera;

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

void MainApp::OnInit() {
    // 导入模型
    VrmModel model(R"(E:\vrm\20220331_1455\20220331_1455\base body\black cat base body v3.5.0.vrm)");
    // 模型上传
    GLuint vao, vbo, ebo;
    GL_CHECK_ERRORS(SetUpModelToGL(model, vao, vbo, ebo));

}

void MainApp::OnUpdate() {

}

void MainApp::OnRender() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int main() {
    MainApp app;
    return app.Run();
}
