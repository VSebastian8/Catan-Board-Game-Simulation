#ifndef OOP_BOARD_H
#define OOP_BOARD_H

#include "tiles.h"

class Board{
    static int dimension;
    float tile_width{};
    sf::Vector2f start_from;
    std::vector<std::vector<std::shared_ptr<Tile>>> tiles;
    static std::vector<int> base_tiling;
    std::vector<int> dice_rolls;
    int anim_i = 0, anim_j = 0, board_timer = 0;
    friend class board_builder;
public:
    explicit Board();
    ~Board() = default;
    Board(const Board& other);
    Board& operator =(const Board&);
    Board& operator |=(const Board&);

    void init_tiles(float);
    void init_offset();
    void initialize(int mode = 0);
    std::shared_ptr<Tile> tile_pointer(int, int, int, int);
    void animate(sf::RenderWindow*, bool&);
    void show(sf::RenderWindow* w);
    std::vector<std::pair<std::pair<int, int>, std::string>> rolled_dice(int);
    void outline_tiles(int);
};

class board_builder{
private:
    Board b;
public:
    board_builder() = default;
    board_builder& tile(float tw);
    board_builder& offset(float xf, float yf);
    Board build();
};

#endif