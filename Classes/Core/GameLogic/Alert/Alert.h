#pragma once

#include <string>
#include <vector>

class Alert {
private:
    std::string m_key;
    std::string m_title;
    std::vector<std::string> m_lines;

public:
    Alert(const std::string& key, const std::string& title, const std::vector<std::string>& lines);
    const std::string& GetKey() const;
    const std::string& GetTitle() const;
    const std::vector<std::string>& GetLines() const;
};
