#include <iostream>
#include <string>
#include <vector>
#include "Item.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace std;

class MovingSprite
{
private:
    sf::Sprite *sprite;
    sf::Texture *texture;
    sf::IntRect *rect;
    int width;
    int height;
    int image_width;
    int image_height;

public:
    MovingSprite(sf::Sprite *sprite, sf::Texture *texture, sf::IntRect *rect, int image_width, int image_height)
    {
        this->texture = texture;
        this->rect = rect;
        sprite = new sf::Sprite(*texture, *rect);
        this->image_width = image_width;
        this->image_height = image_height;
        this->width = rect->width;
        this->height = rect->height;
    }
    virtual ~MovingSprite()
    {
        delete sprite;
        delete texture;
        delete rect;
    }
    void nextSprite()
    {
        if (rect->left + width >= image_width)
        {
            rect->left = width;
        }
        else
        {
            rect->left = rect->left + width;
        }
        sprite->setTextureRect(*rect);
    }
    void draw(sf::RenderWindow *window)
    {
        window->draw(*sprite);
    }
    sf::Sprite *getSprite()
    {
        return sprite;
    }
    void setPosition(int x, int y)
    {
        sprite->setPosition(x, y);
    }
};

class Default_Sprites
{
public:
    static MovingSprite *getPlayerSprite()
    {
        //sf::Sprite sprite, sf::Texture texture, sf::IntRect *rect
        sf::Texture texture;
        texture.loadFromFile("../Images/FriskSprite.png");
        sf::IntRect rect(0, 0, 19, 29);
        sf::Sprite spr(texture, rect);
        MovingSprite *temp = new MovingSprite(&spr, &texture, &rect, 76, 87);
        return temp;
    }
};

class HandWeapon : public Item
{
private:
    string name;
    int damage;
    int range;
    int speed;
    string element;
    MovingSprite *sprite;

public:
    HandWeapon(int x, int y, string name, int damage, int range, int speed, string element, MovingSprite *sprite) : Item(x, y)
    {
        this->name = name;
        this->damage = damage;
        this->range = range;
        this->speed = speed;
        this->element = element;
        this->sprite = sprite;
    }
    virtual ~HandWeapon()
    {
        delete sprite;
    }
    int getDamage()
    {
        return damage;
    }
    int getRange()
    {
        return range;
    }
    int getSpeed()
    {
        return speed;
    }
    string getElement()
    {
        return element;
    }
    string getName()
    {
        return name;
    }
};

class Projectile : public HandWeapon
{

private:
public:
    Projectile(int x, int y, string name, int damage, int range, int speed, string element, MovingSprite *sprite) : HandWeapon(x, y, name, damage, 2, 2, element, sprite)
    {
    }
};

class Bow : public Item
{
private:
    int multiplier;
    string name;

public:
    Bow(int x, int y, int multiplier, string name) : Item(x, y)
    {
        this->multiplier = multiplier;
        this->name = name;
    }
};

class Sword : public HandWeapon
{
public:
    Sword(int x, int y, string name, int damage, string element, MovingSprite *sprite) : HandWeapon(x, y, name, damage, 2, 2, element, sprite)
    {
    }
};

class Spear : public HandWeapon
{
public:
    Spear(int x, int y, string name, int damage, string element, MovingSprite *sprite) : HandWeapon(x, y, name, damage, 4, 3, element, sprite)
    {
    }
};

class Axe : public HandWeapon
{
public:
    Axe(int x, int y, string name, int damage, string element, MovingSprite *sprite) : HandWeapon(x, y, name, damage, 2, 1, element, sprite)
    {
    }
};

class Entity : public Item
{
private:
    int dx;
    int dy;
    int orientation; //1– up, 2– right, 3–down, 4–left
    int health;
    MovingSprite *sprite;

public:
    Entity(int x, int y, int orientation, int dx, int dy, int health, MovingSprite *sprite) : Item(x, y)
    {
        this->orientation = orientation;
        this->dy = dy;
        this->dx = dx;
        this->health = health;
        this->sprite = sprite;
    }
    virtual ~Entity()
    {
    }

    int getDx()
    {
        return dx;
    }
    int getDy()
    {
        return dy;
    }
    void update()
    {
        setX(getX() + dx);
        setY(getY() + dy);
    }
    int getOrientation()
    {
        return orientation;
    }
    sf::Sprite *getSprite()
    {
        return sprite->getSprite();
    }
    virtual void draw(sf::RenderWindow *window)
    {
        window->draw(*getSprite());
    }
};

class Player : public Entity
{

private:
    int MAX_INVENTORY_SIZE;
    vector<HandWeapon *> weapons;
    HandWeapon *weapon;
    vector<Bow *> bows;
    Bow *bow;

public:
    //    Enemy(int x, int y, int orientation, int dx, int dy, int health) : Entity(x, y, orientation, dx, dy, health)

    Player(int x, int y, int orientation, int dx, int dy, int health) : Entity(x, y, orientation, dx, dy, health, Default_Sprites::getPlayerSprite()) //int x, int y, int orientation, int dx, int dy, int health
    {
        MAX_INVENTORY_SIZE = 10;
        weapon = nullptr;
        bow = nullptr;
    }
    virtual ~Player()
    {
        delete weapon;
        delete bow;
    }
    bool addWeapon(HandWeapon *w)
    {
        if (weapons.size() < MAX_INVENTORY_SIZE)
        {
            weapons.push_back(w);
            return true;
        }
        return false;
    }
    HandWeapon *removeWeapon(int i)
    {
        if (weapons.size() == 1)
        {
            cout << "cannot remove only weapon" << endl;
            return nullptr;
        }
        if (i >= weapons.size())
        {
            cout << "WTF" << endl;
            return nullptr;
        }
        if (weapons[i] != nullptr)
        {
            return *weapons.erase(weapons.begin() + i);
        }
        else
        {
            return nullptr;
        }
    }
    //change weapon in hand to that of in slot i
    void changeHandWeaponHand(int i)
    {
        if (i >= weapons.size())
        {
            return;
        }
        else if (weapons[i] != nullptr)
        {
            weapon = weapons[i];
        }
    }
    HandWeapon *getWeapon(int i)
    {
        return weapons[i];
    }
    bool addBow(Bow *w)
    {
        if (bows.size() >= MAX_INVENTORY_SIZE)
        {
            bows.push_back(w);
            return true;
        }
        return false;
    }
    Bow *removeBow(int i)
    {
        if (i >= bows.size())
        {
            cout << "WTF" << endl;
            return nullptr;
        }
        else if (bows[i] != nullptr)
        {
            Bow *done = bows[i];
            bows.erase(bows.begin() + i);
            delete done;
        }
        else
        {
            return nullptr;
        }
        return nullptr;
    }
    void changeBowHand(int i)
    {
        if (i >= bows.size())
        {
            cout << "WTF" << endl;
            return;
        }
        if (bows[i] != NULL)
        {
            bow = bows[i];
        }
    }
    Bow *getBow(int i)
    {
        return bows[i];
    }
    void keyboard(sf::RenderWindow *window)
    {
        sf::Event event;
        window->pollEvent(event);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            this->x--;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            this->x++;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            this->y++;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            this->x--;
        }
    }
    void update(sf::RenderWindow *window)
    {
        keyboard(window);
    }
};

class Enemy : public Entity
{
private:
    bool seen;

public:
    Enemy(int x, int y, int orientation, int dx, int dy, int health, MovingSprite *sprite) : Entity(x, y, orientation, dx, dy, health, sprite)
    {
        seen = false;
    }
    ~Enemy()
    {
    }
};

class Level
{
private:
    Player *player;
    int number;

public:
    Level()
    {
        //int x, int y, int orientation, int dx, int dy, int health
        //int x, int y, int orientation, int dx, int dy, int health
        player = new Player(10, 10, 1, 1, 2, 10);
    }
    void draw(sf::RenderWindow *window)
    {
        player->draw(window);
    }
    virtual ~Level()
    {
        delete player;
    }
    Player *getPlayer()
    {
        return player;
    }
};

class Game
{

private:
    vector<Level *> levels;
    bool isOver;
    Level *currentLevel;

public:
    Game()
    {
        //int x, int y, int orientation, int dx, int dy, int health, MovingSprite *sprite
        currentLevel = new Level();
    }
    virtual ~Game()
    {
        delete currentLevel;
    }
    Player *getPlayer()
    {
        return currentLevel->getPlayer();
    }
    void draw(sf::RenderWindow *window)
    {
        //cout << getPlayer()->getX() << endl;

        currentLevel->getPlayer()->draw(window);
        currentLevel->draw(window);
    }
};

class GameMechanics
{
private:
public:
    static bool isAboutToCollide(Item item1, Item item2, Level *level)
    {
        return false;
    }

    static bool projectileIsAboutToCollide(Projectile projectile, Item item2, Level level)
    {
        return false;
    }
};

class Default
{
public:
    static HandWeapon *getDefaultWeapon()
    {
        return nullptr;
    }
    static MovingSprite *getDefaultsprite()
    {
        return nullptr;
    }
    static Bow *getDefaultBow()
    {
        return nullptr;
    }
};

int main()
{

    // create the window
    Game *game = new Game();
    //int x, int y, int orientation, int dx, int dy, int health
    Player *player = new Player(10, 10, 1, 5, 5, 100);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 800), "SFMLGameStarter", sf::Style::Default, settings);

    // run the program as long as the window is open
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        game->draw(&window);

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Delete))
            {
                window.close();
            }

            window.clear(sf::Color::White);
            window.display();
        }
    }
    return 0;
}