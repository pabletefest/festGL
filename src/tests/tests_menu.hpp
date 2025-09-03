#pragma once

#include "test.hpp"

#include <vector>
#include <memory>

class TestsMenu : public Test {
public:
    TestsMenu(const std::string &name);
    virtual ~TestsMenu() = default;

    void onUpdate() override;
    void onRender() override;
    void onImGuiRender() override;

    std::string getCurrentTestName() const;
    inline Test* getCurrentTest() const { return m_currentTest; }
    inline void setCurrentTest(Test *test) { m_currentTest = test; }

    template<typename T>
    void registerTest(const std::string &name);

private:
    std::vector<std::unique_ptr<Test>> m_tests;
    Test *m_currentTest;
};

template <typename T>
inline void TestsMenu::registerTest(const std::string &name)
{
    static_assert(std::is_convertible_v<T, Test>, "Provided type is not derived from Test type!");

    m_tests.push_back(std::make_unique<T>(name));
}
