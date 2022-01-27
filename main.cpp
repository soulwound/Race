#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <sstream>
using namespace std;

int RndNmb(int min, int max)// функция для рандом числа вроде..
{
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}
class Car
{    
public:
    Car(int x)
    {
        car_y = -67 - RndNmb(0, 500);
        car_x = x;
        car_ms = RndNmb(1, 5);
        m_car = new sf::RectangleShape(sf::Vector2f(65, 67));
        m_car_texture.loadFromFile("sprites/cars.png");
        m_car->setTexture(&m_car_texture);
        m_car->setTextureRect(sf::IntRect(0, 1 + 67 * RndNmb(0, 2), 65, 67));
        m_car->setPosition(car_x, car_y);
    }
    ~Car()
    {
        delete m_car;
    }
    sf::RectangleShape* Get() { return m_car; }
    int GetY()
    {
        return m_car->getPosition().y;
    }
    int GetX()
    {
        return m_car->getPosition().x;
    }
    sf::FloatRect GetBounds()// для столконовений
    {
        return m_car->getGlobalBounds();
    }
    void Move(int x)
    {
        if (car_y < 1060 )
        {
            m_car->move(0, car_ms);
            car_y = m_car->getPosition().y;
        }
        else 
        {
            car_y = -67 - RndNmb(0, 200);
            m_car->setTextureRect(sf::IntRect(0, 1 + 67 * RndNmb(0, 2), 65, 67));
            m_car->setPosition(car_x, car_y);
        }
    }
    void Stop()
    {
        car_ms = 0;
    }
private:
        int car_x, car_y;
        float car_ms;
        sf::RectangleShape* m_car;
        sf::Texture m_car_texture;
};

int main() 
{
    sf::Font font;
    font.loadFromFile("fonts/GoingUnder.otf");
    sf::Text score("", font, 50);
    score.setFillColor(sf::Color::Red);
    int j = 0;
    const int car_count = 4;
    int y_bg = -2880;
    sf::RenderWindow window(sf::VideoMode(540, 960), "Test");//создаю окно
    window.setFramerateLimit(60);// лимит fps
    sf::Texture background_texture;
    sf::Sprite background_sprite; 
    background_texture.loadFromFile("sprites/background.png");
    background_sprite.setTexture(background_texture);
    background_sprite.setPosition(0, y_bg);
    sf::Texture main_car_texture;
    int score_count = 0;
    sf::Sprite main_car_sprite;
    main_car_texture.loadFromFile("sprites/main_car.png");
    main_car_sprite.setTexture(main_car_texture);
    main_car_sprite.setPosition(270, 890);
    main_car_sprite.setTextureRect(sf::IntRect(98, 321, 60, 65));
    int background_ms = 10;
    int move_speed = 3  ;
    int cars_kd = 0;
    float CurrentFrame = 0;
    int mas[4] = { 113, 189, 273, 357 };
    sf::Clock clock;
    std::vector<Car*> cars;
    for (int i = 0; i < car_count; i++)
    {
        cars.push_back(new Car(mas[i]));// создание машин
    }
    sf::FloatRect car_bounds;
    while (window.isOpen()) 
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 800;
        window.clear();
        if (background_sprite.getPosition().y < 0)
        {
            y_bg += background_ms;
            background_sprite.setPosition(0, y_bg);
        }
        else
        {
            y_bg = -2880;
            background_sprite.setPosition(0, y_bg);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && main_car_sprite.getPosition().x < 388)//движение вправо
        {
            main_car_sprite.move(move_speed, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && main_car_sprite.getPosition().x > 87)//движение вправо
        {
            main_car_sprite.move(-move_speed, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && main_car_sprite.getPosition().y > 0)//движение вправо
        {
            main_car_sprite.move(0, -move_speed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && main_car_sprite.getPosition().y < 893)//движение вправо
        {
            main_car_sprite.move(0, move_speed);
        }
        sf::Event close;
        while (window.pollEvent(close))//закрытие окна
        {
            if (close.type == sf::Event::Closed)
                window.close();
        }
        window.draw(background_sprite);
        window.draw(main_car_sprite);//прорисовка машины
        for (const auto& car_sprite : cars)
        {
            window.draw(*car_sprite->Get());
        }
        for (const auto& car_sprite : cars)
        {
            car_sprite->Move(mas[1]);
        }
        std::ostringstream score_string;
        score_string << score_count;
        score.setString("SCORE: " + score_string.str());
        score.setPosition(0, 0);
        window.draw(score);
        car_bounds = main_car_sprite.getGlobalBounds();
        for (const auto& car_sprite : cars)
            if (car_bounds.intersects(car_sprite->GetBounds()))
            {
                for (const auto& car_sprite : cars)
                {
                    car_sprite->Stop();
                }
                main_car_sprite.setTextureRect(sf::IntRect(98, 192, 60, 65));
                background_ms = 0;
                move_speed = 0;
            }
        if (move_speed != 0)
            score_count += time / 20;
        window.display();
    }
    return 0;
}