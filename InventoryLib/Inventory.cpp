#include "pch.h"
#include "Inventory.h"
#include <iostream>
#include <windows.h>

#pragma region De/Constructors
InventoryLib::Inventory::Inventory()
{
    #ifdef _DEBUG
    printf("--- !!! --- \n THIS IS THE DEBUG BUILD, EXPECT MANY LOG OUTPUTS\n--- !!! --- \n");
    #endif
    this->maxCarryWeight = -1;
    this->items = std::make_unique<BaseItemVector>();
}

InventoryLib::Inventory::Inventory(int newSlotCount)
{
#ifdef _DEBUG
    printf("Creating inventory with %i slots.\n", newSlotCount);
#endif

    this->maxCarryWeight = -1.0f;
    this->items = std::make_unique<BaseItemVector>(newSlotCount);
}

InventoryLib::Inventory::Inventory(float newWeight)
{
    #ifdef _DEBUG
    printf("Creating inventory with %f maxCarryWeight.\n", newWeight);
    #endif

    this->maxCarryWeight = newWeight;
    this->items = std::make_unique<BaseItemVector>();
}

InventoryLib::Inventory::Inventory(int newSlotCount, float newWeight, int maxSlots, float maxWeight)
{
    #ifdef _DEBUG
    printf("Creating inventory with %i slots and %f maxCarryWeight.\n", newSlotCount, newWeight);
    #endif

    
    this->maxSlots = maxSlots;
    this->maxCarryWeight = newWeight;
    this->items = std::make_unique<BaseItemVector>(newSlotCount);
}

InventoryLib::Inventory::Inventory(const Inventory& other)
{
    maxCarryWeight = other.maxCarryWeight;
    maxSlots = other.maxSlots;
    items = std::make_unique<BaseItemVector>(other.GetInventorySize());

    for (int i = 0; i < other.GetInventorySize(); i++)
    {
        items->at(i) = std::make_shared<BaseItem>(*other.items->at(i));
    }
}

InventoryLib::Inventory::~Inventory()
{
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

    items->at(availableSlot) = std::make_shared<BaseItem>(*item);
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
        if(*items->at(slot) != *item)
        {
            #ifdef _DEBUG
            printf("Theres already a different item in slot %i. ID: %s \n", slot, items->at(slot)->ID.c_str());
            #endif
            return;
        }

        const int possibleToAdd = items->at(slot)->stackSize - items->at(slot)->currentStack;
        if(possibleToAdd >= item->currentStack)
        {
            items->at(slot)->currentStack += item->currentStack;
            success = true;

            #ifdef _DEBUG
            printf("Successfully increased stack of item in slot %i.\n", slot);
            #endif
            return;
        }
        else
        {
            items->at(slot)->currentStack += possibleToAdd;
            item->currentStack -= possibleToAdd;
            success = true;
            #ifdef _DEBUG
            printf("Successfully filled stack of item in slot %i. The input item stack got reduced to %i.\n", slot, item->currentStack);
            #endif
            return;
        }
    }

    items->at(slot) = std::make_shared<BaseItem>(*item);
    success = true;
    #ifdef _DEBUG
    printf("Successfully added it as a new item in slot %i.\n", slot);
    #endif
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

    for (SharedBaseItem& invItem : *items)
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
            items->at(removableSlot) = nullptr;

            #ifdef _DEBUG
            printf("Removed the whole stack in slot %i. \n", removableSlot);
            #endif
        }
    }
}


void InventoryLib::Inventory::RemoveItemInSlot(int slot)
{
    bool ignored1;
    RemoveItemInSlot(slot, ignored1);
}

void InventoryLib::Inventory::RemoveItemInSlot(int slot, bool& success)
{
    success = false;

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

    items->at(slot) = nullptr;
    #ifdef _DEBUG
    printf("Successfully removed item in slot %i.", slot);
    #endif
}



void InventoryLib::Inventory::SortByName(bool ascending)
{
    auto sorting = [](Inventory* inv, int slotPos, bool ascending) -> bool
        {
            if (inv->items->at(slotPos) == nullptr)
            {
                return true;
            }
            if (inv->items->at(slotPos + 1) == nullptr) return false;

            if (inv->items->at(slotPos)->name == inv->items->at(slotPos + 1)->name)
            {
                if (inv->items->at(slotPos)->currentStack < inv->items->at(slotPos + 1)->currentStack)
                {
                    return true;
                }
            }

            if (ascending)
            {
                if (IsStringGreater(inv->items->at(slotPos)->name, inv->items->at(slotPos + 1)->name))
                {
                    return true;
                }
            }
            else
            {
                if (!IsStringGreater(inv->items->at(slotPos)->name, inv->items->at(slotPos + 1)->name))
                {
                    return true;
                }
            }

            return false;
        };

    Sort(sorting, ascending);
}

void InventoryLib::Inventory::SortByTag(bool ascending) //first tries to sort by tag name, than by name and than by stack size
{
    //bubble sort
    auto sorting = [](Inventory* inv, int slotPos, bool ascending) -> bool
    {
            if (inv->items->at(slotPos) == nullptr)
            {
                return true;
            }
            if (inv->items->at(slotPos + 1) == nullptr) return false;

            if (inv->items->at(slotPos)->tag == inv->items->at(slotPos + 1)->tag)
            {
                if (inv->items->at(slotPos)->name == inv->items->at(slotPos + 1)->name &&
                    inv->items->at(slotPos)->currentStack < inv->items->at(slotPos + 1)->currentStack)
                {
                    return true;
                }
            }

            if (ascending)
            {

                if (IsStringGreater(inv->items->at(slotPos)->tag, inv->items->at(slotPos + 1)->tag))
                {
                    return true;
                }
            }
            else
            {
                if (!IsStringGreater(inv->items->at(slotPos)->tag, inv->items->at(slotPos + 1)->tag))
                {
                    return true;
                }
            }
            return false;
    };

    Sort(sorting, ascending);
}

void InventoryLib::Inventory::SortByStack(bool ascending)
{
    auto sorting = [](Inventory* inv, int slotPos, bool ascending) -> bool
        {
            if (inv->items->at(slotPos) == nullptr)
            {
                return true;
            }
            if (inv->items->at(slotPos + 1) == nullptr) return false;

            if (inv->items->at(slotPos)->currentStack == inv->items->at(slotPos + 1)->currentStack)
            {
                if (inv->items->at(slotPos)->name == inv->items->at(slotPos + 1)->name &&
                    inv->items->at(slotPos)->currentStack < inv->items->at(slotPos + 1)->currentStack)
                {
                    return true;
                }

                if (IsStringGreater(inv->items->at(slotPos)->name, inv->items->at(slotPos + 1)->name))
                {
                    return true;
                }
            }

            if (ascending)
            {

                if (inv->items->at(slotPos)->currentStack > inv->items->at(slotPos + 1)->currentStack)
                {
                    return true;
                }
            }
            else
            {
                if (inv->items->at(slotPos)->currentStack < inv->items->at(slotPos + 1)->currentStack)
                {
                    return true;
                }
            }

            return false;
        };

    Sort(sorting, ascending);
}


std::string InventoryLib::Inventory::GetInventoryStructure(bool readable) const
{
    std::string retVal = "";

    for (int i = 0; i < GetInventorySize(); i++)
    {
        if (items->at(i) == nullptr) continue;
        if (!items->at(i)->IsValid()) continue;

        if (readable)
        {
            retVal += "{\n";

            retVal += "\tItemID:" + items->at(i)->ID + ":\n" + 
                "\t{\n";

            retVal += "\t\tName:" + items->at(i)->name + ";\n" + 
                "\t\tTag:" + items->at(i)->tag + ";\n" + 
                "\t\tStackSize:" + std::to_string(items->at(i)->stackSize) + ";\n" +
                "\t\tCurrentStack:" + std::to_string(items->at(i)->currentStack) + ";\n" +
                "\t\tSlot:" + std::to_string(i) + ";\n";

            retVal += "\t}\n";
            retVal += "}\n";
        }
        else
        {
            retVal += "{ItemID:" + items->at(i)->ID + ": "+
                "{Name:" + items->at(i)->name + "; Tag:" + items->at(i)->tag + 
                "; StackSize:" + std::to_string(items->at(i)->stackSize) + 
                "; CurrentStack:" + std::to_string(items->at(i)->currentStack) +
                "; Slot:" + std::to_string(i) + ";}}\n";
        }
    }
    return retVal;
}

int InventoryLib::Inventory::FindItem(BaseItem* item, bool allowFullStacks) const
{
    for(int i = 0; i < GetInventorySize(); i++)
    {
        if(*items->at(i) == *item)
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

bool InventoryLib::Inventory::HasItem(BaseItem* item, int*& slots, int amount) const
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

bool InventoryLib::IsStringGreater(const std::string& first, const std::string& second, int pos)
{
    if (pos < 0) pos = 0;

    std::string optimisedFirst = first;
    std::string optimisedSecond = second;

    if(pos == 0) //optimise strings in first runthrough
    {
        optimisedFirst = MakeStringUpperCase(first);
        optimisedSecond = MakeStringUpperCase(second);

        if (optimisedFirst == optimisedSecond) return false;
    }

    if (pos > static_cast<int>(optimisedFirst.size()) && pos > static_cast<int>(optimisedSecond.size())) return false;
    if (pos > static_cast<int>(optimisedFirst.size())) return false;
    if (pos > static_cast<int>(optimisedSecond.size())) return true;

    if (optimisedFirst[pos] == optimisedSecond[pos])
    {
        return IsStringGreater(optimisedFirst, optimisedSecond, pos + 1);
    }

    return(optimisedFirst[pos] > optimisedSecond[pos]);
}

std::string InventoryLib::MakeStringUpperCase(const std::string& word)
{
    std::string retVal = "";

    for (char symbol : word)
    {
        if(int(symbol) > 96 && (int)symbol < 123) //is lower case
        {
            retVal += char(int(symbol) - 32);
        }
        else
        {
            retVal += symbol;
        }
    }

    return retVal;
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

    return items->at(slot).get();
}


void InventoryLib::Inventory::Sort(bool(* comparison)(Inventory*, int, bool), bool ascending)
{
    for (int i = 1; i < GetInventorySize(); i++)
    {
        for (int j = 0; j < GetInventorySize() - i; j++)
        {
            if(comparison(this, j, ascending))
            {
                Reorder(j, j + 1);
            }
        }
    }
}

void InventoryLib::Inventory::Reorder(int pos, int pos2)
{
    SharedBaseItem temp = items->at(pos);
    items->at(pos) = items->at(pos2);
    items->at(pos2) = temp;
}



