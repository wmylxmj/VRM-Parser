//
// Created by 13973 on 25-2-28.
//

#pragma once
#include "Model.h"

class VrmModel : public Model {

public:
    std::map<std::string, unsigned int> humanBoneNameIndexMapping;
    explicit VrmModel(const std::string& filePath);

private:
    std::string ExtractVRMJson(const std::string& filePath);
};
