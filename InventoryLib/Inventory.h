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
        Inventory(int newSlotCount, float newWeight);
        virtual ~Inventory();

#pragma region Member
    private: 
        float maxWeight = -1.0f;        // -1 to deactivates maxWeight

        std::vector<BaseItem*>* items;  // using std::vector as I don't know if the vector might grow or not

#pragma endregion


#pragma region Functions
    public:
        virtual void SetSlotCount(int newSlotCount, bool& success);
        virtual void SetSlotCount(int newSlotCount);

        //consider adding functionality to this
        virtual void AddItem(BaseItem* item);
        virtual void AddItem(BaseItem* item, int slot);
        virtual void AddItem(BaseItem* item, bool& success);
        virtual void AddItem(BaseItem* item, int slot, bool& success);

        virtual BaseItem* GetItemBySlot(int slot) const;

        virtual int GetInventorySize() const
        {
            return static_cast<int>(items->size());
        }

        virtual bool IsInventoryFull() const
        {
            for (BaseItem* item : items)
            {
                if (item == nullptr) return false;
            }

            return true;
        }

#pragma endregion
        
    };
}
