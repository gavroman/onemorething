#include "Game.h"

std::vector<std::unique_ptr<Player>> players;

int main(const int argc, const char ** argv) {
    int map_number = (argc == 2) ? std::atoi(argv[1]) : 2;
    Game game(map_number);
    return 0;
}