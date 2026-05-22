#include "Shop.h"
#include <iostream>
#include <limits>

static void ClearInput()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Shop::AskEnterShop(Gamemode& gamemode)
{
    char choice;
    while (true)
    {
        std::cout << "Would you like to enter the shop? (y/n): ";
        std::cin >> choice;
        ClearInput();

        if (choice == 'y' || choice == 'Y')
        {
            gamemode.SetPlayerState(PlayerState::Shop);
            return;
        }
        if (choice == 'n' || choice == 'N')
        {
            return;
        }
        std::cout << "Please enter y or n.\n";
    }
}