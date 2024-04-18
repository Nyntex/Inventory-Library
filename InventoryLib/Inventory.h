#pragma once

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
#pragma endregion

#pragma region Member
    private:
        int maxSlots = -1;// -1 deactivates max slots and the inventory can grow almost indefinitely
        bool autoResize = false; //whether to add new slots when the last slot is getting filled

        //TODO: MAKE INVENTORY USE WEIGHT
        float maxCarryWeight = -1.0f;// <0 deactivates maxCarryWeight
        float weight = -1.0f;       //the weight of the inventory


        UniquePtrBaseItemVector items;

#pragma endregion


#pragma region Functions
    public:
        virtual bool SetSlotCount(int newSlotCount); //returns whether it successfully changed it's size

        virtual bool AddItem(BaseItem* item);
        virtual bool AddItem(BaseItem* item, int slot);

        virtual bool RemoveItem(BaseItem* item);
        virtual bool RemoveItem(BaseItem* item, int amount);

        virtual bool RemoveItemInSlot(int slot);

        virtual void SortByName(bool ascending = true);
        virtual void SortByTag(bool ascending = true);
        virtual void SortByStack(bool ascending = true);


        virtual SharedPtrBaseItem GetItemInSlot(int slot) const;
        virtual std::string GetInventoryStructure(bool readable = true) const;
        virtual std::vector<int> FindItem(BaseItem* item, bool allowFullStacks = true) const;
        virtual bool HasItem(BaseItem* item, std::vector<int>& slots, int amount = 1) const;
        virtual bool HasEnoughSpaceToAddItem(BaseItem* item);
        virtual int GetInventorySize() const
        {
            return static_cast<int>(items->size());
        }
        virtual bool IsInventoryFull() const
        {
            for (const SharedPtrBaseItem& item : *items)
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
