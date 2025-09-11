#include "tests_menu.hpp"

#include "glad/gl.h"

#include "imgui.h"

TestsMenu::TestsMenu(const std::string &name)
    : Test(name), m_currentTest(this)
{
}

void TestsMenu::onUpdate()
{
}

void TestsMenu::onRender()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void TestsMenu::onImGuiRender()
{
    for (const auto &test : m_tests) {
        if (ImGui::Button(test->getTestName().c_str())) {
            m_currentTest = test.get();
        }
    }
}

std::string TestsMenu::getCurrentTestName() const
{
    if (m_currentTest)
        return m_currentTest->getTestName();

    return this->getTestName();
}
