#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include <vector>

const int square_size = 10;          // int, not float — it is a grid step
const int Width  = 1600;
const int Height = 900;
const float cursor_radius = 10.f;
const int row_length = Width  / square_size;   // 90 columns
const int col_length = Height / square_size;   // 60 rows
const int area_multiplier = 3;

struct pos {
    int x;
    int y;
};


inline int index_of(int x, int y) {
    return (x / square_size) * col_length + (y / square_size);
}



std::vector<int> matrix_check(pos block) {
    std::vector<int> neighbours;
    neighbours.reserve(8);
    for (int di = -square_size ; di <= square_size; di += square_size) {
        for (int dj = -square_size; dj <= square_size; dj += square_size) {
            if (di == 0 && dj == 0) continue;          // skip the cell itself
            const int i = (block.x + di + Width)  % Width;
            const int j = (block.y + dj + Height) % Height;
            neighbours.push_back(index_of(i, j));
        }
    }
    return neighbours;
}






class block {
public:
    float x;
    float y;
    int state;
    sf::RectangleShape rect;

    block(float x_, float y_, int state_)
        : x(x_), y(y_), state(state_)
    {
        rect.setSize(sf::Vector2f(square_size , square_size ));
        //rect.setOutlineThickness(0.f);
        rect.setPosition({x, y});
    }


    void check(const std::vector<block>& things, std::vector<int>& to_update) const {
        const int self = index_of(static_cast<int>(x), static_cast<int>(y));

        int alive = 0;
        for (int n : matrix_check(pos{static_cast<int>(x), static_cast<int>(y)}))
            alive += things[n].state;

        if (state == 1)
            to_update[self] = (alive == 2 || alive == 3) ? 1 : 0;
        else
            to_update[self] = (alive == 3) ? 1 : 0;
    }

    void draw(sf::RenderWindow& window) {
        rect.setFillColor(state == 1 ? sf::Color(0, 255, 0) : sf::Color(0, 0, 60));
        window.draw(rect);
    }
};

std::vector<int> area_revive(const int area,int x, int y) {
    std::vector<int> neighbours;
    neighbours.reserve((3+2*(area-1))*(3+2*(area-1)));
    for (int di = -square_size *area; di <= square_size*area; di += square_size) {
        for (int dj = -square_size*area; dj <= square_size*area; dj += square_size) {
            if (di == 0 && dj == 0) continue;          // skip the cell itself
            const int i = (x + di + Width)  % Width;
            const int j = (y + dj + Height) % Height;
            neighbours.push_back(index_of(i, j));
        }
    }
    return neighbours;
}




class cursor{

public:
    float x;
    float y;
    float radius;
    sf::CircleShape shape;
    sf::Color color;

    cursor(float x_ , float y_, float radius_,sf::Color color_)
        : x(x_), y(y_), radius(radius_), color(color_)
    {
        shape.setRadius(10.f);
        shape.setOrigin({radius,radius});
        shape.setFillColor(color);
        shape.setPosition({x,y});
    }

    void draw(sf::RenderWindow& window) {
        shape.setPosition({x,y});
        window.draw(shape);
    }
    void movement(sf::RenderWindow& window) {
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        x = mouse.x;
        y = mouse.y;
    }
    void revive(std::vector<int>& state) {
        // area

        std::vector<int> dude = area_revive(area_multiplier,x,y);

        for (int item: dude) {
            state[item] = 1;
        }




    }


};








int main() {
    std::cout << row_length << "," << col_length << "\n";
    std::cout << "random [y,n]: ";
    char ans;
    std::cin >> ans;
    bool rand = false;
    if (ans == 'y') {
        rand = true;
    }else if (ans == 'n') {
        std::cout << "ok";
    } else {
        std::cout << "unknown";
    }




    sf::RenderWindow window(
        sf::VideoMode({static_cast<unsigned>(Width), static_cast<unsigned>(Height)}),
        "My window");
    window.setFramerateLimit(60);


    // rendor mouse
    cursor m(150.f,150.f,cursor_radius,sf::Color::Red);




    std::mt19937 rng(std::random_device{}());
    std::bernoulli_distribution coin(0.30);

    std::vector<int>   state;
    std::vector<block> all_blocks;
    state.reserve(row_length * col_length);
    all_blocks.reserve(row_length * col_length);

    for (int x_ = 0; x_ < Width; x_ += square_size) {
        for (int y_ = 0; y_ < Height; y_ += square_size) {
            int alive = 0;
            if (rand == true ) {
                alive = coin(rng) ? 1 : 0;
            }

            state.push_back(alive);
            all_blocks.emplace_back(static_cast<float>(x_), static_cast<float>(y_), alive);
        }
    }

    std::vector<int> next_state(state.size());

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        for (std::size_t i = 0; i < state.size(); ++i)
            all_blocks[i].state = state[i];

        m.movement(window);




        // Read from this generation, write to the next, then swap.
        for (const auto& item : all_blocks)
            item.check(all_blocks, next_state);
        m.revive(next_state);
        state.swap(next_state);
        //rendor
        window.clear(sf::Color::Black);

        for (auto& item : all_blocks)
            item.draw(window);
        m.draw(window);
        window.display();
    }
}