#include "pch.h"
#include "Inventory.h"
#include <iostream>

#pragma region De/Constructors



InventoryLib::Inventory::Inventory(int newSlotCount, float newWeight, int maxSlots, bool newAutoResize, float maxWeight)
{
    #ifdef _DEBUG
    printf("Creating inventory with %i slots and %f maxCarryWeight.\n", newSlotCount, newWeight);
    #endif

    
    this->maxSlots = maxSlots;
    this->maxCarryWeight = maxWeight;
    this->weight = newWeight;
    this->autoResize = newAutoResize;
    this->items = std::make_unique<BaseItemVector>(newSlotCount);
}

InventoryLib::Inventory::Inventory(const Inventory& other)
{
    maxCarryWeight = other.maxCarryWeight;
    weight = other.weight;
    maxSlots = other.maxSlots;
    autoResize = other.autoResize;
    items = std::make_unique<BaseItemVector>(other.GetInventorySize());

    for (int i = 0; i < other.GetInventorySize(); i++)
    {
        if (other.items->at(i) == nullptr) continue;
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


bool InventoryLib::Inventory::SetSlotCount(int newSlotCount)
{
    bool success = false;

    if (newSlotCount < 0)
    {
        return false;
    }

    if (GetInventorySize() == newSlotCount)
    {
        #ifdef _DEBUG
        printf("inventory size = new slot count. no inventory size changes.\n");
        #endif

        success = true;
    }
    else if (GetInventorySize() < newSlotCount)
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
            if (!canShrink) return success;
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
    ClearPresortedVectors();
    return success;
}


bool InventoryLib::Inventory::AddItem(BaseItem* item)
{
    bool success = false;
    if (item == nullptr)
    {
        #ifdef _DEBUG
        printf("Invalid item input to add to inventory!\n");
        #endif
        return false;
    }
    if (!item->IsValid())
    {
        #ifdef _DEBUG
        printf("Invalid item input to add to inventory!\n");
        #endif
        return false;
    }

    bool enoughSpace = HasEnoughSpaceToAddItem(item);
    if (!enoughSpace)
    {
        if(autoResize)
        {
            for(int i = 0; i < 4; i++)
            {
                items->push_back(nullptr);
            }
            enoughSpace = true;
        }
        else
        {
            #ifdef _DEBUG
            printf("Not enough space to add item!\n");
            #endif
            return false;
        }
    }
    if(maxCarryWeight >= 0.0f)
    {
        if(GetCurrentCarryingWeight() + item->WeightOfStack() > maxCarryWeight)
        {
            #ifdef _DEBUG
            printf("Not enough carrying capacity to add item!\n");
            #endif
            return false;
        }
    }

    #ifdef _DEBUG
    printf("[Adding item with ID: %s]", item->ID.c_str());
    #endif

    ClearPresortedVectors();

    for (int i = 0; i < GetInventorySize(); i++)
    {
        if (items->at(i) == nullptr) continue;
        if (*items->at(i) == *item)
        {
            if (items->at(i)->currentStack < items->at(i)->stackSize)
            {
                if (items->at(i)->stackSize - items->at(i)->currentStack >= item->currentStack)
                {
                    items->at(i)->currentStack += item->currentStack;
                    success = true;

                    #ifdef _DEBUG
                    printf("Added %i to stack in slot %i. Went from %i to %i.", item->currentStack, i, items->at(i)->currentStack - item->currentStack, items->at(i)->currentStack);
                    if (items->at(i)->currentStack == items->at(i)->stackSize)
                    {
                        printf("Slot %i is maxed out now.", i);
                    }
                    printf("\n");
                    #endif  
                    return success;
                }
                else
                {
                    #ifdef _DEBUG
                    const int added = item->currentStack;
                    #endif

                    item->currentStack -= items->at(i)->stackSize - items->at(i)->currentStack;
                    items->at(i)->currentStack = items->at(i)->stackSize;

                    #ifdef _DEBUG
                    printf("Added %i to stack in slot %i. Maxed it. Total to add is now %i.\n", added, i, item->currentStack);
                    #endif
                }
            }
        }
    }

    const std::vector<int> availableSlot = FindItem(nullptr);
    items->at(availableSlot.at(0)) = std::make_shared<BaseItem>(*item);

    #ifdef _DEBUG
    printf("Item was successfully added to a new slot (%i).\n", availableSlot.at(0));
    #endif

    success = true;
    return success;
}

bool InventoryLib::Inventory::AddItemToSlot(BaseItem* item, int slot)
{
    bool success = false;
    if (slot >= GetInventorySize() || slot < 0)
    {
        #ifdef _DEBUG
        printf("Slot outside inventory range! Slot: %i | InventorySize: %i \n", slot, GetInventorySize());
        #endif
        return false;
    }

    if (item == nullptr)
    {
        #ifdef _DEBUG
        printf("tried to add nullptr to inventory");
        #endif
        return false;
    }

    ClearPresortedVectors();
    if (items->at(slot) != nullptr)
    {
        if (*items->at(slot) != *item)
        {
            #ifdef _DEBUG
            printf("Theres already a different item in slot %i. ID: %s \n", slot, items->at(slot)->ID.c_str());
            #endif

            return false;
        }

        const int possibleToAdd = items->at(slot)->stackSize - items->at(slot)->currentStack;
        if (possibleToAdd >= item->currentStack)
        {
            items->at(slot)->currentStack += item->currentStack;

            #ifdef _DEBUG
            printf("Successfully increased stack of item in slot %i.\n", slot);
            #endif

            success = true;
            return success;
        }
        else
        {
            items->at(slot)->currentStack += possibleToAdd;
            item->currentStack -= possibleToAdd;

            #ifdef _DEBUG
            printf("Successfully filled stack of item in slot %i. The input item stack got reduced to %i.\n", slot, item->currentStack);
            #endif

            success = true;
            return success;
        }
    }

    items->at(slot) = std::make_shared<BaseItem>(*item);

    #ifdef _DEBUG
    printf("Successfully added it as a new item in slot %i.\n", slot);
    #endif

    success = true;
    return success;
}


bool InventoryLib::Inventory::RemoveItem(BaseItem* item, int amount)
{
    ClearPresortedVectors();

    if (amount == 0) return true;

    if (item == nullptr)
    {
        #ifdef _DEBUG
        printf("Invalid item input to remove from inventory!\n");
        #endif
        return false;
    }

    if(amount < 0)
    {
        for (SharedPtrBaseItem& invItem : *items)
        {
            if (invItem == nullptr) continue;

            if (*invItem == *item)
            {
                #ifdef _DEBUG
                printf("Successfully removed itemstack with ID %s.\n", item->ID.c_str());
                #endif

                invItem = nullptr;
                return true;
            }
        }
        return false;
    }

    std::vector<int> itemsToRemove{};
    int totalCount = 0;

    for (int i = 0; i < GetInventorySize(); i++)
    {
        if (items->at(i) == nullptr) continue;

        if (*items->at(i) == *item)
        {
            totalCount += items->at(i)->currentStack;
            itemsToRemove.push_back(i);
        }
    }

    if (totalCount < amount)
    {
        #ifdef _DEBUG
        printf("Not enough items of this kind: %s", item->ID.c_str());
        #endif
        return false;
    }

    for (const int removableSlot : itemsToRemove)
    {
        if (items->at(removableSlot)->currentStack >= amount)
        {
            #ifdef _DEBUG
            printf("Removed %i items from stack in slot %i. Went from %i to %i.\n", amount, removableSlot, items->at(removableSlot)->currentStack, items->at(removableSlot)->currentStack - amount);
            #endif
            items->at(removableSlot)->currentStack -= amount;
            return true;
        }

        if (items->at(removableSlot)->currentStack < amount)
        {
            amount -= items->at(removableSlot)->currentStack;
            items->at(removableSlot) = nullptr;

            #ifdef _DEBUG
            printf("Removed the whole stack in slot %i. \n", removableSlot);
            #endif
        }
    }

    return false;
}

bool InventoryLib::Inventory::RemoveItemInSlot(int slot, int amount)
{
    ClearPresortedVectors();

    if (!IsItemInSlotValid(slot))
    {
        return false;
    }

    if (amount == 0)
    {
        #ifdef _DEBUG
        printf("Amount to remove was 0, successfully removed nothing.\n");
        #endif
        return true;
    }

    if(amount < 0)
    {
        #ifdef _DEBUG
        printf("Successfully removed item stack in slot %i.\n", slot);
        #endif
        items->at(slot) = nullptr;
        return true;
    }

    if(amount > items->at(slot)->currentStack)
    {
        #ifdef _DEBUG
        printf("Item stack in slot %i does not contain %i items\n", slot, amount);
        #endif
        return false;
    }

    if(amount < items->at(slot)->currentStack)
    {
        #ifdef _DEBUG
        printf("Successfully removed all items in slot %i.\n", slot);
        #endif
        items->at(slot)->currentStack -= amount;
        return true;
    }

    #ifdef _DEBUG
    printf("Successfully removed all items in slot %i.\n", slot);
    #endif
    items->at(slot) = nullptr;
    return true;
}


void InventoryLib::Inventory::Sort(std::function<bool(const SharedPtrBaseItem&, const SharedPtrBaseItem&)> comparison)
{
    for (int i = 1; i < GetInventorySize(); i++)
    {
        for (int j = 0; j < GetInventorySize() - i; j++)
        {
            if(comparison(items->at(j), items->at(j+1)))
            {
                MoveItemToSlot(j, j + 1);
            }
        }
    }
}

void InventoryLib::Inventory::SortByName(bool ascending)
{
    if (itemsSortedByName != nullptr && lastNameSortWasAscending == ascending)
    {
        *items = *itemsSortedByName;
        return;
    }
    lastNameSortWasAscending = ascending;

    auto sorting = [ascending](const SharedPtrBaseItem& AItem, const SharedPtrBaseItem& BItem) -> bool
        {
            if (AItem == nullptr) return true;
            if (BItem == nullptr) return false;

            if (AItem->name == BItem->name)
            {
                if (AItem->currentStack < BItem->currentStack)
                {
                    return true;
                }
            }

            if(ascending)
            {
                if (IsStringGreater(AItem->name, BItem->name))
                {
                    return true;
                }
            }
            else
            {
                if (!IsStringGreater(AItem->name, BItem->name))
                {
                    return true;
                }
            }

            return false;
        };

    Sort(sorting);
    itemsSortedByName = std::make_unique<BaseItemVector>(*items);
}

void InventoryLib::Inventory::SortByTag(bool ascending) //first tries to sort by tag name, than by name and than by stack size
{
    if (itemsSortedByTag != nullptr && lastTagSortWasAscending == ascending)
    {
        *items = *itemsSortedByTag;
        return;
    }
    lastTagSortWasAscending = ascending;

    auto sorting = [ascending](const SharedPtrBaseItem& AItem, const SharedPtrBaseItem& BItem) -> bool
        {
            if (AItem == nullptr)
            {
                return true;
            }
            if (BItem == nullptr) return false;

            if (AItem->tag == BItem->tag)
            {
                if (AItem->name == BItem->name &&
                    AItem->currentStack < BItem->currentStack)
                {
                    return true;
                }

                if (IsStringGreater(AItem->name, BItem->name))
                {
                    return true;
                }
            }

            if(ascending)
            {
                if (IsStringGreater(AItem->tag, BItem->tag))
                {
                    return true;
                }
            }
            else
            {
                if (!IsStringGreater(AItem->tag, BItem->tag))
                {
                    return true;
                }
            }

            return false;
        };

    Sort(sorting);

    itemsSortedByTag = std::make_unique<BaseItemVector>(*items);
}

void InventoryLib::Inventory::SortByStack(bool ascending)
{
    if (itemsSortedByStack != nullptr && lastStackSortWasAscending == ascending)
    {
        *items = *itemsSortedByStack;
        return;
    }
    lastStackSortWasAscending = ascending;

    auto ascendingSort = [ascending](const SharedPtrBaseItem& AItem, const SharedPtrBaseItem& BItem) -> bool
        {
            if (AItem == nullptr)
            {
                return true;
            }
            if (BItem == nullptr) return false;

            if (AItem->currentStack == BItem->currentStack)
            {
                if (AItem->name == BItem->name &&
                    AItem->stackSize < BItem->stackSize)
                {
                    return true;
                }

                if (IsStringGreater(AItem->name, BItem->name))
                {
                    return true;
                }
            }

            if(ascending)
            {
                if (AItem->currentStack > BItem->currentStack)
                {
                    return true;
                }
            }
            else
            {
                if (AItem->currentStack < BItem->currentStack)
                {
                    return true;
                }
            }

            return false;
        };

    Sort(ascendingSort);

    itemsSortedByStack = std::make_unique<BaseItemVector>(*items);
}

std::vector<InventoryLib::SharedPtrBaseItem> InventoryLib::Inventory::GetAllItemsByComparison(
    std::function<bool(const SharedPtrBaseItem& items)> comparison) const
{
    std::vector<SharedPtrBaseItem> retVal{};

    for (SharedPtrBaseItem item : *items)
    {
        if(comparison(item))
        {
            retVal.push_back(item);
        }
    }

    return retVal;
}

std::vector<InventoryLib::SharedPtrBaseItem> InventoryLib::Inventory::GetAllItemsWithTag(std::string tag) const
{
    auto comparison = [tag](const SharedPtrBaseItem item) -> bool
        {
            if (item == nullptr) return false;
            return item->tag == tag;
        };

    return GetAllItemsByComparison(comparison);
}

std::vector<InventoryLib::SharedPtrBaseItem> InventoryLib::Inventory::GetAllItemsThatContain(std::string part) const
{
    auto comparison = [part](const SharedPtrBaseItem item) -> bool
        {
            if (item == nullptr) return false;
            if (item->name.find(part)) return true;
            if (item->tag.find(part)) return true;
            if (std::to_string(item->currentStack).find(part)) return true;
            if (std::to_string(item->stackSize).find(part)) return true;
            if (std::to_string(item->weightPerItem).find(part)) return true;
            if (std::to_string(item->WeightOfStack()).find(part)) return true;
            return false;
        };

    return GetAllItemsByComparison(comparison);
}

void InventoryLib::Inventory::MoveItemToSlot(int pos, int pos2)
{
    if (!IsSlotValid(pos))
    {
        #ifdef _DEBUG
        printf("Slot is invalid: %i", pos);
        #endif
        return;
    }

    if (!IsSlotValid(pos2))
    {
        #ifdef _DEBUG
        printf("Slot is invalid: %i", pos2);
        #endif
        return;
    }

    SharedPtrBaseItem temp = items->at(pos);
    items->at(pos) = items->at(pos2);
    items->at(pos2) = temp;
}

void InventoryLib::Inventory::ClearPresortedVectors()
{
    itemsSortedByName = nullptr;
    itemsSortedByStack = nullptr;
    itemsSortedByTag = nullptr;
}


std::string InventoryLib::Inventory::GetInventoryStructure(bool readable) const
{
    std::string retVal = "";

    for (int i = 0; i < GetInventorySize(); i++)
    {

        if (items->at(i) == nullptr) continue;
        if (!items->at(i)->IsValid()) continue;
        std::string temp = GetItemAsString(*items->at(i), readable);
        if (temp.empty())  continue;
        retVal += temp;
    }

    return retVal;
}

std::string InventoryLib::Inventory::GetItemAsString(BaseItem item, bool readable) const
{
    std::string retVal = "";

    if (!item.IsValid()) return retVal;

    if (readable)
    {
        retVal += "{\n";

        retVal += "\tItemID:" + item.ID + ":\n" +
            "\t{\n";

        retVal += "\t\tName:" + item.name + ";\n" +
            "\t\tTag:" + item.tag + ";\n" +
            "\t\tStackSize:" + std::to_string(item.stackSize) + ";\n" +
            "\t\tCurrentStack:" + std::to_string(item.currentStack) + ";\n";

        retVal += "\t}\n";
        retVal += "}\n";
    }
    else
    {
        retVal += "{ItemID:" + item.ID + ": " +
            "{Name:" + item.name + "; Tag:" + item.tag +
            "; StackSize:" + std::to_string(item.stackSize) +
            "; CurrentStack:" + std::to_string(item.currentStack) +
            ";}}\n";
    }
    
    return retVal;
}

std::vector<int> InventoryLib::Inventory::FindItem(BaseItem* item, bool allowFullStacks) const
{
    std::vector<int> retVal{};

    for(int i = 0; i < GetInventorySize(); i++)
    {
        if(item == nullptr)
        {
            if(items->at(i) == nullptr)
            {
                retVal.push_back(i);
            }
        }
        else if(*items->at(i) == *item)
        {
            if(!allowFullStacks && item != nullptr)
            {
                if(items->at(i)->IsStackFull())
                {
                    continue;
                }
            }
            retVal.push_back(i);
        }
    }
    return retVal;
}

bool InventoryLib::Inventory::HasItem(BaseItem* item, int amount) const
{
    int count = 0;

    for(int i = 0; i< GetInventorySize(); i++)
    {
        if(item == nullptr)
        {
            if(items->at(i) == nullptr) return true;
        }
        else if(*items->at(i) == *item)
        {
            count += items->at(i)->currentStack;
        }
    }

    return count >= amount;
}

std::vector<int> InventoryLib::Inventory::GetSlotsWithItem(BaseItem* item) const
{
    if (!item) return {};

    std::vector<int> retVal{};

    for (int i = 0; i < GetInventorySize(); i++)
    {
        if (items->at(i) == nullptr) continue;

        if (*items->at(i) == *item)
        {
            retVal.push_back(i);
        }
    }

    return retVal;
}

bool InventoryLib::Inventory::HasEnoughSpaceToAddItem(BaseItem* item) const
{
    if(HasItem(nullptr))
    {
        return true;
    }

    if(!HasItem(item))
    {
        return false;
    }

    std::vector<int> slotsWithItem = GetSlotsWithItem(item);
    int spaceAvailable = 0;

    for (int slot : slotsWithItem)
    {
        spaceAvailable += item->stackSize - items->at(slot)->currentStack;
    }

    return spaceAvailable > item->currentStack;
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

InventoryLib::SharedPtrBaseItem InventoryLib::Inventory::GetItemInSlot(int slot) const
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
        return nullptr;
    }

    return items->at(slot);
}

float InventoryLib::Inventory::GetCurrentCarryingWeight() const
{
    float retVal = 0.0f;
    for (const SharedPtrBaseItem& item : *items)
    {
        if(item != nullptr)
        {
            retVal += item->WeightOfStack();
        }
    }
    return retVal;
}

bool InventoryLib::Inventory::IsItemInSlotValid(int slot) const
{
    if (slot >= GetInventorySize() || slot < 0)
    {
        #ifdef _DEBUG
        printf("Slot out of range. Slot: %i | Inventory size: %i", slot, GetInventorySize());
        #endif

        return false;
    }

    if (items->at(slot) == nullptr)
    {
        #ifdef _DEBUG
        printf("Item in slot %i is already empty", slot);
        #endif

        return false;
    }

    if (!items->at(slot)->IsValid())
    {
        #ifdef _DEBUG
        printf("Item in slot %i is invalid", slot);
        #endif

        return false;
    }

    return true;
}

bool InventoryLib::Inventory::IsSlotValid(int slot) const
{
    if (slot >= GetInventorySize() || slot < 0)
    {
        #ifdef _DEBUG
        printf("Slot out of range. Slot: %i | Inventory size: %i", slot, GetInventorySize());
        #endif

        return false;
    }

    return true;
}




