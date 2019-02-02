#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <functional>
using namespace std;

const string filledInSquare("\u25A0");

struct Tile {
    bool containsMine; 
    Tile(): containsMine(false) {}
    Tile(bool _containsMine): containsMine(_containsMine) {}
};

struct TileState {
    bool clicked;
    int value;
    TileState(bool _clicked, int _value): clicked(_clicked), value(_value) {}
};

struct State {
    vector<vector<Tile>> grid;
};

bool victory();

string tileRepresentation(const Tile& tile) {
    return tile.containsMine ? "*" : " ";
}

string tileRepresentation(const TileState& tileState) {
    if (!tileState.clicked) {
        return filledInSquare;
    }

    return to_string(tileState.value);
}

ostringstream printGrid(const vector<vector<Tile>>& grid) {
    ostringstream ret;
    for (auto row : grid) {
        for (auto tile : row) {
            ret << tileRepresentation(tile);
        }
        ret << "\n";
    }
    return ret;
}

ostringstream printGridState(const vector<vector<TileState>>& gridState) {
    ostringstream a;
    for (auto row : gridState) {
        for (auto tileState : row) {
            a << tileRepresentation(tileState);
        }
        a << "\n";
    }
    return a;
}

struct Coordinate {
    int x;
    int y;
    Coordinate(int _x, int _y): x(_x), y(_y) {}
};

vector<vector<Tile>> constructGrid(const int size, vector<Coordinate> mineLocations) {
    vector<vector<Tile>> ret(size, vector<Tile>(size));

    for (auto location : mineLocations) {
        auto x = location.x;
        auto y = location.y;
        assert(x < size && x >= 0);
        assert(y < size && y >= 0);
        ret[x][y].containsMine = true;
    }

    return ret;
}

class GameState {

};

class Command {
    public:
        virtual void execute(GameState&) = 0;
};

class SelectCommand: public Command {
    private:
        Coordinate _c;
    
    public:
        SelectCommand(Coordinate c): _c(c) {}

        void execute(GameState& gs) {
            cout << "Selecting: (" << _c.x << ", " << _c.y << ")" << endl;
        }
};

void compileTest(GameState& gs) {
    cout << "bruh" << endl;
};

int main() {
    auto grid = constructGrid(5, {
        {0, 0},
        {1, 1},
        {4, 0},
        {0, 4}
    });

    auto repr = printGrid(grid);
    cout << repr.str() << endl;

    unordered_map<string, std::function<void(GameState&)>> commandMap;
    commandMap["select"] = compileTest;

    // unordered_map<string, std::function<void(Command::*)(GameState&)>> commandMap2;

    // commandMap2["select2"] = bind(&SelectCommand::execute, this, placeholders::_1);

    // commands must know how to parse the rest of the line, and how to create their commands
    // factory
    // command is select, I have the rest of the line -> creates command parser ->
    // parses the line, and creates a command object
    // call execute on that command

    // game loop
    while (!victory()) {
        string command;
        getline(cin, command);

        if (command == "exit") {
            cout << filledInSquare << endl;
            return 0;
        }

        cout << "Your command was: " << command << endl;
        cout << printGrid(grid).str() << endl;
    }
}

bool victory() {
    return false;
}



// we also have a game loop that takes state and event, and given the event, does the approprait ething to the sate
// function(state& state) // seems low key fluxxed up but maybe it'll work, makes it mutable only in the reducer?

// generate grid
// has to generate mines as well

// click on place -> reveal squares around if no mines
// put number of mines in surrounding area if there are squares

// get victory condition (all mines revealed, could be expensive to generate)