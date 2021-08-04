#pragma once

#include <string>
#include <list>

class Alert {
private:
    std::string m_key;
    std::string m_title;
    std::list<std::string> m_lines;

public:
    Alert(const std::string& key, const std::string& title, const std::list<std::string>& lines);
    const std::string& GetKey() const;
    const std::string& GetTitle() const;
    const std::list<std::string>& GetLines() const;
};
