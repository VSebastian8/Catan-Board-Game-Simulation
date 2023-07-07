#include "player.h"
#include "structures.h"
#include <algorithm>
#include "errors.h"
#include <rlutil.h>

int Player::player_count = 0;
std::vector<sf::Color> Player::player_colors = {sf::Color(227, 230, 193),
                               sf::Color(72, 23, 179), sf::Color(212, 23, 74), sf::Color(27, 209, 164)};
Player::Player(std::string nm, const std::vector<int> &res) :
    name(std::move(nm)), player_number(++player_count), score(0), color(player_colors[player_number]), turn(false), resources(res){}

std::ostream &operator<<(std::ostream &out, const Player &p) {
    rlutil::resetColor();
    out << "Player " << p.player_number << " - " << p.name << "  Won!\n";
    rlutil::resetColor();
    return out;
}
sf::Text Player::show() {
    if (!info_font.loadFromFile( "assets/georgia_bold.ttf")){
        rlutil::setColor(rlutil::WHITE);
        throw font_error("georgia bold");
    }
    info.setFont(info_font);
    info.setString("Player " + std::to_string(player_number) + " - " + name + " [brick: " + std::to_string(resources[0]) +
                   ", sheep: " + std::to_string(resources[1]) + ", hay: " + std::to_string(resources[2]) + ", wood: " + std::to_string(resources[3]) +
                   ", rock: " + std::to_string(resources[4]) + "]   Score: " + std::to_string(score));
    info.setPosition(100, (float)(20 + 30 * player_number));
    info.setCharacterSize(20);
    if(turn)
        info.setFillColor(color);
    else
        info.setFillColor(player_colors[0]);
    return info;

}
void Player::set_turn(const bool value) {
    turn = value;
}
std::vector<int> Player::get_resources() const {
    return resources;
}
std::string Player::get_name() const {
    return name;
}
int Player::get_score() const {
    return score;
}
void Player::add_structure(const std::shared_ptr<Structure> &s) {
    structures.push_back(s);
    score++;
}
void Player::increase_res(int index, int count) {
    resources[index] += count;
}

void Player::decrease_res(int index, int count) {
    resources[index] -= count;
}

void Player::show_structures(sf::RenderWindow* window) {
    for(const auto& s : structures){
        s->show(color, window);
    }
}
bool Player::town_at(int x, int y){
    for (unsigned i = 0; i < structures.size(); ++i){
        if(structures[i]->at(x, y) && structures[i]->is_town()){
            structures.erase (structures.begin() + i);
            return true;
        }
    }
    return false;
}
bool Player::connects_to(int x1, int y1, int x2, int y2) {
    return std::any_of(structures.begin(), structures.end(), [&](auto i){return (i->at(x1, y1) || i->at(x2, y2));});
    /*for(const auto& i : structures)
          if(i->at(x1, y1) || i->at(x2, y2))
              return true;
      return false;*/
}