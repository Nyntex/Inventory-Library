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
    this->maxWeight = -1;
    this->items = new std::vector<BaseItem*>();
}

InventoryLib::Inventory::Inventory(int newSlotCount)
{
    #ifdef DEBUG_PRINTING
    printf("Creating inventory with %i slots.\n", newSlotCount);
    #endif

    this->maxWeight = -1.0f;
    this->items = new std::vector<BaseItem*>(newSlotCount);
}

InventoryLib::Inventory::Inventory(float newWeight)
{
    #ifdef DEBUG_PRINTING
    printf("Creating inventory with %f maxWeight.\n", newWeight);
    #endif

    this->maxWeight = newWeight;
    this->items = new std::vector<BaseItem*>();
}

InventoryLib::Inventory::Inventory(int newSlotCount, float newWeight)
{
    #ifdef DEBUG_PRINTING
    printf("Creating inventory with %i slots and %f maxWeight.\n", newSlotCount, newWeight);
    #endif

    this->maxWeight = newWeight;
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

    if (newSlotCount < 1) return;

    if (GetInventorySize() == newSlotCount)
    {
        #ifdef DEBUG_PRINTING
        printf("inventory size = new slot count. no inventory size changes.\n");
        #endif

        success = true;
        return;
    }

    if (GetInventorySize() < newSlotCount)
    {
        const int amountToAdd = newSlotCount - GetInventorySize();

        #ifdef DEBUG_PRINTING
        const int originalSize = GetInventorySize();
        printf("Trying to add %i slots to inventory with %i slots.\n",amountToAdd, originalSize);
        #endif

        for (int i = 0; i < amountToAdd; i++)
        {
            items->push_back(nullptr);
        }

        #ifdef DEBUG_PRINTING
        printf("successfully changed inventory size from %i to %i.\n", originalSize, GetInventorySize());
        #endif

        success = true;
    }

    else if (GetInventorySize() > newSlotCount)
    {
        bool canShrink = true;
        const int amountToRemove = GetInventorySize() - newSlotCount;

        #ifdef DEBUG_PRINTING
        const int originalSize = GetInventorySize();
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
        printf("Slot count successfully shrank from %i to %i.\n", originalSize, GetInventorySize());
        #endif

        success = true;
    }
}

void InventoryLib::Inventory::SetSlotCount(int newSlotCount)
{
    bool ignored;
    SetSlotCount(newSlotCount, ignored);
}


void InventoryLib::Inventory::AddItem(BaseItem* item)
{
    bool ignored;
    AddItem(item, ignored);
}

void InventoryLib::Inventory::AddItem(BaseItem* item, int slot)
{
    bool ignored;
    AddItem(item, slot, ignored);
}

void InventoryLib::Inventory::AddItem(BaseItem* item, bool& success)
{
    success = false;
    for (const BaseItem* slotItem : items)
    {
        if(slotItem == nullptr)
        {
            slotItem = item;
            success = true;
            return;
        }
    }
}

void InventoryLib::Inventory::AddItem(BaseItem* item, int slot, bool& success)
{
    success = false;
    if (slot >= GetInventorySize() || slot < 0) return;
    if (items->at(slot) != nullptr) return;

    items->at(slot) = item;
    success = true;
}


void InventoryLib::Inventory::RemoveItem(BaseItem* item)
{
    bool ignored;
    RemoveItem(item, ignored);
}

void InventoryLib::Inventory::RemoveItem(BaseItem* item, bool& success)
{
    success = false;

}

void InventoryLib::Inventory::RemoveItem(int slot)
{
    bool ignored;
    RemoveItem(slot, ignored);
}

void InventoryLib::Inventory::RemoveItem(int slot, bool& success)
{
    success = false;
}

InventoryLib::BaseItem* InventoryLib::Inventory::GetItemInSlot(int slot) const
{
    if (slot >= GetInventorySize() || slot < 0) return nullptr;
    return items->at(slot);
}
