#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <functional>
using namespace std;

const string filledInSquare("\u25A0");

struct Event {
    const string label;
};

void dispatch(Event event) {

}

struct Coordinate {
    int x;
    int y;
    Coordinate(int _x, int _y): x(_x), y(_y) {}
};

struct Tile {
    bool containsMine; 
    Tile(): containsMine(false) {}
    Tile(bool _containsMine): containsMine(_containsMine) {}
};

struct TileState {
    bool clicked;
    int value;
    TileState(): clicked(false), value(-1) {}
    TileState(bool _clicked, int _value): clicked(_clicked), value(_value) {}
};

struct State {
    vector<vector<Tile>> grid;
};

bool victory();

vector<Coordinate> getNeighbors(Coordinate c, int gridSize);

string tileRepresentation(const Tile& tile) {
    return tile.containsMine ? "*" : " ";
}

string tileRepresentation(const TileState& tileState) {
    if (!tileState.clicked) {
        return filledInSquare;
    }

    auto ret = tileState.value != 0 ? to_string(tileState.value) : " ";
    return ret;
}

template<typename T>
string join(const vector<T> v, char delim) {
    ostringstream ret;
    for (int i = 0; i < v.size(); i++) {
        ret << v[i];
        if (i != v.size() - 1) {
            ret << delim;
        }
    }
    return ret.str();
}


void clickLocation(Coordinate c, const vector<vector<Tile>>& grid, vector<vector<TileState>>& gridState);

ostringstream printGrid(const vector<vector<Tile>>& grid) {
    ostringstream ret;
    for (auto row : grid) {
        vector<string> tileRepresentations;
        // tileRepresentations.resize(row.size());
        transform(begin(row), end(row), back_inserter(tileRepresentations), [](Tile t) {
            return tileRepresentation(t);
        });
        ret << join(tileRepresentations, '|');
        ret << "\n";
    }
    return ret;
}

string printGridState(const vector<vector<TileState>>& gridState) {
    ostringstream a;
    for (auto row : gridState) {
        vector<string> tileRepresentations;
        // tileRepresentations.resize(row.size());
        transform(begin(row), end(row), back_inserter(tileRepresentations), [](TileState t) {
            return tileRepresentation(t);
        });

        a << join(tileRepresentations, '|');
        a << "\n";
        // a << join(vector<string>(row.size(), "_"), ' ') << "\n";
    }
    return a.str();
}



vector<vector<TileState>> constructDefaultGridState(const int size) {
    return vector<vector<TileState>>(size, vector<TileState>(size));
}

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

    auto gridState = constructDefaultGridState(5);

    auto repr = printGrid(grid);
    cout << repr.str() << endl;

    auto neighbors = getNeighbors(Coordinate(2,2), grid.size());
    for (auto neighbor : neighbors) {
        cout << "(" << neighbor.x << ", " << neighbor.y << ")" << " ";
    }
    cout << endl;

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
        string command("sup");
        // getline(cin, command);

        int x; int y;
        cin >> x >> y;

        if (command == "exit") {
            cout << filledInSquare << endl;
            return 0;
        }

        try {
            clickLocation(Coordinate(x, y), grid, gridState);
        } catch (exception e) {
            cout << "yo" << endl;
            cout << e.what() << endl;
            continue;
        } catch (...) {
            // cout << e.what() << endl;
            cout << "The rest" << endl;
            continue;
        }

        cout << "Your command was: " << x << ", " << y << endl;
        cout << printGridState(gridState) << endl;
    }
}

bool victory() {
    return false;
}

bool inBounds(Coordinate c, int gridSize) {
    bool xInBounds = c.x >= 0 && c.x < gridSize;
    bool yInBounds = c.y >= 0 && c.y < gridSize;

    return xInBounds && yInBounds;
}

vector<Coordinate> getNeighbors(Coordinate c, int gridSize) {
    // go in a circle around the middling square
    // start at top row 
    vector<Coordinate> ret;
    for (int i = 0; i < 2; i++) {
        Coordinate topLeft(c.x - 1, c.y - 1);
        Coordinate bottomLeft(c.x + 1, c.y - 1);
        Coordinate topRight(c.x - 1, c.y + 1);
        Coordinate bottomRight(c.x +1, c.y + 1);

        topLeft.y += i;
        topRight.x += i;
        bottomRight.y -= i;
        bottomLeft.x -= i;

        vector<Coordinate> all{topLeft, bottomLeft, topRight, bottomRight};
        for (auto coord : all) {
            if (inBounds(coord, gridSize)) {
                ret.push_back(coord);
            }
        }
    }

    return ret;
}

int getValueFromSurroundingSquares(Coordinate c, const vector<vector<Tile>>& grid) {
    auto neighbors = getNeighbors(c, grid.size());
    int sum = 0;
    for (const auto& v: neighbors)  if (grid[v.x][v.y].containsMine) sum += 1;
    return sum;
}

void clickLocation(Coordinate c, const vector<vector<Tile>>& grid, vector<vector<TileState>>& gridState) {
    auto x = c.x;
    auto y = c.y;
    if (gridState[x][y].clicked) {
        // cout << "Tile was already clicked" << endl;
        throw "Tile was already clicked";
    }

    // flood fill all the tiles around it
    if (grid[x][y].containsMine) {
        cout << "You lost!" << endl;
        exit(1);
        // end the game
    }

    gridState[x][y].clicked = true;
    gridState[x][y].value = getValueFromSurroundingSquares(c, grid);

    if (gridState[x][y].value == 0) {
        auto neighbors = getNeighbors(c, grid.size());
        for (const auto& neighbor : neighbors) {
            try {
                clickLocation(Coordinate(neighbor.x, neighbor.y), grid, gridState);
            } catch (...) {
                
            }
        }
    }
}



void test_clickLocation() {
    // grid
}


// we also have a game loop that takes state and event, and given the event, does the approprait ething to the sate
// function(state& state) // seems low key fluxxed up but maybe it'll work, makes it mutable only in the reducer?

// generate grid
// has to generate mines as well

// click on place -> reveal squares around if no mines
// put number of mines in surrounding area if there are squares

// get victory condition (all mines revealed, could be expensive to generate)