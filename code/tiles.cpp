#include "tiles.h"
#include "errors.h"
#include "rlutil.h"

Tile::Tile(int x, int y, float w, int d) : x(x), y(y), width(w), disk_width(w / 4), dim(sf::Vector2f(w, w)), disk_dim(sf::Vector2f(w / 4, w / 4)),
    dice_roll(d), x_offset(0), y_offset(0), adjacent_points(std::vector<std::pair<int, int>>()),
    poz(sf::Vector2f(float(x) * width,float(y) * width)),
    color(sf::Color(0,0,0)), disk_color(sf::Color(220, 220, 220)), outlined(false)
{
    square.setOrigin(0, 0);
    square.setSize(dim);
    disk.setOrigin(disk_dim);
    disk.setRadius(disk_width);
    initialize_text();
}
Tile::Tile(std::string name, std::string res) : name(std::move(name)), resource(std::move(res)), x(-1), y(-1), width(25),
    disk_width(8), dim(sf::Vector2f(25, 25)), disk_dim(sf::Vector2f(8, 8)), dice_roll(0), x_offset(0), y_offset(0),
    adjacent_points(std::vector<std::pair<int, int>>()), poz(sf::Vector2f((float)x * width, (float)y * width)),
    color(sf::Color(0,0,0)), disk_color(sf::Color(220, 220, 220)), outlined(false)
{
    square.setOrigin(0, 0);
    square.setSize(dim);
    disk.setOrigin(disk_dim);
    disk.setRadius(disk_width);
}
void Tile::initialize_text(){
    if (!font.loadFromFile( "assets/courier_prime_bold.ttf" )){
        rlutil::resetColor();
        throw font_error("courier prime bold");
    }
    text.setFont(font);
    if(dice_roll < 10)
        text.setOrigin(0.07f * width, 0.16f * width);
    else
        text.setOrigin(0.13f * width, 0.16f * width);
    text.setString(std::to_string(dice_roll));
    text.setCharacterSize((unsigned int)(24 * width / 100));
    text.setPosition(x_offset + (float)x * width + width / 2, y_offset + (float)y * width + width / 2);
}

void Tile::print() const {
    std::cout << name << " gives you: " << resource << "\n";
}
std::ostream &operator<<(std::ostream &out, const Tile &tile) {
    tile.print();
    return out;
}

std::shared_ptr<Tile> Tile::clone() {
    return std::make_shared<Tile>(*this);
}
void Tile::calculate_offset(sf::Vector2f place) {
    x_offset = place.x;
    y_offset = place.y;
    text.setPosition(x_offset + (float)x * width + width / 2, y_offset + (float)y * width + width / 2);
}
sf::RectangleShape Tile::show() {
    square.setPosition(poz + sf::Vector2f(x_offset, y_offset));
    square.setFillColor(color);
    return square;
}
sf::CircleShape Tile::show_disk() {
    if(dice_roll < 2 || dice_roll > 12)
        return sf::CircleShape();
    disk.setPosition(x_offset + (float)x * width + width / 2, y_offset + (float)y * width + width / 2);
    disk.setFillColor(disk_color);
    return disk;
}

int Tile::get_dice_value() const {
    return dice_roll;
}
void Tile::set_dice_value(int score) {
    dice_roll = score;
    text.setString(std::to_string(dice_roll));
}
sf::Text Tile::show_dice_value() {
    if(dice_roll < 2 || dice_roll > 12)
        return {};
    if(outlined){
        disk.setOutlineThickness(3);
        disk.setOutlineColor(sf::Color(181, 112, 230));
    }
    else
        disk.setOutlineThickness(0);

    text.setFillColor(color);
    return text;
}
void Tile::outline_disk(int dice) {
    outlined = (dice_roll == dice);
}

void Tile::calculate_points(int d) {
    if(x <= 0 || y <= 0 || x >= d || y >= d)
        throw tile_bounds("(" + std::to_string(x) + ", " + std::to_string(y) + ")");
    adjacent_points = {std::pair<int, int>(x - 1, y - 1), std::pair<int, int>(x, y - 1),
                       std::pair<int, int>(x - 1, y), std::pair<int, int>(x, y)};
}
void Tile::parse_points(std::vector<std::pair<std::pair<int, int>, std::string>> &data, int &dice) {
    if(dice_roll == dice)
        for(auto point : adjacent_points){
            data.emplace_back(point, resource);
        }
}


Ocean::Ocean(int x, int y, float w) : Tile(x, y, w, 0) {
    name = "Ocean";
    resource = "none";
    color = sf::Color(120, 230, 223 );
}
std::shared_ptr<Tile> Ocean::clone() {
    return std::make_shared<Ocean>(*this);
}

Desert::Desert(int x, int y, float w, int d) : Tile(x, y, w, d) {
    name = "Desert";
    resource = "none";
    color = sf::Color(	227, 230, 193);
}
std::shared_ptr<Tile> Desert::clone() {
    return std::make_shared<Desert>(*this);
}

Hill::Hill(int x, int y, float w, int d) : Tile(x, y, w,d) {
    name = "Hill";
    resource = "brick";
    color = sf::Color(236, 148, 101);
}
std::shared_ptr<Tile> Hill::clone() {
    return std::make_shared<Hill>(*this);
}

Pasture::Pasture(int x, int y, float w, int d) : Tile(x, y, w,d) {
    name = "Pasture";
    resource = "sheep";
    color = sf::Color(138, 234, 146);
}
std::shared_ptr<Tile> Pasture::clone() {
    return std::make_shared<Pasture>(*this);
}

Field::Field(int x, int y, float w, int d) : Tile(x, y, w, d) {
    name = "Field";
    resource = "hay";
    color = sf::Color(246, 204, 87);
}
std::shared_ptr<Tile> Field::clone() {
    return std::make_shared<Field>(*this);
}

Forest::Forest(int x, int y, float w, int d) : Tile(x, y, w, d) {
    name = "Forest";
    resource = "wood";
    color = sf::Color(48, 103, 91);
}
std::shared_ptr<Tile> Forest::clone() {
    return std::make_shared<Forest>(*this);
}

Mountain::Mountain(int x, int y, float w, int d) : Tile(x, y, w,d) {
    name = "Mountain";
    resource = "rock";
    color = sf::Color(64, 71, 109);
}
std::shared_ptr<Tile> Mountain::clone() {
    return std::make_shared<Mountain>(*this);
}

Rocky_Jungle::Rocky_Jungle(int x, int y, float w, int d) : Tile(x, y, w,d), resource2("wood") {
    name = "Rocky Jungle";
    color = sf::Color(85, 180, 159);
}
void Rocky_Jungle::print() const {
    std::cout << name << " gives you: " << resource << " + " << resource2 << "\n";
}
void Rocky_Jungle::parse_points(std::vector<std::pair<std::pair<int, int>, std::string>> &data, int &dice) {
    if(dice_roll == dice)
        for(auto point : adjacent_points){
            data.emplace_back(point, resource);
            data.emplace_back(point, resource2);
        }
}
std::shared_ptr<Tile> Rocky_Jungle::clone() {
    return std::make_shared<Rocky_Jungle>(*this);
}

void info_console() {
    rlutil::saveDefaultColor();
    rlutil::setColor(rlutil::YELLOW);
    std::vector<Tile> info_tiles = {tile_factory::forest(), tile_factory::hill(), tile_factory::pasture(), tile_factory::field(),
                                    tile_factory::mountain(), tile_factory::rocky_jungle()};
    for(const auto &i : info_tiles)
        std::cout << i;
    rlutil::resetColor();
    std::cout << "You can pause or quit anytime by pressing P/Q\n\n";
}