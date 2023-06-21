#pragma once

class Structure;
class Player{
    static int player_count;
    static std::vector<sf::Color> player_colors;
    std::string name;
    int player_number;
    sf::Color color;
    bool turn;
    std::vector<int> resources; //[brick, sheep, hay, wood, rock]
    sf::Text info;
    sf::Font info_font;
    std::vector<std::shared_ptr<Structure>> structures;
public:
    explicit Player(std::string nm = "?", const std::vector<int>& res = {3, 4, 2, 7, 3}): name(std::move(nm)), player_number(++player_count),
                                                        color(player_colors[player_number]), turn(false), resources(res){}
    ~Player() = default;
    friend std::ostream& operator <<(std::ostream& out, const Player& p);
    sf::Text show();
    void set_turn(const bool value){
        turn = value;
    }
    std::vector<int> get_resources(){
        return resources;
    }
    void add_structure(const std::shared_ptr<Structure>& s){
        structures.push_back(s);
    }
    void decrease_res(int index, int count){
        resources[index] -= count;
    }
    void increase_res(int index, int count){
        resources[index] += count;
    }
    void show_structures(sf::RenderWindow*);
    bool town_at(float, float);
};
int Player::player_count = 0;
std::vector<sf::Color> Player::player_colors = {sf::Color(227, 230, 193),
                       sf::Color(72, 23, 179), sf::Color(212, 23, 74), sf::Color(27, 209, 164)};

sf::Text Player::show() {
    if (!info_font.loadFromFile( "georgia bold.ttf")){
        rlutil::setColor(rlutil::WHITE);
        throw font_error("georgia bold");
    }
    info.setFont(info_font);
    info.setString("Player " + std::to_string(player_number) + " - " + name + " [brick: " + std::to_string(resources[0]) +
                   ", sheep: " + std::to_string(resources[1]) + ", hay: " + std::to_string(resources[2]) + ", wood: " + std::to_string(resources[3]) +
                   ", rock: " + std::to_string(resources[4]) + "]");
    info.setPosition(100, (float)(50 + 30 * player_number));
    info.setCharacterSize(20);
    if(turn)
        info.setFillColor(color);
    else
        info.setFillColor(player_colors[0]);
    return info;
}

std::ostream &operator<<(std::ostream &out, const Player &p) {
    rlutil::setColor(rlutil::WHITE);
    out << "Player " << p.player_number << " - " << p.name;
    out << " [brick: " << p.resources[0] << ", sheep: " << p.resources[1] << ", hay: " << p.resources[2]
        << ", wood: " << p.resources[3] << ", rock: " << p.resources[4] << "]\n";
    return out;
}

