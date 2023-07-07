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
    explicit resource_error(const std::string& str, const std::string& res, const std::string& player = ""): game_error(player + "Not enough resources for " + str + ", need more " + res){};
};

class bad_placement: public game_error{
public:
    explicit bad_placement(const char* message): game_error(message){}
};
class weird_road: public bad_placement{
public:
    explicit weird_road(): bad_placement("Wrong road coordinates"){};
};
class unconnected_road: public bad_placement{
public:
    explicit unconnected_road(): bad_placement("Road doesn't attach to structure"){};
};
class lonely_city: public bad_placement{
public:
    explicit lonely_city(): bad_placement("City not built over town"){};
};
class spot_taken: public bad_placement{
public:
    explicit spot_taken(): bad_placement("Can't built town over other structures"){};
};
class spot_neighbour: public bad_placement{
public:
    explicit spot_neighbour(): bad_placement("Can't built town near other structures"){};
};