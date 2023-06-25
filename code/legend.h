template<class T> class Legend2 {
    sf::RenderWindow* window;
    std::string content;
    T shape;
    sf::Text content_text;
    sf::Font content_font;
public:
    Legend2(sf::RenderWindow* w, std::string x, T ob): window(w), content(std::move(x)), shape(ob){}
    void init(float x, float y){
        std::cout << shape << "\n";

        if (!content_font.loadFromFile( "assets/georgia_bold.ttf")){
            throw font_error("georgia bold");
        }
        content_text.setString(content);
        content_text.setFont(content_font);
        content_text.setCharacterSize(24);
        content_text.setFillColor(sf::Color(227, 230, 193));
        content_text.setPosition(x, y);
    }
    void show(){
        window->draw(content_text);
    }
};

template<> class Legend2<sf::CircleShape> {
    sf::RenderWindow* window;
    std::string content;
    sf::CircleShape shape;
    const float radius;
    sf::Text content_text;
    sf::Font content_font;
public:
    Legend2(sf::RenderWindow* w, std::string x, float rad): window(w), content(std::move(x)), radius(rad){}
    void init(float x, float y);
    void show();
};

void Legend2<sf::CircleShape>::init(float x, float y) {
    shape.setFillColor(sf::Color(72, 23, 179));
    shape.setOrigin(radius, radius);
    shape.setRadius(radius);
    shape.setPosition(x, y);

    if (!content_font.loadFromFile( "assets/georgia_bold.ttf")){
        throw font_error("georgia bold");
    }
    content_text.setString(content);
    content_text.setFont(content_font);
    content_text.setCharacterSize(16);
    content_text.setOrigin(0, 8);
    content_text.setFillColor(sf::Color(227, 230, 193));
    content_text.setPosition(x + 50, y);
}

void Legend2<sf::CircleShape>::show() {
    window->draw(shape);
    window->draw(content_text);
}

template<> class Legend2<sf::RectangleShape> {
    sf::RenderWindow* window;
    std::string content;
    sf::RectangleShape shape;
    const sf::Vector2f size;
    sf::Text content_text;
    sf::Font content_font;
public:
    Legend2(sf::RenderWindow* w, std::string x, float width, float height): window(w), content(std::move(x)), size(sf::Vector2f(width, height)){}
    void init(float, float);
    void show();
};

void Legend2<sf::RectangleShape>::init(float x, float y) {
    if(content == "City")
        shape.setFillColor(sf::Color(212, 23, 74));
    else
        shape.setFillColor(sf::Color(27, 209, 164));
    shape.setOrigin(size.x / 2, size.y / 2);
    shape.setSize(size);
    shape.setPosition(x, y);

    if (!content_font.loadFromFile( "assets/georgia_bold.ttf")){
        throw font_error("georgia bold");
    }
    content_text.setString(content);
    content_text.setFont(content_font);
    content_text.setCharacterSize(16);
    content_text.setOrigin(0, 8);
    content_text.setFillColor(sf::Color(227, 230, 193));
    content_text.setPosition(x + 50, y);
}

void Legend2<sf::RectangleShape>::show() {
    window->draw(shape);
    window->draw(content_text);
}