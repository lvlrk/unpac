#include "../include/bytestream.h"
#include <stdexcept>
#include <fmt/core.h>

int EOS = -1;

bytestream::~bytestream() {
    close();
}

void bytestream::open(const databuf& t_streambuf) {
    if(m_is_open) {
        throw std::runtime_error("error: cannot open an already opened bytestream");
        return;
    }

    if(t_streambuf.empty()) {
        throw std::runtime_error("error: empty databuf");
        return;
    }

    m_streambuf = t_streambuf;
    m_is_open = true;
}

void bytestream::from_file(const std::string& t_filename) {
    if(m_is_open) {
        throw std::runtime_error("error: cannot open an already opened bytestream");
        return;
    }

    m_filename = t_filename;

    std::ifstream stream;
    stream.exceptions(std::ifstream::failbit);

    databuf buf;

    try {
        stream.open(m_filename, std::ios_base::binary);

        stream.seekg(0, stream.end);
        int size = stream.tellg();
        stream.seekg(0);

        buf.resize(size);

        stream.read(reinterpret_cast<char*>(&buf[0]), size);

        stream.close();
    } catch(const std::ifstream::failure& e) {
        stream.close();
        close();

        throw std::runtime_error(fmt::format("error: couldnt open bytestream"
            "from file '{}'", e.what()));
    }

    m_streambuf = buf;
    m_is_open = true;
}

void bytestream::from_ifstream(std::ifstream& t_ifstream) {
    if(m_is_open) {
        throw std::runtime_error("error: cannot open an already opened bytestream");
        return;
    }

    if(!(t_ifstream.exceptions() & std::ifstream::failbit))
        t_ifstream.exceptions(t_ifstream.exceptions() | std::ifstream::failbit);

    databuf buf;

    try {
        int streampos = t_ifstream.tellg();

        t_ifstream.seekg(0, t_ifstream.end);
        int size = t_ifstream.tellg();
        t_ifstream.seekg(0);

        buf.resize(size);

        t_ifstream.read(reinterpret_cast<char*>(&buf[0]), size);

        t_ifstream.seekg(streampos);
    } catch(const std::ifstream::failure& e) {
        t_ifstream.close();
        close();

        throw std::runtime_error(fmt::format("error: couldnt open bytestream"
            "from stream '{}'", e.what()));
    }

    m_streambuf = buf;
    m_is_open = true;
}

void bytestream::from_bytestream(bytestream bs, int t_pos, int size) {
    if(m_is_open) {
        throw std::runtime_error("error: cannot open an already opened bytestream");
        return;
    }

    if(!bs.is_open()) {
        throw std::runtime_error("error: cannot create a bytestream from a closed bytestream");
    }

    if(bs.rdbuf().empty()) {
        throw std::runtime_error("error: empty databuf");
    }

    if(t_pos > bs.size()) {
        throw std::runtime_error("error: reached EOS");
    }

    if(t_pos + size > bs.size()) {
        throw std::runtime_error("error: reached EOS");
    }

    m_streambuf.resize(size - t_pos);

    std::memcpy(reinterpret_cast<char*>(&m_streambuf[0]), reinterpret_cast<const char*>(&(bs.rdbuf()[0]) + t_pos), size - t_pos);
    m_is_open = true;

    m_endianness = bs.endianness();
    m_filename = bs.name();
}

void bytestream::close() {
    m_streambuf.clear();
    m_is_open = false;
}

void bytestream::to_file(const std::string& t_filename) {
    if(m_is_open) {
        std::ofstream stream;
        stream.exceptions(std::ofstream::failbit);
        
        try {
            if(t_filename == "" && m_filename != "")
                stream.open(m_filename, std::ofstream::trunc);
            else
                stream.open(t_filename, std::ofstream::trunc);

            stream.write(reinterpret_cast<char*>(&m_streambuf[0]), m_streambuf.size());

            stream.close();
        } catch(const std::ifstream::failure& e) {
            stream.close();
            close();

            throw std::runtime_error(fmt::format("error: couldnt write to file"
                "'{}'", e.what()));
        }
    } else {
        throw std::runtime_error("error: bytestream not open");
    }
}

void bytestream::read(char *ptr, int size) {
    if(m_is_open){
        if(ptr && size) {
            if(m_pos + size > static_cast<int>(m_streambuf.size())) {
                throw std::runtime_error("error: reached EOS");
            } else {
                std::memcpy(ptr, reinterpret_cast<char*>(&m_streambuf[0] + m_pos), size);

                m_pos += size;
            }
        } else {
            m_is_open = false;
            close();

            throw std::runtime_error("error: cant read into null pointer");
        }
    } else {
        throw std::runtime_error("error: bytestream not open");
    }
}

const char *bytestream::rread(int size) {
    char *out;

    if(m_is_open) {
        out = new char[size];

        std::memcpy(out, reinterpret_cast<char*>(&m_streambuf[0] + m_pos), size);
        m_pos += size;
    } else {
        throw std::runtime_error("error: bytestream not open");
    }

    return out;
}

char bytestream::get() {
    if(m_is_open) {
        if(m_pos + 1 > static_cast<int>(m_streambuf.size())) {
            throw std::runtime_error("error: reached EOS");
        } else {
            return m_streambuf[m_pos++];
        }
    } else {
        throw std::runtime_error("error: bytestream not open");
    }
}

void bytestream::write(const char *ptr, int size) {
    if(m_is_open){
        if(ptr && size) {
            if(m_pos + size > static_cast<int>(m_streambuf.size())) {
                m_streambuf.resize(m_streambuf.size() + size);
            }

            std::memcpy(reinterpret_cast<char*>(&m_streambuf[0] + m_pos), const_cast<char*>(ptr), size);

            m_pos += size;
        } else {
            m_is_open = false;
            close();

            throw std::runtime_error("error: cant write with null pointer");
        }
    } else {
        throw std::runtime_error("error: bytestream not open");
    }

}

void bytestream::put(char c) {
    if(m_is_open) {
        if(m_pos + 1 > static_cast<int>(m_streambuf.size())) {
            m_streambuf.resize(m_streambuf.size() + 1);
            //throw std::runtime_error("error: reached EOS");
        }

        //return streambuf[pos - 1];
        c = m_streambuf[m_pos++];
        //std::memcpy(reinterpret_cast<char*>(&streambuf[0] + pos), &c, 1);
    } else {
        throw std::runtime_error("error: bytestream not open");
    }
}

int bytestream::tell() {
    if(m_is_open) {
        return m_pos;
    } else {
        throw std::runtime_error("error: bytestream not open");
    }
}

void bytestream::seek(int p, int whence) {
    if(m_is_open) {
        switch(whence) {
            case beg:
                m_pos = p;
                break;
            case cur:
                m_pos += p;
                break;
            case end:
                m_pos = m_streambuf.size() - p;
                break;
        }
    } else {
        throw std::runtime_error("error: bytestream not open");
    }
}

int bytestream::endianness(int t_endianness) {
    if(m_is_open) {
        if(t_endianness != -1) {
            m_endianness = t_endianness;
            return -1;
        } else {
            return m_endianness;
        }
    } else {
        throw std::runtime_error("error: bytestream not open");
    }
}

databuf bytestream::rdbuf() {
    if(m_is_open) {
        return m_streambuf;
    } else {
        throw std::runtime_error("error: bytestream not open");
    }
}

bool bytestream::is_open() {
    return m_is_open;
}

int bytestream::size() {
    return m_streambuf.size();
}

std::string bytestream::name(std::string t_filename) {
    if(t_filename == "")
        return m_filename;
    else {
        m_filename = t_filename;
        return "";
    }
}
