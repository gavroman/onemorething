#include "Game.h"

int main(const int argc, const char ** argv) {
    int map_number = (argc == 2) ? std::atoi(argv[1]) : 2;
    Game game;
    return 0;
}