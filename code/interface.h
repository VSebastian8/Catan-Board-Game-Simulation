#ifndef OOP_INTERFACE_H
#define OOP_INTERFACE_H

#include "board.h"
#include "structures.h"
#include "player.h"

class Game{
    int time, demo_time;
    sf::Event e;
    sf::VideoMode vm;
    sf::RenderWindow* window;
    Board b_final, b_ocean, b_tiled, b_tiled2, b_scored, b_scored2;
    std::vector<std::weak_ptr<Settlement>> buildings; //keeps track of all towns and cities
    std::vector<Player> players;
    Player* current_player = {};
    sf::Font dice_font;
    sf::Text dice_text;
    int dice1, dice2;
    int dice_timer, outline_timer, generation_timer;
    int turn_speed, roll_speed, transaction_speed;
    bool done, paused;
    sf::Text animation_text;
    sf::Font animation_font;
    std::vector<std::pair<int, std::vector<int>>> demo_data = {};
    std::vector<std::pair<int, std::vector<int>>>::iterator instruction;
public:
    Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    ~Game();
    static Game& get_game();
    void init_demo();
    void simulation(int);
    void initialize_dice_text();
    void show_generation(bool&);
    void run();
    void check_spots(const int&, const int&);
    void transaction(Player&, const std::string&, const std::vector<int>&);
    void roll_dice();
    void dice_animation();
    void dice_effects();
    static void calculate_win(std::vector<Player>&);
    void make_turn(Player& p);
    void check_input(int&);
};

bool on_board(const std::vector<int>& list);

#endif