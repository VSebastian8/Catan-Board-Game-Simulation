#pragma once

class Board{
    static int dimension;
    bool animation;
    std::vector<std::vector<std::shared_ptr<Tile>>> tiles;
    std::vector<int> base_tiling, dice_rolls;
public:
    explicit Board(bool anim = false): animation(anim),
                                       tiles(std::vector<std::vector<std::shared_ptr<Tile>>>(dimension, std::vector<std::shared_ptr<Tile>>(dimension))){
        for(int i = 0; i < dimension; i++)
            for(int j = 0; j < dimension; j++)
                tiles[i][j] = std::make_shared<Ocean>(i, j);
    }
    ~Board() = default;

    void initialize();
    static std::shared_ptr<Tile> tile_pointer(int nr, int i, int j, int d);
    void show(sf::RenderWindow* w){
        for(int i = 0; i < dimension; i++)
            for(int j = 0; j < dimension; j++){
                w->draw(tiles[i][j]->show());
                w->draw(tiles[i][j]->show_disk());
                w->draw(tiles[i][j]->show_dice_value());
            }
    }
};

int Board::dimension = 7;

void Board::initialize(){
    //Tiles: Forest(0), Hill(1), Pasture(2), Field(3), Mountain(4), Rocky_Jungle(5), Desert(6), Ocean(7)
    base_tiling = {
               1, 0, 3,
            2, 3, 4, 0, 5,
            1, 2, 0, 1, 3,
            4, 1, 0, 5, 4,
               0, 2, 6
    };
    static std::random_device rd;
    std::shuffle(base_tiling.begin(), base_tiling.end(), rd);
    dice_rolls = {2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 8, 8, 8, 8, 9, 9, 10, 10, 11, 11};
    std::shuffle(dice_rolls.begin(), dice_rolls.end(), rd);

    if(!animation){
        int index1 = -1, index2 = -1;
        for(int j = 2; j < 5; j++)
            tiles[1][j] = tile_pointer(base_tiling[++index1], 1, j, dice_rolls[++index2]);
        for(int i = 2; i < 5; i++)
            for(int j = 1; j < 6; j++)
                tiles[i][j] = tile_pointer(base_tiling[++index1], i, j, dice_rolls[++index2]);
        for(int j = 2; j < 5; j++)
            tiles[5][j] = tile_pointer(base_tiling[++index1], 5, j, dice_rolls[++index2]);
    }
    for(int i = 0; i < dimension; i++){
        for(int j = 0; j < dimension; j++) {
            tiles[i][j]->calculate_points();
        }
    }
    std::cout << "Punctele vecine pentru tile(5, 3):\n";
    tiles[5][3]->parse_points();
}

std::shared_ptr<Tile> Board::tile_pointer(const int nr, int i, int j, int d){
    switch(nr){
        case 0: return std::make_shared<Forest>(i, j, d);
        case 1: return std::make_shared<Hill>(i, j, d);
        case 2: return std::make_shared<Pasture>(i, j, d);
        case 3: return std::make_shared<Field>(i, j, d);
        case 4: return std::make_shared<Mountain>(i, j, d);
        case 5: return std::make_shared<Rocky_Jungle>(i, j, d);
        case 6: return std::make_shared<Desert>(i, j);
        default: return std::make_shared<Tile>(i, j, d);
    }
}