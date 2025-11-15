#include <iostream>
#include <string>
#include "Random.h"

class Creature {
public:
    std::string name{};
    char symbol{};
    int health{};
    int DPA{};
    int defense{};
    int gold{};

    Creature(std::string name, char symbol, int health, int DPA, int defense, int gold) : name{name}, symbol{symbol}, health{health}, DPA{DPA}, defense{defense}, gold{gold} {}

    std::string getName() {return name;}
    char getSymbol() {return symbol;}
    int getHealth() {return health;}
    int getDPA() {return DPA;}
    int getDefense() {return defense;}
    int getGold() {return gold;}

    void reduceHealth(int n) {
        health -= n;
    }

    bool isDead() {return health <= 0;}

    void addGold(int n) {
        gold += n;
    }
};

class Player : public Creature {
public:
    int level{};
    int flee_rate{};
    Player (std::string name, char symbol = '@', int health = 10, int DPA = 3, int defense = 1, int gold = 10, int level = 1, int flee_rate = 7) : Creature{name, symbol, health, DPA, defense, gold}, level{level}, flee_rate {flee_rate} {}

    void levelUp() {
        level ++;
        defense ++;
        DPA ++;
    }

    int getLevel() {return level;}
    bool hasWon() {return level >= 20;}
};

class Monster : public Creature {
public:
    enum Type {
        dragon,
        orc,
        slime,
        max_types
    };
private:
    static inline Creature monsterData[] {
    // Creature monsterData[] {
        Creature { "dragon", 'D', 20, 5, 10, 100 },
        Creature { "orc", 'o', 4, 3, 3, 25 },
        Creature { "slime", 's', 1, 2, 1, 10 }
    };
public:
    Monster(Type type) : Creature{ monsterData[type] }{}
    static Monster getRandomMonster()
    {
        int num{ Random::get(0, max_types - 1) };
        return Monster{ static_cast<Type>(num) };
    }
};

class Potion : public Creature {
public:

    enum Type {
        heal,
        strength,
        poison,
        max_types,
    };
    enum Size {
        small,
        medium,
        large,
        max_sizes,
    };

    Potion (Type type, Size size) : Creature{ potionData[type][size] }{};

private:
    static inline Creature potionData[max_types][max_sizes] {
        {       // Health potion
            Creature { "heal", 's', 3, 0, 0, 0 },
            Creature { "heal", 'm', 7, 0, 0, 0 },
            Creature { "heal", 'l', 10, 0, 0, 0 }
        },

        {       // strength potion
            Creature { "strength", 's', 0, 1, 1, 0 },
            Creature { "strength", 'm', 0, 3, 2, 0 },
            Creature { "strength", 'l', 0, 5, 3, 0 }
        },

        {       // poison potion
            Creature { "poison", 's', -1, 0, 0, 0 },
            Creature { "poison", 'm', -3, 0, 0, 0 },
            Creature { "poison", 'l', -10, 0, 0, 0 }
        },

    };
public:
    static Potion getRandomPotion() {
        int type_num{ Random::get(0, max_types - 1) };
        int size_num{ Random::get(0, max_sizes - 1) };
        return Potion{ static_cast<Type>(type_num), static_cast<Size>(size_num) };
    }

};

class Equipment : public Creature {
    public:

    enum Type {
        // helmet,
        armor,
        sword,
        boots,
        max_types,
    };
    enum Quality {
        old,
        normal,
        rare,
        epic,
        max_quality,
    };

    Equipment (Type type, Quality quality) : Creature{ equipmentData[type][quality] }{};

    private:

    static inline Creature equipmentData[max_types][max_quality] {
        // {       // helmet
        //     Creature { "helmet", 'o', 2, 0, 0, 5 },
        //     Creature { "helmet", 'n', 4, 0, 0, 10 },
        //     Creature { "helmet", 'r', 6, 0, 0, 15 },
        //     Creature { "helmet", 'e', 10, 0, 0, 20 }
        // },

        {       // armor
            Creature { "armor", 'o', 0, 0, 3, 5 },
            Creature { "armor", 'n', 0, 0, 5, 10 },
            Creature { "armor", 'r', 0, 0, 7, 15 },
            Creature { "armor", 'e', 0, 0, 10, 20 }
        },

        {       // sword
            Creature { "sword", 'o', 0, 3, 0, 5 },
            Creature { "sword", 'n', 0, 5, 0, 7 },
            Creature { "sword", 'r', 0, 7, 0, 10 },
            Creature { "sword", 'e', 0, 10, 0, 15 }
        },
        {       // boots
            Creature { "boots", 'o', 0, 0, 0, 3 },
            Creature { "boots", 'n', 0, 0, 0, 5 },
            Creature { "boots", 'r', 0, 0, 0, 7 },
            Creature { "boots", 'e', 0, 0, 0, 10 }
        },

    };
    public:
    static Equipment getRandomEquipment() {
        int type_num{ Random::get(0, max_types - 1) };
        int size_num{ Random::get(0, max_quality - 1) };
        return Equipment{ static_cast<Type>(type_num), static_cast<Quality>(size_num) };
    }

};

void attackPlayer(Monster& monster, Player& player) {
    if (monster.isDead()) {
        return;
    }
    else {
        int damage = (monster.DPA > player.defense) ? monster.DPA - player.defense : 0;
        player.health -= damage;
        std::cout << "The " << monster.getName() << " attacked you for " << damage << " damages." << std::endl;
        if (player.isDead()) {
            std::cout << "You died." << std::endl;
        }
        return;
    }
}

void attackMonster(Player& player, Monster& monster) {
    if (player.isDead()) {
        return;
    }
    else {
        int damage = (player.DPA > monster.defense) ? player.DPA - monster.defense : 0;
        monster.health -= damage;
        std::cout << "You attacked the " << monster.getName() <<  " for " << damage << " damages." << std::endl;
        if (monster.isDead()) {
            std::cout << "You killed the " << monster.getName() << "!" << std::endl;
            player.levelUp();
            player.addGold(monster.gold);
            std::cout << "You leveled up!" << std::endl << "You found " << monster.getGold() << " gold!" << std::endl;
            if (Random::get(1, 3) == 1) {
                std::cout << "You found a potion." << std::endl;
                std::cout << "Your level:" << player.getLevel() << std::endl;
                std::cout << "Your health:" << player.health << std::endl;
                std::cout << "Your DPA:" << player.DPA << std::endl;
                std::cout << "Do you want to drink it?" << std::endl;
                std::cout << "(Y)es or (N)o:" << std::endl;
                char drink_discard{};
                std::cin >> drink_discard;
                if (drink_discard == 'Y' or drink_discard == 'y') {
                    Potion potion {Potion::getRandomPotion()};
                    player.health += potion.health;
                    player.DPA += potion.DPA;
                    player.defense += potion.defense;
                    std::cout << "You drank a " << potion.name << "(" << potion.symbol << ")" << "potion" << std::endl;
                    if (potion.name == "strength"){
                        std::cout << "You've gained " << potion.DPA << " damages and " << potion.defense << " defenses."<< std::endl;
                    }
                    else if (potion.name == "heal") {
                        std::cout << "You've gained " << potion.health << " health." << std::endl;
                    }
                    else if (potion.name == "poison") {
                        std::cout << "You've lost " << -potion.health << " health." << std::endl;
                        if (player.isDead()) {
                            std::cout << "You died." << std::endl;
                        }
                    }

                }
            }
        }
        return;
    }
}

void fightMonster(Player& player) {
    Monster monster{Monster::getRandomMonster()};
    std::cout << "You have encountered a " << monster.getName() << std::endl;

    char fight_run{};

    while (!monster.isDead() && !player.isDead()) {
        std::cout <<"Your level:" << player.getLevel() << std::endl;
        std::cout << "Your health:" << player.health << std::endl;
        std::cout << "Your DPA:" << player.DPA << std::endl;
        std::cout << "Your defense:" << player.defense << std::endl;
        std::cout << "(R)un or (F)ight:";
        std::cin >> fight_run;
        if (fight_run == 'R' or fight_run == 'r') {
            if (Random::get(player.flee_rate, 11) == 10) {
                std::cout << "You successfully fled." << std::endl;
                return;
            }
            else {
                std::cout << "You failed to flee." << std::endl;
                attackPlayer(monster, player);
                continue;
            }
        }
        else if (fight_run == 'F' or fight_run == 'f') {
            attackMonster(player, monster);
            attackPlayer(monster, player);
            continue;
        }
    }
}

void encounterMerchant(Player& player) {
    std::cout << "You've encountered a merchant." << std::endl;
    Equipment equipment{Equipment::getRandomEquipment()};
    std::cout << "The merchant sells a " << equipment.name << "(" << equipment.symbol << ")." << std::endl;
    std::cout << "You have " << player.gold << " gold, do you want to buy(y/n)?" << std::endl;
    char buy_leave{};
    std::cin >> buy_leave;
    if (buy_leave == 'y' or buy_leave == 'Y') {
        if (player.gold >= equipment.gold) {
            std::cout << "You've bought " << equipment.name << "(" << equipment.symbol << ")." << std::endl;
            player.gold -= equipment.gold;
            player.health += equipment.health;
            player.DPA += equipment.DPA;
            player.defense += equipment.defense;
            if (equipment.name == "boots") {
                switch (equipment.symbol) {
                    case 'o':
                        player.flee_rate = 4;
                        break;
                    case 'n':
                        player.flee_rate = 6;
                        break;
                    case 'r':
                        player.flee_rate = 8;
                        break;
                    case 'e':
                        player.flee_rate = 10;
                        break;
                }
            }
        }
        else {
            std::cout << "You don't have enough gold" << std::endl;
        }

    }
    else {
        std::cout << "You've left the merchant." << std::endl;
    }
}

int main()
{
    std::string my_name;
    std::cout << "Please enter player name: ";
    std::cin >> my_name;
    std::cout << "Welcome " << my_name <<"." << std::endl;
    Player player(my_name);
    while (!player.isDead() && !player.hasWon()) {
        if (Random::get(1, 3) == 1){encounterMerchant(player);}
        fightMonster(player);
    }
    if (player.hasWon()) {
        std::cout << "You won." << std::endl;
    }


    return 0;
}