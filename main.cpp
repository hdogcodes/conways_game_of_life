#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include <algorithm>
float square_size = 10;
unsigned int Width = 900;
unsigned int Height = 600;

int row_length = Width/square_size;
int col_length = Height/square_size;




struct pos {
    int x;
    int y;
};


// matrix to check local area
std::vector<pos> matrix_check(pos block) {
    std::vector<pos> poses;
    for (int i = block.x - square_size; i <= block.x + square_size; i+= square_size) {
        for (int j = block.y - square_size; j <= block.y +square_size;j+= square_size) {
            if ((i <0 || j<0)||(i > Width|| j > Height)||(i==block.x&&j==block.y)) {
                continue;
            }
            pos temp(i,j);
            poses.push_back(temp);


        }
    }
    return poses;
}


class block {
    public:
    float x;
    float y;
    int state;
    sf::RectangleShape rect;
    block(float x_, float y_ , int state_)
        :x(x_),y(y_),state(state_)
    {
        rect.setSize(sf::Vector2f(square_size,square_size));
        rect.setFillColor(sf::Color(0,0,255));
        rect.setOutlineColor(sf::Color(0,0,0));
        rect.setOutlineThickness(5.f);
        rect.setPosition({x,y});

    }
    std::vector<int> check(std::vector<block> things,std::vector<int> to_update) {
        //check area around block
        //add into to change states
        int alive= 0;
        pos temp(x,y);
        //player index
        auto player = std::find_if(things.begin(), things.end(), [&](const block& b) {
                return b.x == x && b.y == y;
            });
        int player_index = std::distance(things.begin(),player);



        //using the function
        std::vector<pos> check_area = matrix_check(temp);
        for (auto item: check_area) {
            auto it = std::find_if(things.begin(), things.end(), [&](const block& b) {
                return b.x == item.x && b.y == item.y;
            });
            if (it->state == 1) {
                ++alive;
            }
        }
        if (state == 1 && alive <2) {
            to_update[player_index] = 0;
        }else if (state == 1 && alive > 3) {
            to_update[player_index] = 0;
        }else if (state ==0 && alive == 3) {
            to_update[player_index] = 1;
        }
        return to_update;

        /*std::cout << "start\n";
        for (auto item : check_area) {
            std::cout << item.x<< ","<< item.y << "\n";
        }
        std::cout<< "end\n";
        */



    }


    void draw(sf::RenderWindow &window) {
        if (state ==1) {
            rect.setFillColor(sf::Color(0,255,0));
        }
        window.draw(rect);
    }


};


int main() {
    std::cout << row_length<< ","<< col_length<< "\n";



    sf::RenderWindow window(sf::VideoMode({Width, Height}), "My window");
    std::vector<int> state;

    std::vector<pos> blocks;
    std::vector<block> all_blocks;

    //positions
    for (int x_ = 0 ; x_ < Width; x_ += square_size ) {
        for (int y_ = 0; y_ < Height; y_ += square_size){

            pos position(x_,y_);
            blocks.push_back(position);
        }
    }
    //rendoring into window
    for (const auto& item:blocks) {
        int rand = std::rand() % 2;
        state.push_back(rand);
        block temp(item.x,item.y,rand);
        all_blocks.push_back(temp);
    }

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        for (int i = 0; i< state.size();i++) {
            all_blocks[i].state = state[i];
        }
        //rendor
        window.clear(sf::Color::Black);

        for (auto item :all_blocks) {
            state = item.check(all_blocks, state);
            item.draw(window);
        }




        window.display();

    }
}