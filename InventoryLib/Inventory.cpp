#include "pch.h"
#include "Inventory.h"
#include <iostream>
#include "macros.h"

#pragma region De/Constructors
InventoryLib::Inventory::Inventory()
{
    DEBUGPRINT("--- !!! --- \n THIS IS THE DEBUG BUILD, EXPECT MANY LOG OUTPUTS\n--- !!! --- \n")
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
        printf("Invalid item input to add to inventory!");
        #endif
        return;
    }
    
    for (BaseItem* slotItem : *items)
    {
        if (slotItem == nullptr) continue;
        if (*slotItem == *item)
        {
            if (slotItem->currentStack < slotItem->stackSize)
            {
                if(slotItem->stackSize - slotItem->currentStack >= item->currentStack)
                {
                    slotItem->currentStack += item->currentStack;
                    success = true;
                    return;
                }
                else
                {
                    item->currentStack -= slotItem->stackSize - slotItem->currentStack;
                    slotItem->currentStack = slotItem->stackSize;
                }
            }
        }
    }

    const int availableSlot = FindItem(nullptr);
    if(availableSlot == -1)
    {
        #ifdef _DEBUG
        printf("No space to add item");
        #endif
        return;
    }

    items->at(availableSlot) = item;
    success = true;
    #ifdef _DEBUG
    printf("Item was successfully added to a new slot (%i)", availableSlot);
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

    std::vector<BaseItem*>* itemsToRemove = new std::vector<BaseItem*>();
    int totalCount = 0;

    for (BaseItem* invItem : *items)
    {
        if (invItem == nullptr) continue;

        if (*invItem == *item)
        {
            totalCount += invItem->currentStack;
            itemsToRemove->push_back(invItem);
        }
    }

    if (totalCount < amount)
    {
        #ifdef _DEBUG
        printf("Not enough items of this kind: %s", item->ID.c_str());
        #endif
    }

    for (BaseItem* removable : *itemsToRemove)
    {
        if(removable->currentStack >= amount)
        {
            removable->currentStack -= amount;
            success = true;
            return;
        }

        if(removable->currentStack < amount)
        {
            amount -= removable->currentStack;
            removable = nullptr;
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
        if (!items->at(i)) continue;

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
                if(items->at(i)->currentStack == items->at(i)->stackSize)
                {
                    continue;
                }
            }
            return i;
        }
    }

    return -1;
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
