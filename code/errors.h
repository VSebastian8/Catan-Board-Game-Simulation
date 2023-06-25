#pragma once

class asset_error: public std::runtime_error{
    using std::runtime_error::runtime_error;
};
class font_error: public asset_error{
public:
    explicit font_error(const std::string& message): asset_error("Font didn't load: " + message) {}
};
//class image_error: public asset_error{
//public:
//    explicit image_error(const std::string& message): asset_error("Image for " + message + " didn't load") {}
//};

class game_error: public std::logic_error{
    using std::logic_error::logic_error;
};
class tile_bounds: public game_error{
public:
    explicit tile_bounds(const std::string& message): game_error("Border tile at: " + message) {};
};
class resource_error: public game_error{
public:
    explicit resource_error(const std::string& str, const std::string& res): game_error("Not enough resources for " + str + ", need more " + res){};
};
class wrong_road_error: public game_error{
public:
    explicit wrong_road_error(): game_error("Wrong road coordinates"){};
};
class city_error: public game_error{
public:
    explicit city_error(): game_error("City not built over town"){};
};