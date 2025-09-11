#include "test_clear_color.hpp"

#include <glad/gl.h>

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

TestClearColor::TestClearColor(const std::string &name)
    : Test(name), m_bgColor({ 0.0f, 0.0f, 0.0f, 1.0f })
{
}

TestClearColor::~TestClearColor()
{
}

void TestClearColor::onUpdate()
{
}

void TestClearColor::onRender()
{
    glClearColor(m_bgColor.r, m_bgColor.g, m_bgColor.b, m_bgColor.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void TestClearColor::onImGuiRender()
{
    ImGui::ColorEdit4("Background color", glm::value_ptr(m_bgColor));
}
