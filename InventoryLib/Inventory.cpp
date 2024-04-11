#include "pch.h"
#include "Inventory.h"
#include <iostream>
#include "macros.h"

#pragma region De/Constructors
InventoryLib::Inventory::Inventory()
{
    #ifdef _DEBUG
    printf("--- !!! --- \n THIS IS THE DEBUG BUILD, EXPECT MANY LOG OUTPUTS\n--- !!! --- \n");
    #endif
    this->maxWeight = -1;
    this->items = new std::vector<BaseItem*>();
}

InventoryLib::Inventory::Inventory(int newSlotCount)
{
#ifdef _DEBUG
    printf("Creating inventory with %i slots.\n", newSlotCount);
#endif

    this->maxWeight = -1.0f;
    this->items = new std::vector<BaseItem*>(newSlotCount);
}

InventoryLib::Inventory::Inventory(float newWeight)
{
    #ifdef _DEBUG
    printf("Creating inventory with %f maxWeight.\n", newWeight);
    #endif

    this->maxWeight = newWeight;
    this->items = new std::vector<BaseItem*>();
}

InventoryLib::Inventory::Inventory(int newSlotCount, float newWeight)
{
    #ifdef _DEBUG
    printf("Creating inventory with %i slots and %f maxWeight.\n", newSlotCount, newWeight);
    #endif

    this->maxWeight = newWeight;
    this->items = new std::vector<BaseItem*>(newSlotCount);
}

InventoryLib::Inventory::~Inventory()
{
    delete items;
    items = nullptr;
    #ifdef _DEBUG
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
        #ifdef _DEBUG
        printf("inventory size = new slot count. no inventory size changes.\n");
        #endif

        success = true;
        return;
    }

    if (GetInventorySize() < newSlotCount)
    {
        const int amountToAdd = newSlotCount - GetInventorySize();

        #ifdef _DEBUG
        const int originalSize = GetInventorySize();
        printf("Trying to add %i slots to inventory with %i slots.\n",amountToAdd, originalSize);
        #endif

        for (int i = 0; i < amountToAdd; i++)
        {
            items->push_back(nullptr);
        }

        #ifdef _DEBUG
        printf("successfully changed inventory size from %i to %i.\n", originalSize, GetInventorySize());
        #endif


        

        success = true;
    }

    else if (GetInventorySize() > newSlotCount)
    {
        bool canShrink = true;
        const int amountToRemove = GetInventorySize() - newSlotCount;

        #ifdef _DEBUG
        const int originalSize = GetInventorySize();
        printf("Trying to remove %i slots from inventory with %i slots.\n", amountToRemove, originalSize);
        #endif

        for(int i = newSlotCount-1; i < static_cast<int>(items->size()); i++)
        {
            if (items->at(i) != nullptr) canShrink = false;
            if (!canShrink) return;
        }

        #ifdef _DEBUG
        printf("Slots to remove are empty. Progressing to removing slots.\n");
        #endif

        for(int i = 0; i < amountToRemove; i++)
        {
            items->pop_back();
        }

        #ifdef _DEBUG
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
    if (item == nullptr)
    {
        #ifdef _DEBUG
        printf("Invalid item input to add to inventory!\n");
        #endif
        return;
    }
    if(!item->IsValid())
    {
        #ifdef _DEBUG
        printf("Invalid item input to add to inventory!\n");
        #endif
        return;
    }

    #ifdef _DEBUG
    printf(("[Adding item with ID: " + item->ID + "]").c_str());
    #endif

    for(int i = 0; i < GetInventorySize(); i++)
    {
        if (items->at(i) == nullptr) continue;
        if (*items->at(i) == *item)
        {
            if (items->at(i)->currentStack < items->at(i)->stackSize)
            {
                if(items->at(i)->stackSize - items->at(i)->currentStack >= item->currentStack)
                {
                    items->at(i)->currentStack += item->currentStack;
                    success = true;

                    #ifdef _DEBUG
                    printf("Added %i to stack in slot %i. Went from %i to %i.", item->currentStack, i, items->at(i)->currentStack - item->currentStack, items->at(i)->currentStack);
                    if(items->at(i)->currentStack == items->at(i)->stackSize)
                    {
                        printf("Slot %i is maxed out now.", i);
                    }
                    printf("\n");
                    #endif
                    return;
                }
                else
                {
                    int added = item->currentStack;
                    item->currentStack -= items->at(i)->stackSize - items->at(i)->currentStack;
                    items->at(i)->currentStack = items->at(i)->stackSize;

                    #ifdef _DEBUG
                    printf("Added %i to stack in slot %i. Maxed it. Total to add is now %i.\n", added, i, item->currentStack);
                    #endif
                }
            }
        }
    }

    const int availableSlot = FindItem(nullptr);
    if(availableSlot == -1)
    {
        #ifdef _DEBUG
        printf("No space to add item.\n");
        #endif
        return;
    }

    items->at(availableSlot) = item;
    success = true;
    #ifdef _DEBUG
    printf("Item was successfully added to a new slot (%i).\n", availableSlot);
    #endif
}

void InventoryLib::Inventory::AddItem(BaseItem* item, int slot, bool& success)
{
    success = false;

    if (slot >= GetInventorySize() || slot < 0)
    {
        #ifdef _DEBUG
        printf("Slot outside inventory range! Slot: %i | InventorySize: %i \n", slot, GetInventorySize());
        #endif
        return;
    }

    if (item == nullptr)
    {
        #ifdef _DEBUG
        printf("tried to add nullptr to inventory");
        #endif
        return;
    }
    if (items->at(slot) != nullptr)
    {
        if(items->at(slot) != item)
        {
            #ifdef _DEBUG
            printf("Theres already an item in slot %i. ID: %s \n", slot, items->at(slot)->ID.c_str());
            #endif
            return;
        }

        const int possibleToAdd = items->at(slot)->stackSize - items->at(slot)->currentStack;
        if(possibleToAdd >= item->currentStack)
        {
            items->at(slot)->currentStack += item->currentStack;
            success = true;

            #ifdef _DEBUG
            printf("Successfully increased stack of item in slot %i", slot);
            #endif
            return;
        }
        else
        {
            items->at(slot)->currentStack += possibleToAdd;
        }

        return;
    }

    items->at(slot) = item;
    success = true;
}



void InventoryLib::Inventory::RemoveItem(BaseItem* item)
{
    bool ignored;
    RemoveItem(item, ignored);
}

void InventoryLib::Inventory::RemoveItem(BaseItem* item, int amount)
{
    bool ignored;
    RemoveItem(item, amount, ignored);
}

void InventoryLib::Inventory::RemoveItem(BaseItem* item, bool& success)
{
    success = false;

    for (BaseItem* invItem : *items)
    {
        if (invItem == nullptr) continue;

        if(*invItem == *item)
        {
            invItem = nullptr;
            success = true;

            #ifdef _DEBUG
            printf("Successfully removed itemstack with ID %s.\n", item->ID.c_str());
            #endif

            return;
        }
    }
    
}

void InventoryLib::Inventory::RemoveItem(BaseItem* item, int amount, bool& success)
{
    success = false;

    if(item == nullptr)
    {
        #ifdef _DEBUG
        printf("Invalid item input to remove from inventory!\n");
        #endif
        return;
    }

    std::vector<int>* itemsToRemove = new std::vector<int>();
    int totalCount = 0;

    for(int i = 0; i < GetInventorySize(); i++)
    {
        if (items->at(i) == nullptr) continue;

        if (*items->at(i) == *item)
        {
            totalCount += items->at(i)->currentStack;
            itemsToRemove->push_back(i);
        }
    }

    if (totalCount < amount)
    {
        #ifdef _DEBUG
        printf("Not enough items of this kind: %s", item->ID.c_str());
        #endif
    }

    for (const int removableSlot : *itemsToRemove)
    {
        if(items->at(removableSlot)->currentStack >= amount)
        {
            #ifdef _DEBUG
            printf("Removed %i items from stack in slot %i. Went from %i to %i.\n", amount, removableSlot, items->at(removableSlot)->currentStack, items->at(removableSlot)->currentStack - amount);
            #endif
            items->at(removableSlot)->currentStack -= amount;
            success = true;
            return;
        }

        if(items->at(removableSlot)->currentStack < amount)
        {
            amount -= items->at(removableSlot)->currentStack;
            delete items->at(removableSlot);
            items->at(removableSlot) = nullptr;

            #ifdef _DEBUG
            printf("Removed the whole stack in slot %i. \n", removableSlot);
            #endif
        }
    }
}


void InventoryLib::Inventory::RemoveItem(int slot)
{
    bool ignored1;
    BaseItem* ignored2;
    RemoveItem(slot, ignored1, ignored2);
    delete ignored2;
}

void InventoryLib::Inventory::RemoveItem(int slot, BaseItem*& removedItem)
{
    bool ignored;
    RemoveItem(slot, ignored, removedItem);
}

void InventoryLib::Inventory::RemoveItem(int slot, bool& success, BaseItem*& removedItem)
{
    success = false;
    removedItem = nullptr;

    if (slot >= GetInventorySize() || slot < 0)
    {
        #ifdef _DEBUG
        printf("Slot out of range. Slot: %i | Inventory size: %i", slot, GetInventorySize());
        #endif
        return;
    }

    if (items->at(slot) == nullptr)
    {
        #ifdef _DEBUG
        printf("Item in slot %i is already empty", slot);
        #endif
        return;
    }

    if (!items->at(slot)->IsValid())
    {
        #ifdef _DEBUG
        printf("Item in slot %i is invalid", slot);
        #endif
        return;
    }

    removedItem = new BaseItem(*items->at(slot));
    delete items->at(slot);
    items->at(slot) = nullptr;
    #ifdef _DEBUG
    printf("Successfully removed item in slot %i.", slot);
    #endif
}


void InventoryLib::Inventory::SortNameUp()
{
    std::vector<BaseItem*>* temp = new std::vector<BaseItem*>();

}

void InventoryLib::Inventory::SortNameDown()
{
}

void InventoryLib::Inventory::SortTagUp()
{
}

void InventoryLib::Inventory::SortTagDown()
{
}

void InventoryLib::Inventory::SortStackUp()
{
}

void InventoryLib::Inventory::SortStackDown()
{
}

std::string InventoryLib::Inventory::GetInventoryStructure()
{
    std::string retVal = "";

    for(int i =0; i < GetInventorySize(); i++)
    {
        if (items->at(i) == nullptr) continue;
        if (!items->at(i)->IsValid()) continue;

        retVal += "{\n";

        retVal += "\tItemID:" + items->at(i)->ID + ":\n";
        retVal += "\t{\n";

        retVal += "\t\tName:" + items->at(i)->name + ";\n";
        retVal += "\t\tTag:" + items->at(i)->tag + ";\n";
        retVal += "\t\tStackSize:" + std::to_string(items->at(i)->stackSize) + ";\n";
        retVal += "\t\tCurrentStack:" + std::to_string(items->at(i)->currentStack) + ";\n";
        retVal += "\t\tSlot:" + std::to_string(i) + ";\n";

        retVal += "\t}\n";
        retVal += "}\n";
    }

    return retVal;
}

int InventoryLib::Inventory::FindItem(BaseItem* item, bool allowFullStacks)
{
    for(int i = 0; i < GetInventorySize(); i++)
    {
        if(items->at(i) == item)
        {
            if(!allowFullStacks && item != nullptr)
            {
                if(items->at(i)->IsStackFull())
                {
                    continue;
                }
            }
            return i;
        }
    }
    return -1;
}

bool InventoryLib::Inventory::HasItem(BaseItem* item, int*& slots, int amount)
{
    if (!item) return false;

    std::vector<int> slotsWithItem = std::vector<int>();
    int count = 0;

    for(int i = 0; i< GetInventorySize(); i++)
    {
        if (items->at(i) == nullptr) continue;

        if(*items->at(i) == *item)
        {
            slotsWithItem.push_back(i);
            count += items->at(i)->currentStack;
        }
    }

    slots = slotsWithItem.data();

    return (count >= amount && !slotsWithItem.empty());
}


InventoryLib::BaseItem* InventoryLib::Inventory::GetItemInSlot(int slot) const
{
    if (slot >= GetInventorySize() || slot < 0)
    {
        #ifdef _DEBUG
        printf("Slot out of range. Slot: %i | Inventory size: %i", slot, GetInventorySize());
        #endif
        return nullptr;
    }

    if(items->at(slot) == nullptr)
    {
        #ifdef _DEBUG
        printf("Item in slot %i does not exist.", slot);
        #endif
    }

    return items->at(slot);
}
