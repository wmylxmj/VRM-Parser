//
// Created by 13973 on 25-3-15.
//

#pragma once

class IApplication {

public:
    virtual ~IApplication() = default;

    static int MakeContextCurrent(GLFWwindow *window);
    virtual bool LoopCondition() { return false; }

    virtual void OnInit() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;

    virtual int Run();
};
