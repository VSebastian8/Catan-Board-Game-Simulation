#pragma once

class asset_error: public std::runtime_error{
    using std::runtime_error::runtime_error;
};
class font_error: public asset_error{
public:
    explicit font_error(const std::string& message): asset_error("Font didn't load: " + message) {}
};
class image_error: public asset_error{
public:
    explicit image_error(const std::string& message): asset_error("Image for " + message + " didn't load") {}
};

class game_error: public std::logic_error{
    using std::logic_error::logic_error;
};
class tile_bounds: public game_error{
public:
    explicit tile_bounds(const std::string& message): game_error("Border tile at: " + message) {};
};