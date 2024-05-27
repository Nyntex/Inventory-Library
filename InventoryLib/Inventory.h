#pragma once

#include <functional>
#include <memory>
#include "BaseItem.h"
#include <vector>
#include <string>


namespace InventoryLib
{
#pragma region typedefs
    typedef std::shared_ptr<BaseItem> SharedPtrBaseItem;
    typedef std::vector<SharedPtrBaseItem> BaseItemVector;
    typedef std::unique_ptr<BaseItemVector> UniquePtrBaseItemVector;
#pragma endregion

    std::string MakeStringUpperCase(const std::string& word);
    bool IsStringGreater(const std::string& first, const std::string& second, int pos = 0);

    class Inventory
    {
    public:
#pragma region Constructor and operator
        Inventory(int newSlotCount = 1, float newWeight = -1, int maxSlots = -1, bool newAutoResize = false, float maxWeight = -1.0f);
        Inventory(const Inventory& other); // copy constructor will copy the values of the item and not their shared pointer
        virtual ~Inventory();

        Inventory& operator=(const Inventory& other)
        {
            if (this != &other)
            {
                maxCarryWeight = other.maxCarryWeight;
                maxSlots = other.maxSlots;

                items = std::make_unique<BaseItemVector>(other.GetInventorySize());

                for(int i = 0; i < other.GetInventorySize(); i++)
                {
                    items->at(i) = std::make_shared<BaseItem>(*other.items->at(i));
                }
            }

            return *this;
        }

        bool operator==(const Inventory& other) const
        {
            if (maxSlots != other.maxSlots || (int)maxCarryWeight != (int)other.maxCarryWeight) return false;
            if (GetInventorySize() != other.GetInventorySize()) return false;

            for(int i = 0; i < GetInventorySize(); i++)
            {
                if (items->at(i) != other.items->at(i)) return false;
            }

            return true;
        }
        bool operator!=(const Inventory& other) const
        {
            return !(*this == other);
        }

#pragma endregion

#pragma region Member
    protected:
        int maxSlots = -1;// -1 deactivates max slots and the inventory can grow almost indefinitely
        bool autoResize = false; //whether to add new slots when the last slot is getting filled

        float maxCarryWeight = -1.0f;// <0 deactivates maxCarryWeight
        float weight = 0.0f;       //the weight of the inventory

        UniquePtrBaseItemVector items;
        bool lastNameSortWasAscending = false;
        UniquePtrBaseItemVector itemsSortedByName;
        bool lastTagSortWasAscending = false;
        UniquePtrBaseItemVector itemsSortedByTag;
        bool lastStackSortWasAscending = false;
        UniquePtrBaseItemVector itemsSortedByStack;

#pragma endregion


#pragma region Functions
    public:
        
        virtual bool SetSlotCount(int newSlotCount);

        
        virtual bool AddItem(BaseItem* item);
        virtual bool AddItemToSlot(BaseItem* item, int slot);

        virtual bool RemoveItem(BaseItem* item, int amount = -1);
        virtual bool RemoveItemInSlot(int slot, int amount = -1);

        //Takes a lambda function which is optimised for a bubble sort
        virtual void Sort(std::function<bool(const SharedPtrBaseItem&, const SharedPtrBaseItem&)> comparison);
        virtual void SortByName(bool ascending = true);
        virtual void SortByTag(bool ascending = true);
        virtual void SortByStack(bool ascending = false);
        virtual std::vector<SharedPtrBaseItem> GetAllItemsByComparison(std::function<bool(const SharedPtrBaseItem& item)> comparison) const;
        virtual std::vector<SharedPtrBaseItem> GetAllItemsWithTag(std::string tag) const;
        virtual std::vector<SharedPtrBaseItem> GetAllItemsThatContain(std::string part) const;
        virtual void MoveItemToSlot(int itemPos, int newItemPos);
        virtual void ClearPresortedVectors();

        virtual SharedPtrBaseItem GetItemInSlot(int slot) const;
        virtual float GetCurrentCarryingWeight() const;
        virtual std::string GetInventoryStructure(bool readable = true) const;
        virtual std::string GetItemAsString(BaseItem item, bool readable) const;
        virtual std::vector<int> FindItem(BaseItem* item, bool allowFullStacks = true) const;
        virtual bool HasItem(BaseItem* item, int amount = 1) const;
        virtual std::vector<int> GetSlotsWithItem(BaseItem* item) const;
        virtual bool HasEnoughSpaceToAddItem(BaseItem* item) const;
        virtual int GetInventorySize() const
        {
            return static_cast<int>(items->size());
        }
        virtual bool IsInventoryFull(bool checkStacks = false) const
        {
            for (const SharedPtrBaseItem& item : *items)
            {
                if (item == nullptr) return false;
                if(checkStacks)
                {
                    if (!item->IsStackFull()) return false;
                }
            }

            return true;
        }
        virtual bool IsItemInSlotValid(int slot) const;
        virtual bool IsSlotValid(int slot) const;

#pragma endregion
        
    };
}
