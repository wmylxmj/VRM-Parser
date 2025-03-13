#include <iostream>

#include "Model/VrmModel.h"

int main() {
    std::cout << "Loading Model..." << std::endl;
    VrmModel model(R"(E:\vrm\20220331_1455\20220331_1455\base body\black cat base body v3.5.0.vrm)");
    std::cout << "Model Loaded." << std::endl;
    std::cout << "Num Vertices: " << model.vertices.size() << std::endl;
    std::cout << "Num Triangle Faces: " << model.indices.size() / 3 << std::endl;
    std::cout << "Num Meshes: " << model.meshEntries.size() << std::endl;
    std::cout << "Num Bones: " << model.bones.size() << std::endl;
    std::cout << "Bone Hierarchy: " << std::endl;
    for (auto &[fst, snd] : model.boneIndexMapping) {
        std::cout << fst << ": " << snd << "->" << model.bones[snd].parentIndex <<  std::endl;
    }
    return 0;
}
