#ifndef _BYTESTREAM_H
#define _BYTESTREAM_H

#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include "my_endian.h"

typedef std::vector<char> databuf;

class bytestream {
public:
    ~bytestream();
    void open(const databuf& t_streambuf);
    void from_file(const std::string& t_filename);
    void from_ifstream(std::ifstream& t_ifstream);
    void from_bytestream(bytestream bs, int t_pos, int size);
    void close();

    void to_file(const std::string& t_filename = "");

    void read(char *ptr, int size);
    const char *rread(int size);
    template<typename T> T read_int() {
        if(m_is_open) {
            T result = 0;

            if(m_pos + sizeof(T) > size()) {
                throw std::runtime_error("error: reached EOS");
            } else {
                std::memcpy(reinterpret_cast<char*>(&result),
                    reinterpret_cast<char*>(&m_streambuf[0] + m_pos), sizeof(T));

                m_pos += sizeof(T);

                if(m_endianness == MY_NATIVE_ENDIAN) {
                    if(sizeof(T) == 2)
                        result = my_bswap_16(result);
                    else if(sizeof(T) == 4)
                        result = my_bswap_32(result);
                    else if(sizeof(T) == 8)
                        result = my_bswap_64(result);
                }
            }

            return result;
        } else {
            throw std::runtime_error("error: bytestream not open");
        }

        return 0;
    }
    char get();

    void write(const char *ptr, int size);
    template<typename T> void write_int(T x) {
        if(m_is_open) {
            if(m_pos + sizeof(T) > size()) {
                //throw std::runtime_error("error: reached EOS");
                
                m_streambuf.resize(m_streambuf.size() + sizeof(T));
            }

            T tmp = x;

            if(m_endianness == MY_NATIVE_ENDIAN) {
                if(sizeof(T) == 2)
                    tmp = my_bswap_16(tmp);
                else if(sizeof(T) == 4)
                    tmp = my_bswap_32(tmp);
                else if(sizeof(T) == 8)
                    tmp = my_bswap_64(tmp);
            }

            std::memcpy(reinterpret_cast<char*>(&m_streambuf[0] + m_pos), reinterpret_cast<char*>(&tmp), sizeof(T));

            m_pos += sizeof(T);
        } else {
            throw std::runtime_error("error: bytestream not open");
        }
    }
    void put(char c);

    constexpr static int beg = 0;
    constexpr static int cur = 1;
    constexpr static int end = 2;

    int tell();
    void seek(int t_pos, int whence = beg);

    int endianness(int t_endianness = -1);
    databuf rdbuf();
    bool is_open();
    int size();
    std::string name(std::string t_filename = "");
private:
    inline bool check_eof(int t_pos) {
        return (t_pos > size());
    }

    databuf m_streambuf;
    int m_pos = 0;

    int m_endianness = MY_NATIVE_ENDIAN;

    std::string m_filename = "";

    bool m_is_open = false;

    const std::string eof_error = "error: reached EOF";
};

#endif
