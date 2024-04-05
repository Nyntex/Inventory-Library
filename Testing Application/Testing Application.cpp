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
}
