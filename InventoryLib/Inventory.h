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
        int slotCount = -1;             // -1 to deactivate slot count

        float weight = -1.0f;           // -1 to deactivates weight

        std::vector<BaseItem*>* items;  // using std::vector as I don't know if the vector might grow or not

#pragma endregion


#pragma region Functions
    public:
        virtual void SetSlotCount(int newSlotCount, bool& success);
        virtual void SetSlotCount(int newSlotCount);
        virtual BaseItem* GetItemBySlot(int slot);

#pragma endregion
        
    };
}
