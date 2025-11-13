#include "SaveManager.h"

void SaveManager::SaveProgress(int currentLevel)
{
    // Open file for writing
    std::ofstream file("data/save.txt");

    if (file.is_open())
    {
        file << currentLevel;
        file.close();
    }
    else
    {
        std::cout << "Could not save game" << std::endl;
    }
}

int SaveManager::LoadProgress()
{
    // Open file for reading
    std::ifstream file("data/save.txt");
    int level = 0;

    if (file.is_open())
    {
        file >> level;
        file.close();
    }

    return level;
}