#include <iostream>
#include <vector>
#include "brsar.h"
#include "types.h"

int Brsar::Main(const std::string& filename) {
    /* start header */
    char magic[4];
    in.read(magic, 4);
    if(!std::equal(magic, magic + 4, "RSAR")) {
        std::cerr << "expected brsar\n";
        return 1;
    }
    u16 bom = in.read_int(bom);
    if(bom == 0xfeff) in.endian() = std::endian::little;
    else in.endian() = std::endian::big;

    char verMax, verMin;
    in.read(&verMax, 1);
    in.read(&verMin, 1);

    if(verMax != 1 || verMin != 4) {
        std::cerr << "unsupported brsar version\n";
        return 1;
    }

    u32 size = in.read_int(size);
    u16 headerSize = in.read_int(headerSize);
    u16 sectionCount = in.read_int(sectionCount);

    u32 symbOff = in.read_int(symbOff);
    u32 symbSize = in.read_int(symbSize);
    u32 infoOff = in.read_int(infoOff);
    u32 infoSize = in.read_int(infoSize);
    u32 fileOff = in.read_int(fileOff);
    u32 fileSize = in.read_int(fileSize);
    /* end header */

    in.seekg(symbOff, in.beg);

    /* start symb header */
    char symbMagic[4];
    in.read(symbMagic, 4);
    if(!std::equal(symbMagic, symbMagic + 4, "SYMB")) {
        std::cerr << "expected symb section\n";
        return 1;
    }

    in.skipg(4);
    u32 filenameTableOff = in.read_int(filenameTableOff);
    u32 soundTreeOff = in.read_int(soundTreeOff);
    u32 playerTreeOff = in.read_int(playerTreeOff);
    u32 groupTreeOff = in.read_int(groupTreeOff);
    u32 bankTreeOff = in.read_int(bankTreeOff);
    /* end symb header */

    in.seekg(filenameTableOff, in.beg);

    /* start symb */
    /* start symb filename table */
    u32 filenameCount = in.read_int(filenameCount);
    std::vector<u32> filenameOffs(filenameCount);
    for(auto i = 0; i < filenameCount; i++) {
        in.read_int(filenameOffs[i]);
    }
    /* end symb filename table */
    /* start symb string table */
    u32 rootIdx = in.read_int(rootIdx);
    u32 entryCount = in.read_int(entryCount);

    struct StrNode {
        u16 flag;
        u16 bitFlag;
        u32 left;
        u32 right;
        u32 strIdx;
        u32 idx;
    };

    std::vector<struct StrNode> strNodes(entryCount);
    for(auto i = 0; i < entryCount; i++) {
        in.read(reinterpret_cast<char*>(&strNodes[i]), 20);
    }
    /* end symb string table */
    /* end symb */

    in.seekg(infoOff, in.beg);

    /* start info */
    enum DataRefType {
        REF_ADDR = 0,
        REF_OFF = 1
    };

    struct DataRef {
        u8 refType;
        u8 type; // 0
        u16 reserved;
        u32 val;
    };

    char infoMagic[4];
    in.read(infoMagic, 4);
    if(!std::equal(infoMagic, infoMagic + 4, "INFO")) {
        std::cerr << "expected info section\n";
        return 1;
    }

    in.skipg(4);

    struct DataRef soundDataTableRef;
    struct DataRef soundbankTableRef;
    struct DataRef playerInfoTableRef;
    struct DataRef collectionTableRef;
    struct DataRef groupTableRef;
    struct DataRef soundCountTable;

    in.read(reinterpret_cast<char*>(&soundDataTableRef), 8);
    in.read(reinterpret_cast<char*>(&soundbankTableRef), 8);
    in.read(reinterpret_cast<char*>(&playerInfoTableRef), 8);
    in.read(reinterpret_cast<char*>(&collectionTableRef), 8);
    in.read(reinterpret_cast<char*>(&groupTableRef), 8);
    in.read(reinterpret_cast<char*>(&soundCountTable), 8);
    /* end info */

    return 0;
}
