#pragma once

#include <memory>
#include "BaseItem.h"
#include <vector>
#include <string>


namespace InventoryLib
{
    typedef std::shared_ptr<BaseItem> SharedBaseItem;
    typedef std::vector<SharedBaseItem> BaseItemVector;
    typedef std::unique_ptr<BaseItemVector> UniqueBaseItemVector;

    std::string MakeStringUpperCase(const std::string& word);
    bool IsStringGreater(const std::string& first, const std::string& second, int pos = 0);

    class Inventory
    {
    public:
        Inventory();
        Inventory(int newSlotCount);
        Inventory(float newWeight);
        Inventory(int newSlotCount, float newWeight, int maxSlots = -1, float maxWeight = -1.0f);
        Inventory(const Inventory& other); // copy constructor will copy the values of the items and not their shared pointer
        virtual ~Inventory();

        //copy assignment operator will copy the values of the items and not their shared pointer
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

#pragma region Member
    private:
        //TODO: MAKE INVENTORY USE WEIGHT
        float maxCarryWeight = -1.0f;// 0> deactivates maxCarryWeight
        
        int maxSlots = -1;// -1 deactivates max slots and the inventory can grow almost indefinitely

        UniqueBaseItemVector items;

#pragma endregion


#pragma region Functions
    public:
        virtual void SetSlotCount(int newSlotCount, bool& success);
        virtual void SetSlotCount(int newSlotCount);

        virtual void AddItem(BaseItem* item);
        virtual void AddItem(BaseItem* item, int slot);
        virtual void AddItem(BaseItem* item, bool& success);
        virtual void AddItem(BaseItem* item, int slot, bool& success);

        virtual void RemoveItem(BaseItem* item);
        virtual void RemoveItem(BaseItem* item, int amount);
        virtual void RemoveItem(BaseItem* item, bool& success);
        virtual void RemoveItem(BaseItem* item, int amount, bool& success);

        virtual void RemoveItemInSlot(int slot);
        virtual void RemoveItemInSlot(int slot, bool& success);

        virtual void SortByName(bool ascending = true);
        virtual void SortByTag(bool ascending = true);
        virtual void SortByStack(bool ascending = true);

        virtual std::string GetInventoryStructure(bool readable = true) const;

        virtual int FindItem(BaseItem* item, bool allowFullStacks = true) const;
        virtual bool HasItem(BaseItem* item, int*& slots, int amount = 1) const;

        virtual BaseItem* GetItemInSlot(int slot) const;

        virtual int GetInventorySize() const
        {
            return static_cast<int>(items->size());
        }

        virtual bool IsInventoryFull() const
        {
            for (const SharedBaseItem& item : *items)
            {
                if (item == nullptr) return false;
            }

            return true;
        }

        //Takes a lambda function which is optimised for a bubble sort
        virtual void Sort(bool (*comparison)(Inventory*,int,bool), bool ascending);

        void Reorder(int pos, int pos2);
#pragma endregion
        
    };
}
