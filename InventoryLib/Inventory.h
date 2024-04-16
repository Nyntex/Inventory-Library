#pragma once
#include "BaseItem.h"
#include <vector>


namespace InventoryLib
{
    class Inventory
    {
    public:
        Inventory();
        Inventory(int newSlotCount);
        Inventory(float newWeight);
        Inventory(int newSlotCount, float newWeight, bool useMaxSlotCount = true, bool useWeight = true);
        virtual ~Inventory();

#pragma region Member
    private: 
        float maxWeight = -1.0f;        // -1 deactivates maxWeight

        int maxSlots = -1;

        std::vector<BaseItem*>* items;

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

        virtual void RemoveItem(int slot);
        virtual void RemoveItem(int slot, BaseItem*& removedItem);
        virtual void RemoveItem(int slot, bool& success, BaseItem*& removedItem);

        virtual void SortByName(bool ascending = true);
        virtual void SortByTag(bool ascending = true);
        virtual void SortByStack(bool ascending = true);

        virtual std::string GetInventoryStructure(bool readable = true);

        virtual int FindItem(BaseItem* item, bool allowFullStacks = true);
        virtual bool HasItem(BaseItem* item, int*& slots, int amount = 1);

        bool IsStringGreater(const std::string& first, const std::string& second, int pos = 0);

        std::string MakeStringUpperCase(const std::string& word);

        virtual BaseItem* GetItemInSlot(int slot) const;

        virtual int GetInventorySize() const
        {
            return static_cast<int>(items->size());
        }

        virtual bool IsInventoryFull() const
        {
            for (BaseItem* item : *items)
            {
                if (item == nullptr) return false;
            }

            return true;
        }

        //Takes a lambda function which is optimised for a bubble sort
        virtual void Sort(bool (*comparison)(Inventory*,int,bool), bool ascending);

    private:
        void Reorder(int pos, int pos2);
#pragma endregion
        
    };
}
