#pragma once

#include <string>

class Test {
public:
    Test(const std::string &name);
    virtual ~Test() = default;

    virtual void onUpdate() {}
    virtual void onRender() {}
    virtual void onImGuiRender() {}

    inline std::string getTestName() const { return m_name; } 

private:
    std::string m_name;
};