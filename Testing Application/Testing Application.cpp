#include <iostream>
#include "Inventory.h"
#include "windows.h"

const std::string tags[]{ "Weapon", "Chestplate", "Helmet", "Gloves", "Bow", "Boots" };

InventoryLib::BaseItem* RandomItem()
{
    std::string randomTag{ tags[int(rand() % (tags->size()))] };

    std::string randomName{};
    for (int i = 0; i < 4; i++)
    {
        randomName += char((rand() % 26) + 65);
    }

    std::string randomID{};
    for (int i = 1; i < 21; i++)
    {
        if (i % 7 == 0)
        {
            randomID += "-";
        }
        else
        {
            randomID += char((rand() % 26) + 65);
        }
    }

    int randomStackSize = rand() % 256;

    InventoryLib::BaseItem* retVal = new InventoryLib::BaseItem(randomName, randomTag, randomID, randomStackSize, rand() % (randomStackSize / 2) + randomStackSize / 2);
    return retVal;
}

int main()
{
    InventoryLib::BaseItem* item = nullptr;
    printf(item->IsValid() ? "true" : "false");

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, 6);
    printf("--- Creation and deletion of Inventory ---\n");
    SetConsoleTextAttribute(hConsole, 7);
    if(true)
    {
        InventoryLib::Inventory* temp = new InventoryLib::Inventory();
        delete temp;
        temp = nullptr;
        printf("\n");
        printf("\n");
    }

    SetConsoleTextAttribute(hConsole, 6);
    printf("--- Creation of inventory with slots and shrinking it ---\n");
    SetConsoleTextAttribute(hConsole, 7);
    if(true)
    {
        InventoryLib::Inventory* inv = new InventoryLib::Inventory(100);
        #ifdef _DEBUG
        printf("-- Start shrinking --");
        #endif
        inv->SetSlotCount(16);
        delete inv;
        printf("\n");

        inv = new InventoryLib::Inventory(1, 100.0, -1);
        delete inv;
        printf("\n");
        printf("\n");

        inv = new InventoryLib::Inventory(20, 100.0f);
        inv->SetSlotCount(24);
        printf("\n");
        inv->SetSlotCount(20);
        printf("\n");
        inv->SetSlotCount(100);
        printf("\n");

        delete inv;
        printf("\n");
        printf("\n");
    }

    SetConsoleTextAttribute(hConsole, 6);
    printf("--- Testing Sort by Name ---\n");
    SetConsoleTextAttribute(hConsole, 7);
    if(true)
    {
        InventoryLib::Inventory* inv = new InventoryLib::Inventory(100);
        inv = new InventoryLib::Inventory(100);
        printf("\n");

        //Testing Sort by name
        for (int i = 0; i < 5; i++)
        {
            InventoryLib::BaseItem* tempItem = RandomItem();
            inv->AddItem(tempItem);
            tempItem = new InventoryLib::BaseItem(*tempItem);
            tempItem->currentStack = 32;
            inv->AddItem(tempItem);
        }
        printf("\n");

        printf("--- UNSORTED ---\n");
        printf(inv->GetInventoryStructure(false).c_str());
        inv->SortByName();
        printf("\n");
        printf("--- SORTED BY NAME ---\n");
        printf(inv->GetInventoryStructure(false).c_str());
        printf("\n");
        printf("\n");
        printf("\n");
    }

    SetConsoleTextAttribute(hConsole, 6);
    printf("--- Testing Sort by Tag ---\n");
    SetConsoleTextAttribute(hConsole, 7);
    if (true)
    {
        InventoryLib::Inventory* inv = new InventoryLib::Inventory(100);
        printf("\n");

        //Testing Sort by name
        for (int i = 0; i < 5; i++)
        {
            InventoryLib::BaseItem* tempItem = RandomItem();
            inv->AddItem(tempItem);
            tempItem = new InventoryLib::BaseItem(*tempItem);
            tempItem->currentStack = 50;
            inv->AddItem(tempItem);
        }
        printf("\n");

        printf("--- UNSORTED ---\n");
        printf(inv->GetInventoryStructure(false).c_str());
        inv->SortByTag();
        printf("\n");
        printf("--- SORTED BY TAG ---\n");
        printf(inv->GetInventoryStructure(false).c_str());
        printf("\n");
        printf("\n");
        printf("\n");
    }

    SetConsoleTextAttribute(hConsole, 6);
    printf("--- Testing Sort by Stack ---\n");
    SetConsoleTextAttribute(hConsole, 7);
    if (true)
    {
        InventoryLib::Inventory* inv = new InventoryLib::Inventory(100);
        printf("\n");

        //Testing Sort by name
        for (int i = 0; i < 5; i++)
        {
            InventoryLib::BaseItem* tempItem = RandomItem();
            tempItem->currentStack = rand()%200;
            if(tempItem->currentStack > tempItem->stackSize)
            {
                tempItem->currentStack = tempItem->stackSize;
            }
            inv->AddItem(tempItem);
            inv->AddItem(tempItem);
        }
        printf("\n");

        printf("--- UNSORTED ---\n");
        printf(inv->GetInventoryStructure(false).c_str());
        inv->SortByStack(false);
        printf("\n");
        printf("--- SORTED BY STACK ---\n");
        printf(inv->GetInventoryStructure(false).c_str());
        inv->SortByStack(true);
        printf("\n");
        printf(inv->GetInventoryStructure(false).c_str());
        inv->SortByStack(false);
        printf("\n");
        printf("\n");
    }

    SetConsoleTextAttribute(hConsole, 6);
    printf("--- All sorts in a row ---\n");
    SetConsoleTextAttribute(hConsole, 7);
    if (false)
    {
        InventoryLib::Inventory* inv = new InventoryLib::Inventory(100);
        for (int i = 0; i < 10; i++)
        {
            inv->AddItem(RandomItem());
        }

        printf("--- UNSORTED ---\n");
        printf(inv->GetInventoryStructure(false).c_str());
        printf("\n");
        inv->SortByName();
        printf("--- Sorted by Name ---\n");
        printf(inv->GetInventoryStructure(false).c_str());
        printf("\n");
        inv->SortByStack(false);
        printf("--- Sorted by Stack ---\n");
        printf(inv->GetInventoryStructure(false).c_str());
        printf("\n");
        inv->SortByTag();
        printf("--- Sorted by Tag ---\n");
        printf(inv->GetInventoryStructure(false).c_str());
        printf("\n");

        inv->SortByTag();
        printf("Breakpoint for looking into inventory");
        inv->AddItem(RandomItem());
        printf("Breakpoint for looking into inventory");
    }

    SetConsoleTextAttribute(hConsole, 6);
    printf("--- Copy Inventory ---\n");
    SetConsoleTextAttribute(hConsole, 7);
    if(false)
    {
        InventoryLib::Inventory* inv = new InventoryLib::Inventory(100);
        inv->AddItem(RandomItem());
        inv->AddItem(RandomItem());
        inv->AddItem(RandomItem());
        inv->AddItem(RandomItem());

        inv->SortByName(true);
        printf("\n");

        InventoryLib::Inventory* inv2 = new InventoryLib::Inventory(*inv);

        printf("%p \n", inv->GetItemInSlot(0).get());
        printf("%p \n", inv->GetItemInSlot(1).get());
        printf("%p \n", inv->GetItemInSlot(2).get());
        printf("%p \n", inv->GetItemInSlot(3).get());
        printf(inv->GetInventoryStructure(false).c_str());
        printf("\n");

        //this item should be the same item as in the other inventory but with a different pointer
        printf("%p \n", inv2->GetItemInSlot(0).get());
        printf("%p \n", inv2->GetItemInSlot(1).get());
        printf("%p \n", inv2->GetItemInSlot(2).get());
        printf("%p \n", inv2->GetItemInSlot(3).get());
        printf(inv2->GetInventoryStructure(false).c_str());

        printf("\n");
        printf("\n");
        printf("\n");
    }

    SetConsoleTextAttribute(hConsole, 6);
    printf("--- Get All Items with Tag Weapon ---\n");
    SetConsoleTextAttribute(hConsole, 7);
    if (false)
    {
        InventoryLib::Inventory* inv = new InventoryLib::Inventory(100);
        for(int i = 0; i < 20; i++)
        {
            inv->AddItem(RandomItem());
        }
        printf("All Items, sorted by tag\n");
        inv->SortByTag();
        printf(inv->GetInventoryStructure(false).c_str());

        printf("\n");
        printf("Weapons Only\n");
        std::vector<std::shared_ptr<InventoryLib::BaseItem>> weapons = inv->GetAllItemsWithTag("Weapon");

        for(int i = 0; i < static_cast<int>(weapons.size()); i++)
        {
            std::string temp = inv->GetItemAsString(*weapons[i], false);
            if (temp.empty()) continue;
            printf(temp.c_str());
        }
        printf("\n");
    }

    
    std::cin.get();
}
