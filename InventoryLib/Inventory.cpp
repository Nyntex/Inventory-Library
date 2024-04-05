#include "pch.h"
#include "Inventory.h"
#include <iostream>

#define DEBUG_PRINTING // Comment to disable debugging


#pragma region De/Constructors
InventoryLib::Inventory::Inventory()
{
#ifdef DEBUG_PRINTING
    printf("--- !!! --- \n Debugging is enabled. Comment out define DEBUG_PRINTING to disable when building\n--- !!! --- \n");
#endif
    this->slotCount = -1;
    this->weight = -1;
    this->items = new std::vector<BaseItem*>();
}

InventoryLib::Inventory::Inventory(int newSlotCount)
{
#ifdef DEBUG_PRINTING
    printf("Creating inventory with %i slots.\n", newSlotCount);
#endif

    this->slotCount = newSlotCount;
    this->weight = -1.0f;
    this->items = new std::vector<BaseItem*>(newSlotCount);
}

InventoryLib::Inventory::Inventory(float newWeight)
{
#ifdef DEBUG_PRINTING
    printf("Creating inventory with %f weight.\n", newWeight);
#endif

    this->slotCount = -1;
    this->weight = newWeight;
    this->items = new std::vector<BaseItem*>();
}

InventoryLib::Inventory::Inventory(int newSlotCount, float newWeight)
{
#ifdef DEBUG_PRINTING
    printf("Creating inventory with %i slots and %f weight.\n", newSlotCount, newWeight);
#endif

    this->slotCount = newSlotCount;
    this->weight = newWeight;
    this->items = new std::vector<BaseItem*>(newSlotCount);
}

InventoryLib::Inventory::~Inventory()
{
    delete items;
    items = nullptr;
#ifdef DEBUG_PRINTING
    printf("Successfully deleted inventory\n");
#endif
}

#pragma endregion

void InventoryLib::Inventory::SetSlotCount(int newSlotCount, bool& success)
{
    success = false;

    if (static_cast<int>(items->size()) == newSlotCount)
    {
#ifdef DEBUG_PRINTING
        printf("inventory size = new slot count. no inventory size changes.\n");
#endif

        success = true;
        return;
    }

    if (static_cast<int>(items->size()) < newSlotCount)
    {
        const int amountToAdd = newSlotCount - static_cast<int>(items->size());

#ifdef DEBUG_PRINTING
        const int originalSize = static_cast<int>(items->size());
        printf("Trying to add %i slots to inventory with %i slots.\n",amountToAdd, originalSize);
#endif

        for (int i = 0; i < amountToAdd; i++)
        {
            items->push_back(nullptr);
        }

#ifdef DEBUG_PRINTING
        printf("successfully changed inventory size from %i to %i.\n", originalSize, static_cast<int>(items->size()));
#endif

        success = true;
    }

    else if (static_cast<int>(items->size()) > newSlotCount)
    {
        bool canShrink = true;
        const int amountToRemove = static_cast<int>(items->size()) - newSlotCount;

#ifdef DEBUG_PRINTING
        const int originalSize = static_cast<int>(items->size());
        printf("Trying to remove %i slots from inventory with %i slots.\n", amountToRemove, originalSize);
#endif

        for(int i = newSlotCount-1; i < static_cast<int>(items->size()); i++)
        {
            if (items->at(i) != nullptr) canShrink = false;
            if (!canShrink) return;
        }

#ifdef DEBUG_PRINTING
        printf("Slots to remove are empty. Progressing to removing slots.\n");
#endif

        for(int i = 0; i < amountToRemove; i++)
        {
            items->pop_back();
        }

#ifdef DEBUG_PRINTING
        printf("Slot count successfully shrank from %i to %i.\n", originalSize, static_cast<int>(items->size()));
#endif

        success = true;
    }
}

void InventoryLib::Inventory::SetSlotCount(int newSlotCount)
{
    bool ignored;
    SetSlotCount(newSlotCount, ignored);
}

InventoryLib::BaseItem* InventoryLib::Inventory::GetItemBySlot(int slot)
{
    //TODO
    return nullptr;
}
