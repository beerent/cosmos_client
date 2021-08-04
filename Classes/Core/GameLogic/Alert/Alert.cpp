#include <Core/GameLogic/Alert/Alert.h>

Alert::Alert(const std::string& key, const std::string& title, const std::list<std::string>& lines) {
    m_key = key;
    m_title = title;
    m_lines = lines;
}

const std::string& Alert::GetKey() const {
    return m_key;
}

const std::string& Alert::GetTitle() const {
    return m_title;
}

const std::list<std::string>& Alert::GetLines() const {
    return m_lines;
}
