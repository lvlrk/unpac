#pragma once

#include <string>
#include "types.h"

namespace g03 {
class Item {
public:
    enum Type {
        DOT = 0,
        PELLET = 1,
        KNIGHT = 2,
        WING = 3,
        NORMAL = 4,
        APPLE = 5,
        APPLE_HALF = 6,
        GRAPE = 7,
        WHITE_GRAPE = 8,
        BANANAS = 9,
        BANANA = 10,
        BEEG_BURGER = 11,
        BURGER = 12,
        VANILLA_SHAKE = 13,
        CHOCO_SHAKE = 14,
        BEEG_CHICKEN = 15,
        CHICKEN = 16,
        PIZZA = 17,
        LIFE_FLAG = 18
    };

    u32 type = DOT; // ItemSetType
    u32 unused1[3] = {0, 0, 0};

    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;
    f32 one = 1.0f;

    u32 unused2[4] = {0, 0, 0, 0};

    void Print();
    int ReadFromFile(int index = 0, const std::string& filename = "BIN_ITEM_SET_DAT");
    int WriteToFile(int index = 0, const std::string& filename = "BIN_ITEM_SET_DAT");
};

enum BoxSetType {
    BOX_WOOD_BIG = 0,
    BOX_WOOD_SMALL = 1,
    BOX_METAL_BIG = 2,
    BOX_METAL_SMALL = 3
};

enum BoxSetItemType {
    BOX_ITEM_NONE = 0,
    BOX_ITEM_DOT1 = 1,
    BOX_ITEM_DOT3 = 2,
    BOX_ITEM_DOT6 = 3,
    BOX_ITEM_DOT9 = 4,
    BOX_ITEM_PELLET = 5,
    BOX_ITEM_KNIGHT = 6,
    BOX_ITEM_WING = 7,
    BOX_ITEM_NORMAL = 8,
    BOX_ITEM_APPLE = 9,
    BOX_ITEM_APPLE_HALF = 10,
    BOX_ITEM_GRAPE = 11,
    BOX_ITEM_WHITE_GRAPE = 12,
    BOX_ITEM_BANANAS = 13,
    BOX_ITEM_BANANA = 14,
    BOX_ITEM_BEEG_BURGER = 15,
    BOX_ITEM_BURGER = 16,
    BOX_ITEM_VANILLA_SHAKE = 17,
    BOX_ITEM_CHOCO_SHAKE = 18,
    BOX_ITEM_BEEG_CHICKEN = 19,
    BOX_ITEM_CHICKEN = 20,
    BOX_ITEM_PIZZA = 21,
    BOX_ITEM_LIFE_FLAG = 22,
};

class Box {
public:
    enum Type {
        WOOD_BIG = 0,
        WOOD_SMALL = 1,
        METAL_BIG = 2,
        METAL_SMALL = 3
    };

    enum ItemType {
        NONE = 0,
        DOT1 = 1,
        DOT3 = 2,
        DOT6 = 3,
        DOT9 = 4,
        PELLET = 5,
        KNIGHT = 6,
        WING = 7,
        NORMAL = 8,
        APPLE = 9,
        APPLE_HALF = 10,
        GRAPE = 11,
        WHITE_GRAPE = 12,
        BANANAS = 13,
        BANANA = 14,
        BEEG_BURGER = 15,
        BURGER = 16,
        VANILLA_SHAKE = 17,
        CHOCO_SHAKE = 18,
        BEEG_CHICKEN = 19,
        CHICKEN = 20,
        PIZZA = 21,
        LIFE_FLAG = 22,
    };

    u32 type = NONE;
    u32 itemType = WOOD_BIG;
    u32 unused1[2] = {0, 0};

    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;
    f32 one = 1.0f;

    u32 unused2[4] = {0, 0, 0, 0};

    void Print();
};

class Gate {
public:
    u32 dots = 0;
    u32 unused1[3] = {0, 0, 0};

    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;
    f32 one = 1.0f;

    u32 unused2[4] = {0, 0, 0, 0};

    int ReadFromFile(int index = 0, const std::string& filename = "BIN_GATE_SET_DAT");
    int WriteToFile(int index = 0, const std::string& filename = "BIN_ITEM_SET_DAT");
};

class Goal {
public:
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;
    f32 one = 1.0f;
    
    u32 unused[4] = {0, 0, 0, 0};
};

class Start {
public:
    u32 unused1[4] = {0, 0, 0, 0};

    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;
    f32 one = 1.0f;

    u32 rotation = 180;
    u32 unused2[3] = {0, 0, 0};

    u32 unused3[4] = {0, 0, 0, 0};
};

class Point {
public:
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;
    f32 one = 1.0f;

    u32 unused[4] = {0, 0, 0, 0};
};
}
