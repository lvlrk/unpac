#include "../include/my_file.h"
#include <cstring>
#include "../include/arcv.h"

namespace ft {
    int id_file(const std::string& filename) {
        bytestream bs;

        bs.from_file(filename);

        return id_bytestream(bs, filename);
    }

    int id_stream(std::ifstream& stream, const std::string& filename) {
        bytestream bs;

        bs.from_ifstream(stream);

        return id_bytestream(bs, filename);
    }

    int id_data(const databuf& data, const std::string& filename) {
        bytestream bs;

        bs.open(data);

        return id_bytestream(bs, filename);
    }

    // fun part
    int id_bytestream(bytestream bs, const std::string& filename) {
        int result = 0;

        char magic[4];
        bs.read(magic, 4);

        bs.seek(0);

        if(!memcmp(magic, "VCRA", 4)) {
            arcv a;

            //a.from_bytestream(bs);
        }

        return 0;
    }
}
