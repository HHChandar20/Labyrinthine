#pragma once
#include <string>
#include <fstream>
#include <iostream>

class SaveManager
{
public:
    // Save current level progress
    static void SaveProgress(int currentLevel);

    // Load saved level progress
    static int LoadProgress();
};