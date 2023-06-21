#pragma once

class Legend{
    sf::RenderWindow* window;
    sf::CircleShape lg_town;
    sf::RectangleShape lg_city;
    sf::RectangleShape lg_road;
    sf::Font lg_font;
    sf::Text lg_town_text;
    sf::Text lg_city_text;
    sf::Text lg_road_text;
    void initialize_shapes();
    void initialize_texts();
public:
    explicit Legend(sf::RenderWindow* window): window(window){
        if (!lg_font.loadFromFile( "georgia bold.ttf")){
            throw font_error("georgia bold");
        }
        initialize_shapes();
        initialize_texts();
    }
    void show_legend(){
        window->draw(lg_town);
        window->draw(lg_city);
        window->draw(lg_road);
        window->draw(lg_town_text);
        window->draw(lg_city_text);
        window->draw(lg_road_text);
    }
};

void Legend::initialize_shapes() {
    lg_town.setFillColor(sf::Color(72, 23, 179));
    lg_town.setOrigin(0, 0);
    lg_town.setRadius(12.5);
    lg_town.setPosition(1300, 800);

    lg_city.setFillColor(sf::Color(212, 23, 74));
    lg_city.setOrigin(0, 0);
    lg_city.setSize(sf::Vector2f(25, 25));
    lg_city.setPosition(1300, 850);

    lg_road.setFillColor(sf::Color(27, 209, 164));
    lg_road.setOrigin(0, 0);
    lg_road.setSize(sf::Vector2f(60, 20));
    lg_road.setPosition(1282, 900);
}
void Legend::initialize_texts() {
    lg_town_text.setFont(lg_font);
    lg_town_text.setString("Town");
    lg_town_text.setPosition(1370, 800);
    lg_town_text.setCharacterSize(16);
    lg_town_text.setFillColor(sf::Color(227, 230, 193));

    lg_city_text.setFont(lg_font);
    lg_city_text.setString("City");
    lg_city_text.setPosition(1370, 850);
    lg_city_text.setCharacterSize(16);
    lg_city_text.setFillColor(sf::Color(227, 230, 193));

    lg_road_text.setFont(lg_font);
    lg_road_text.setString("Road");
    lg_road_text.setPosition(1370, 900);
    lg_road_text.setCharacterSize(16);
    lg_road_text.setFillColor(sf::Color(227, 230, 193));
}

void info_console(){
    rlutil::setColor(rlutil::YELLOW);
    std::vector<std::shared_ptr<Tile>> info_tiles = {std::make_shared<Forest>(), std::make_shared<Hill>(), std::make_shared<Pasture>(),
                                                     std::make_shared<Field>(), std::make_shared<Mountain>(), std::make_shared<Rocky_Jungle>()};
    for(const auto &i : info_tiles)
        std::cout << *i;
    std::cout << "\n";
    rlutil::resetColor();
}