#pragma once
#include <random>

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
    explicit Board(): tiles(std::vector<std::vector<std::shared_ptr<Tile>>>(dimension, std::vector<std::shared_ptr<Tile>>(dimension))) {}
    ~Board() = default;

    void init_tiles(float tw);
    void init_offset();
    void initialize(int mode);
    std::shared_ptr<Tile> tile_pointer(int, int, int, int);
    void animate(sf::RenderWindow*, bool&);
    void show(sf::RenderWindow* w);
    std::vector<std::pair<std::pair<int, int>, std::string>> rolled_dice(int);
    Board& operator |=(const Board&);
    Board& operator =(const Board&);
    Board(const Board& other);
};

class board_builder{
private:
    Board b;
public:
    board_builder() = default;
    board_builder& tile(float tw){
        b.tile_width = tw;
        b.init_tiles(tw);
        return *this;
    }
    board_builder& offset(float xf, float yf){
        b.start_from = sf::Vector2f(xf, yf);
        b.init_offset();
        return *this;
    }
    Board build(){
        return b;
    }
};


int Board::dimension = 7;
std::vector<int> Board::base_tiling = {
           0, 0, 0,
        0, 0, 1, 1, 1,
        1, 2, 2, 2, 3,
        3, 3, 4, 4, 4,
           5, 5, 6
};

void Board::init_tiles(float tw) {
    for (int i = 0; i < dimension; i++)
        for (int j = 0; j < dimension; j++)
            tiles[i][j] = std::make_shared<Ocean>(i, j, tw);
}
void Board::init_offset(){
    for (int i = 0; i < dimension; i++)
        for (int j = 0; j < dimension; j++)
            tiles[i][j]->calculate_offset(start_from);
}

void Board::show(sf::RenderWindow *w) {
    for(int i = 0; i < dimension; i++)
        for(int j = 0; j < dimension; j++){
            w->draw(tiles[i][j]->show());
            w->draw(tiles[i][j]->show_disk());
            w->draw(tiles[i][j]->show_dice_value());
        }
}

void Board::initialize(int mode = 0){
    //Tiles: Forest(0), Hill(1), Pasture(2), Field(3), Mountain(4), Rocky_Jungle(5), Desert(6), Ocean(7)

    static std::random_device rd;
    //mode 0 is default shuffled tiles and dice rolls
    //mode 1 and 2 showing just tiles (normal and shuffled)
    dice_rolls = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    if(mode != 1 and mode != 2)
        dice_rolls = {2, 3, 4, 4, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 10, 10, 11, 12};
    if(mode == 2)
        std::shuffle(base_tiling.begin(), base_tiling.end(), rd);
    //mode 3 not shuffled dice rolls
    if(mode == 0)
        std::shuffle(dice_rolls.begin(), dice_rolls.end(), rd);

    int index1 = -1, index2 = -1;
    for(int i = 2; i < 5; i++)
        tiles[i][1] = tile_pointer(base_tiling[++index1], i, 1, dice_rolls[++index2]);
    for(int j = 2; j < 5; j++)
        for(int i = 1; i < 6; i++)
            tiles[i][j] = tile_pointer(base_tiling[++index1], i, j, dice_rolls[++index2]);
    for(int i = 2; i < 5; i++)
        tiles[i][5] = tile_pointer(base_tiling[++index1], i, 5, dice_rolls[++index2]);

    for(int i = 0; i < dimension; i++)
        for(int j = 0; j < dimension; j++)
            tiles[i][j]->calculate_offset(start_from);

    //mistake example:
    //tiles[2][6] = tile_pointer(1, 2, 6, 12);
    //tiles[2][6]->calculate_offset(start_from);

    for(int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            try {
                tiles[i][j]->calculate_points((float) dimension - 1);
            }
            catch (tile_bounds &err) {
                std::cout << err.what() << "\n";
            }
        }
    }
}

std::shared_ptr<Tile> Board::tile_pointer(const int nr, int i, int j, int d){
    switch(nr){
        case 0: return std::make_shared<Forest>(i, j, tile_width, d);
        case 1: return std::make_shared<Hill>(i, j, tile_width, d);
        case 2: return std::make_shared<Pasture>(i, j, tile_width, d);
        case 3: return std::make_shared<Field>(i, j, tile_width, d);
        case 4: return std::make_shared<Mountain>(i, j, tile_width, d);
        case 5: return std::make_shared<Rocky_Jungle>(i, j, tile_width, d);
        case 6: return std::make_shared<Desert>(i, j, tile_width, d);
        case 7: return std::make_shared<Ocean>(i, j, tile_width);
        default: return std::make_shared<Tile>(i, j, tile_width, d);
    }
}

Board &Board::operator=(const Board &other) {
    for(int i = 0; i < dimension; i++)
        for(int j = 0; j < dimension; j++){
            tiles[i][j] = (other.tiles[i][j]->clone());
            tiles[i][j]->calculate_offset(start_from);
        }
    return *this;
}

Board &Board::operator|=(const Board &other) {
    //operator pentru copierea tipului de tile, nu si scorul
    for(int i = 0; i < dimension; i++)
        for(int j = 0; j < dimension; j++){
            int keep = tiles[i][j]->get_dice_value();
            tiles[i][j] = (other.tiles[i][j]->clone());
            tiles[i][j]->set_dice_value(keep);
            tiles[i][j]->calculate_offset(start_from);
        }
    return *this;
}

Board::Board(const Board &other): tile_width(other.tile_width), start_from(other.start_from), tiles(std::vector<std::vector<std::shared_ptr<Tile>>>(dimension, std::vector<std::shared_ptr<Tile>>(dimension))){
    for(int i = 0; i < dimension; i++)
        for(int j = 0; j < dimension; j++){
            tiles[i][j] = (other.tiles[i][j]->clone());
            tiles[i][j]->calculate_offset(start_from);
        }
}

std::vector<std::pair<std::pair<int, int>, std::string>> Board::rolled_dice(int dice) {
    std::vector<std::pair<std::pair<int, int>, std::string>> data;
    for(int i = 0; i < dimension; i++){
        for(int j = 0; j < dimension; j++){
            tiles[i][j]->outline_disk(dice);
            tiles[i][j]->parse_points(data, dice);
        }
    }
    return data;
}


