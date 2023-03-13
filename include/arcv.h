#ifndef _ARCV_H
#define _ARCV_H

#include <string>
#include <vector>

class arcv {
public:
    class member {
    public:
        std::string name(std::string t_name = "");
        std::vector<char> data(std::vector<char> t_data = {});
    private:
        std::string m_name;
        std::vector<char> m_data;
    };
    void from_file(const std::string& t_filename);
private:
    std::string m_filename;
    std::vector<member> m_members;
};

#endif
