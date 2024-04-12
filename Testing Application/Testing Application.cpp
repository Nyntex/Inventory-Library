#include <iostream>
#include "Inventory.h"


int main()
{
    auto randomItem = []() -> InventoryLib::BaseItem*
        {
            std::string randomTag = "";
            randomTag += char((rand() % 26) + 65);
            randomTag += char((rand() % 26) + 65);
            randomTag += char((rand() % 26) + 65);
            randomTag += char((rand() % 26) + 65);
            randomTag += char((rand() % 26) + 65);
            randomTag += char((rand() % 26) + 65);
            randomTag += char((rand() % 26) + 65);
            randomTag += char((rand() % 26) + 65);
            randomTag += char((rand() % 26) + 65);

            std::string randomName = "";
            randomName += char((rand() % 26) + 65);
            randomName += char((rand() % 26) + 65);
            randomName += char((rand() % 26) + 65);
            randomName += char((rand() % 26) + 65);

            std::string randomID = "";
            randomID += char((rand() % 26) + 65);
            randomID += char((rand() % 26) + 65);
            randomID += char((rand() % 26) + 65);
            randomID += char((rand() % 26) + 65);
            randomID += char((rand() % 26) + 65);
            randomID += char((rand() % 26) + 65);
            randomID += "-";
            randomID += char((rand() % 26) + 65);
            randomID += char((rand() % 26) + 65);
            randomID += char((rand() % 26) + 65);
            randomID += char((rand() % 26) + 65);
            randomID += char((rand() % 26) + 65);
            randomID += char((rand() % 26) + 65);
            randomID += "-";
            randomID += char((rand() % 26) + 65);
            randomID += char((rand() % 26) + 65);
            randomID += char((rand() % 26) + 65);
            randomID += char((rand() % 26) + 65);
            randomID += char((rand() % 26) + 65);
            randomID += char((rand() % 26) + 65);

            int randomStackSize = rand() % 256;
            
            InventoryLib::BaseItem* retVal = new InventoryLib::BaseItem(randomName, randomTag, randomID, randomStackSize, rand() % (randomStackSize / 2) + randomStackSize / 2);
            return retVal;
        };

    printf("--- Creation and deletion of Inventory ---\n");
    if(false)
    {
        InventoryLib::Inventory* temp = new InventoryLib::Inventory();
        delete temp;
        temp = nullptr;
        printf("\n");
        printf("\n");
    }

    printf("--- Creation of inventory with slots and shrinking it ---\n");
    if(false)
    {
        InventoryLib::Inventory* inv = new InventoryLib::Inventory(100);
        bool success;
        inv->SetSlotCount(16, success);
        delete inv;
        printf("\n");

        inv = new InventoryLib::Inventory(100.0f);
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

    printf("--- Testing Sort by Name ---\n");
    if(false)
    {
        InventoryLib::Inventory* inv = new InventoryLib::Inventory(100);
        inv = new InventoryLib::Inventory(100);
        printf("\n");

        InventoryLib::BaseItem* randomItem1 = new InventoryLib::BaseItem("Otto", "Weapon", "000001-00001-000001", 64, 7);
        printf("Created Item \"Otto\"\n");
        InventoryLib::BaseItem* randomItem2 = new InventoryLib::BaseItem("Ralf", "Weapon", "000021-00021-000021", 64, 4);
        printf("Created Item \"Ralf\"\n");

        inv->AddItem(new InventoryLib::BaseItem(*randomItem1));
        //printf("\n");


        //Testing Sort by name
        for (int i = 0; i < 20; i++)
        {
            InventoryLib::BaseItem* tempItem = new InventoryLib::BaseItem(*randomItem1);
            tempItem->currentStack = 64;
            tempItem->name = "";
            tempItem->name += char((rand() % 26) + 65);
            tempItem->name += char((rand() % 26) + 65);
            tempItem->name += char((rand() % 26) + 65);
            tempItem->name += char((rand() % 26) + 65);
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
    }

    printf("--- Testing Sort by Tag ---\n");
    if(false)
    {
        InventoryLib::Inventory* inv = new InventoryLib::Inventory(100);
        inv = new InventoryLib::Inventory(100);
        printf("\n");

        InventoryLib::BaseItem* randomItem1 = new InventoryLib::BaseItem("Otto", "Weapon", "000001-00001-000001", 64, 7);
        printf("Created Item \"Otto\"\n");
        InventoryLib::BaseItem* randomItem2 = new InventoryLib::BaseItem("Ralf", "Weapon", "000021-00021-000021", 64, 4);
        printf("Created Item \"Ralf\"\n");

        inv->AddItem(new InventoryLib::BaseItem(*randomItem1));
        //printf("\n");


        //Testing Sort by name
        for (int i = 0; i < 40; i++)
        {
            InventoryLib::BaseItem* tempItem = randomItem();

            inv->AddItem(tempItem);
            tempItem = new InventoryLib::BaseItem(*tempItem);
            tempItem->currentStack = 50;
            inv->AddItem(tempItem);
        }

        printf("--- UNSORTED ---\n");
        printf(inv->GetInventoryStructure(false).c_str());
        inv->SortByTag();
        printf("\n\n\n");
        printf("--- SORTED BY TAG ---\n");
        printf(inv->GetInventoryStructure(false).c_str());
    }

    printf("--- Testing Sort by Tag ---\n");
    if (true)
    {
        InventoryLib::Inventory* inv = new InventoryLib::Inventory(100);
        printf("\n");

        //Testing Sort by name
        for (int i = 0; i < 40; i++)
        {
            InventoryLib::BaseItem* tempItem = randomItem();
            inv->AddItem(tempItem);
            tempItem = new InventoryLib::BaseItem(*tempItem);
            tempItem->currentStack = 50;
            inv->AddItem(tempItem);
        }

        printf("--- UNSORTED ---\n");
        printf(inv->GetInventoryStructure(false).c_str());
        inv->SortByTag();
        printf("\n\n\n");
        printf("--- SORTED BY TAG ---\n");
        printf(inv->GetInventoryStructure(false).c_str());
    }

    //int* ignored;
    //printf("Does the inventory contain the item? : %s\n", inv->HasItem(new InventoryLib::BaseItem(*randomItem1), ignored, 64) ? "True" : "False");

    std::cin.get();
}
