#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>



class Object
{
public:
    virtual void setPositionOutside(float x, float y)  = 0;
    virtual void update() = 0;
    virtual sf::FloatRect getGlobalBounds() const = 0; 
    virtual void draw(sf::RenderWindow& window) const = 0;
    virtual ~Object() = default;
    
};


class Boat : public sf::Sprite{
private:
    sf::Texture texture;
public:
    Boat() {
        if (!texture.loadFromFile("C:\\Users\\sashy\\source\\repos\\fishingsimo\\recurces\\boat.png"))
        {
            std::cerr << "Failed to load boat texture!" << std::endl;
            exit(EXIT_FAILURE);
        }

        setTexture(texture);
        setScale(0.2, 0.2);
        setPosition(0, 500);
    }
    void update()  {
        float speed = 0.5f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            move(-speed, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            move(speed, 0);
    }

};

class Hook : public sf::RectangleShape {
private:
    float hookY = 0;
public:
    Hook() : RectangleShape(sf::Vector2f(4, 50)) {
        setFillColor(sf::Color::Red);
        setPosition(400, 500);
    }
    void update(const sf::Vector2f& boatPos) {
        setPosition(boatPos.x + 20, boatPos.y - 50 + hookY);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) hookY -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) hookY += 1;
    }
    bool checkCollision(const sf::FloatRect& bounds) {
        return getGlobalBounds().intersects(bounds);
    }

};

class Fish : public Object {
private:
    float speed;
    sf::RectangleShape shape;
public:
    Fish() : shape(sf::RectangleShape(sf::Vector2f(40, 20))), speed(0.1f) {
        shape.setFillColor(sf::Color::Yellow);
        shape.setPosition(rand() % 200 + 600, rand() % 400 + 100);
    }
    void update() override {
        shape.move(-speed, 0);
        if (shape.getPosition().x < 50)
            shape.setPosition(800, shape.getPosition().y);
    }

    sf::FloatRect getGlobalBounds() const override {
        return shape.getGlobalBounds();
    }

    void draw(sf::RenderWindow& window) const override
    {
        window.draw(shape);
    }

    void setPositionOutside(float x, float y) override{
        shape.setPosition(x, y);
    }
};

class Aqualangist : public Object {
private: 
    float speed;
    sf::Texture texture;
    sf::Sprite sprite;
public:
    Aqualangist() : speed(0.2f)
    {
        if(!texture.loadFromFile("C:\\Users\\sashy\\source\\repos\\fishingsimo\\recurces\\diver.png"))
        {
            std::cerr << "Failed to load boat texture!" << std::endl;
            exit(EXIT_FAILURE);
        }
        sprite.setTexture(texture);
        sprite.setScale(-0.1f, 0.1f);
        sprite.setPosition(800, rand() % 500);
    }
    void update() override {
        sprite.move(-speed, 0);
        if (sprite.getPosition().x < -100)
            sprite.setPosition(800, sprite.getPosition().y);
    }
    
    sf::FloatRect getGlobalBounds() const override {
        return sprite.getGlobalBounds();
    }

    void draw(sf::RenderWindow& window) const override {
        window.draw(sprite);
    }

    void setPositionOutside(float x, float y) override {
        sprite.setPosition(x, y);
    }
};

class Mine : public Object {
private:
    float speed;
    sf::CircleShape shape;
public:
    Mine() : shape(sf::CircleShape(7)), speed(0.2f) {
        shape.setFillColor(sf::Color::Black);
        shape.setPosition(800, rand() % 600);
    }
    void update() override {
        shape.move(-speed, 0);
        if (shape.getPosition().x < -100)
            shape.setPosition(800, shape.getPosition().y);
    }
    sf::FloatRect getGlobalBounds() const override {
        return shape.getGlobalBounds();
    }
    void draw(sf::RenderWindow &window) const override{
        window.draw(shape);
    }
    
    void setPositionOutside(float x, float y) override {
        shape.setPosition(x, y);
    }
};

class Game {
private:
    int score = 0;
    sf::RenderWindow window;
    sf::VideoMode videoMode;
    void initWindow()
    {
        this->videoMode.height = 600;
        this->videoMode.width = 800;
        window.create(this->videoMode, "Fishing Simo", sf::Style::Titlebar | sf::Style::Close);
    }
    sf::Event event;
    sf::Font font;
    sf::Text scoreText;
    sf::Clock fishSpawnClock, mineSpawnClock, diverSpawnClock;
    sf::Clock gameClock;
    sf::SoundBuffer bufferCatch, bufferWin, bufferOuch;
    sf::Sound soundCatch, soundWin, soundOuch;
public:
	
	bool gameOver = false;
    Game() {
        srand(time(0));

        if (!font.loadFromFile("C:\\Users\\sashy\\source\\repos\\fishingsimo\\recurces\\timesnewromanps_italicmt.ttf")) {
            std::cerr << "Failed to load font!" << std::endl;
            exit(EXIT_FAILURE);
        }

        if (!bufferCatch.loadFromFile("C:\\Users\\sashy\\source\\repos\\fishingsimo\\recurces\\catch.wav")) {
            std::cerr << "Failed to load catch.wav!" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (!bufferOuch.loadFromFile("C:\\Users\\sashy\\source\\repos\\fishingsimo\\recurces\\over.wav")) {
            std::cerr << "Failed to load over.wav!" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (!bufferWin.loadFromFile("C:\\Users\\sashy\\source\\repos\\fishingsimo\\recurces\\win.wav")) {
            std::cerr << "Failed to load win.wav!" << std::endl;
            exit(EXIT_FAILURE);
        }

        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(10, 10);

        soundCatch.setBuffer(bufferCatch);
        soundWin.setBuffer(bufferWin);
        soundOuch.setBuffer(bufferOuch);
    }
    int getScore() const { return score; }
    bool showEndScreen(const std::string& message) {
        sf::Text text(message, font, 24);
        text.setFillColor(sf::Color::White);
        text.setPosition(100, 100);

        sf::RectangleShape background(sf::Vector2f(800, 600));
        background.setFillColor(sf::Color(0, 0, 100, 200));

        while (true) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            window.clear();
            window.draw(background);
            window.draw(text);
            window.display();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                return true; 
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
                return false;
            }
        }
    }

    void showTextScreen(const std::string& text) {
        sf::Text message(text, font, 24);
        message.setFillColor(sf::Color::White);
        message.setPosition(50, 100);

        sf::RectangleShape background(sf::Vector2f(800, 600));
        background.setFillColor(sf::Color(0, 0, 100, 200));

        while (true) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
                if (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed)
                    return;
            }

            window.clear();
            window.draw(background);
            window.draw(message);
            window.display();
        }
    }

    void run() {
        this->initWindow();

        while (window.isOpen()) {
            showTextScreen("Fishing Simo\nGoal : collect 50 points!\nControls:\n<- -> move boat\nstraight up/down - move hook\nPress Any Key to start a game");

            while (window.isOpen()) {
                Boat boat;
                Hook hook;
                std::vector<std::unique_ptr<Object>> objects;
                const float spawnFishTime = 2.0f;
                const float spawnMineTime = 6.0f;
                const float spawnDiversTime = 6.0f;
                const float gameTime = 30.0f;

                fishSpawnClock.restart();
                mineSpawnClock.restart();
                diverSpawnClock.restart();
                gameClock.restart();
                score = 0;

                bool restart = false;

                while (window.isOpen()) {
                    sf::Event event;
                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed)
                            window.close();
                    }

                    if (score >= 50) {
                        soundWin.play();
                        showEndScreen("You Win!\nPoints: " + std::to_string(score) + "\nClick 'R' to restart of 'Esc' to exit!");
                        restart = true;
                        break;
                    }

                    if (gameClock.getElapsedTime().asSeconds() > gameTime) {
                        showEndScreen("You Lose!\nPoints: " + std::to_string(score) + "\nClick 'R' to restart of 'Esc' to exit!");
                        restart = true;
                        break;
                    }

                    boat.update();
                    hook.update(boat.getPosition());

                    if (fishSpawnClock.getElapsedTime().asSeconds() > spawnFishTime) {
                        objects.push_back(std::make_unique<Fish>());
                        fishSpawnClock.restart();
                    }
                    if (mineSpawnClock.getElapsedTime().asSeconds() > spawnMineTime) {
                        objects.push_back(std::make_unique<Mine>());
                        mineSpawnClock.restart();
                    }
                    if (diverSpawnClock.getElapsedTime().asSeconds() > spawnDiversTime) {
                        objects.push_back(std::make_unique<Aqualangist>());
                        diverSpawnClock.restart();
                    }
                    for (auto& obj : objects) {
                        obj->update();
                        if (hook.checkCollision(obj->getGlobalBounds())) {
                            Fish* fish = dynamic_cast<Fish*>(obj.get());
                            if (fish) {
                                fish->setPositionOutside(-100, -100);
                                score += 10;
                                soundCatch.play();
                            }
                            else {
                                soundOuch.play();
                                showEndScreen("You Lose!\nPoints: " + std::to_string(score) + "\nClick 'R' to restart of 'Esc' to exit!");
                                restart = true;
                                break;
                            }
                        }
                    }
                    if (restart) break;

                    window.clear(sf::Color::Blue);
                    window.draw(boat);
                    window.draw(hook);
                    for (const auto& obj : objects) obj->draw(window);
                    scoreText.setString("Points: " + std::to_string(score));
                    window.draw(scoreText);
                    window.display();
                }

                if (!restart)
                    break;
            }
        }
    }
    
};




int main() {
	Game game;
	game.run();
	return 0;
}