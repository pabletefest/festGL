#pragma once

#include "test.hpp"

#include <glm/vec4.hpp>

class TestClearColor : public Test {
public:
    TestClearColor(const std::string &name);
    virtual ~TestClearColor();

    void onUpdate() override;
    void onRender() override;
    void onImGuiRender() override;

private:
    glm::vec4 m_bgColor;
};