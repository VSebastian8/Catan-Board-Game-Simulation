#pragma once

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
public:
    explicit Player(const std::string& nm = "?", const std::vector<int>& res = {3, 5, 2, 7, 3}): name(nm), player_number(++player_count),
                                                        color(player_colors[player_number]), turn(false), resources(res){}
    ~Player() = default;
    friend std::ostream& operator <<(std::ostream& out, const Player& p);
    sf::Text show();
    void set_turn(const bool value){
        turn = value;
    }
};
int Player::player_count = 0;
std::vector<sf::Color> Player::player_colors = {sf::Color(235, 235, 235) ,sf::Color(255, 0, 0), sf::Color(0, 255, 0), sf::Color(0, 0, 255)};

sf::Text Player::show() {
    if (!info_font.loadFromFile( "georgia bold.ttf")){
        rlutil::setColor(rlutil::WHITE);
        throw font_error("georgia bold");
    }
    info.setFont(info_font);
    info.setString("Player " + std::to_string(player_number) + " - " + name + " [brick: " + std::to_string(resources[0]) +
                   ", sheep: " + std::to_string(resources[1]) + ", hay: " + std::to_string(resources[2]) + ", wood: " + std::to_string(resources[3]) +
                   ", rock: " + std::to_string(resources[4]) + " ]");
    info.setPosition(100, (float)(50 + 20 * player_number));
    info.setCharacterSize(16);
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
}