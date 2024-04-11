#include <iostream>
#include "Inventory.h"


int main()
{
    printf("--- Creation and deletion of Inventory ---\n");

    InventoryLib::Inventory* temp = new InventoryLib::Inventory();
    delete temp;
    temp = nullptr;

    printf("\n");
    printf("--- Creation of inventory with slots and shrinking it ---\n");

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

    inv = new InventoryLib::Inventory(100);
    printf("\n");

    InventoryLib::BaseItem* randomItem1 = new InventoryLib::BaseItem("Otto", "Weapon", "000001-00001-000001", 64, 7);
    printf("Created Item \"Otto\"\n");
    InventoryLib::BaseItem* randomItem2 = new InventoryLib::BaseItem("Ralf", "Weapon", "000021-00021-000021", 64, 4);
    printf("Created Item \"Ralf\"\n");

    inv->AddItem(new InventoryLib::BaseItem(*randomItem1));
    //printf("\n");

    for(int i = 0; i < 20; i++)
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

    /*for (int i = 0; i < 5; i++)
    {
        InventoryLib::BaseItem* tempItem = new InventoryLib::BaseItem(*randomItem1);
        tempItem->currentStack = rand() % 20;
        inv->AddItem(tempItem, (rand()%50)+6);
    }*/

    printf(inv->GetInventoryStructure().c_str());
    inv->SortByName();
    printf(inv->GetInventoryStructure().c_str());

    //inv->RemoveItem(new InventoryLib::BaseItem(*randomItem1), 100);
    

    //int* ignored;
    //printf("Does the inventory contain the item? : %s\n", inv->HasItem(new InventoryLib::BaseItem(*randomItem1), ignored, 64) ? "True" : "False");

}
