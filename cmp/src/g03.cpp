#include <iostream>
#include "g03.h"
#include "estream.h"

void g03::Item::Print() {
    switch(type) {
    case DOT:
        std::cout << "Pac-Dot\n";
        break;
    case PELLET:
        std::cout << "Power-Pellet\n";
        break;
    case KNIGHT:
        std::cout << "Knight-Chocolate\n";
        break;
    case WING:
        std::cout << "Wing-Chocolate\n";
        break;
    case NORMAL:
        std::cout << "Pac-Chocolate\n";
        break;
    case APPLE:
        std::cout << "Apple\n";
        break;
    case APPLE_HALF:
        std::cout << "Half an Apple\n";
        break;
    case GRAPE:
        std::cout << "Grapes\n";
        break;
    case WHITE_GRAPE:
        std::cout << "White Grapes\n";
        break;
    case BANANAS:
        std::cout << "Bananas\n";
        break;
    case BANANA:
        std::cout << "Peeled Banana\n";
        break;
    case BEEG_BURGER:
        std::cout << "MEGA-Cheeseburger\n";
        break;
    case BURGER:
        std::cout << "Cheeseburger\n";
        break;
    case VANILLA_SHAKE:
        std::cout << "Vanilla Milkshake\n";
        break;
    case CHOCO_SHAKE:
        std::cout << "Chocolate Milkshake\n";
        break;
    case BEEG_CHICKEN:
        std::cout << "Whole Chicken\n";
        break;
    case CHICKEN:
        std::cout << "Chicken Drumstick\n";
        break;
    case PIZZA:
        std::cout << "Health-Up Pizza\n";
        break;
    case LIFE_FLAG:
        std::cout << "Life-Up Flag\n";
        break;
    default:
        std::cout << "Unknown (will crash)\n";
        break;
    }
}

int g03::Item::ReadFromFile(int index, const std::string& filename) {
    estream in(filename);
    if(!in.is_open()) return 1;
    in.endian() = std::endian::big;

    in.seekg(index * 0x30, in.beg);

    type = in.read_int<u32>();
    in.read(reinterpret_cast<char*>(&unused1), 12);

    x = in.read_float();
    y = in.read_float();
    z = in.read_float();
    one = in.read_float();

    in.read(reinterpret_cast<char*>(&unused2), 16);

    return 0;
}

int g03::Item::WriteToFile(int index, const std::string& filename) {
    estream out(filename);
    if(!out.is_open()) return 1;
    out.endian() = std::endian::big;

    out.seekp(index * 0x30, out.beg);

    out.write_int(type);
    out.write(reinterpret_cast<char*>(&unused1), 12);

    out.write_float(x);
    out.write_float(y);
    out.write_float(z);
    out.write_float(one);

    out.write(reinterpret_cast<char*>(&unused2), 16);

    return 0;
}

int g03::Gate::ReadFromFile(int index, const std::string& filename) {
    estream in(filename);
    if(!in.is_open()) return 1;
    in.endian() = std::endian::big;

    in.seekg(index * 0x30, in.beg);

    dots = in.read_int<u32>();
    in.read(reinterpret_cast<char*>(&unused1), 12);

    x = in.read_float();
    y = in.read_float();
    z = in.read_float();
    one = in.read_float();

    in.read(reinterpret_cast<char*>(&unused2), 16);

    return 0;
}

int g03::Gate::WriteToFile(int index, const std::string& filename) {
    estream out(filename);
    if(!out.is_open()) return 1;
    out.endian() = std::endian::big;

    out.seekp(index * 0x30, out.beg);

    out.write_int(dots);
    out.write(reinterpret_cast<char*>(&unused1), 12);

    out.write_float(x);
    out.write_float(y);
    out.write_float(z);
    out.write_float(one);

    out.write(reinterpret_cast<char*>(&unused2), 16);

    return 0;
}
