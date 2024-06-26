#pragma once

#include <string>

namespace InventoryLib
{
    class BaseItem
    {
    public:
        BaseItem();
        BaseItem(const BaseItem& other);
        BaseItem(std::string newName, std::string newTag, std::string newID, int newStackSize = 50, int newCurrentStack = 1, float newWeightPerItem = 0.f);
        virtual ~BaseItem();

        std::string name = "None";

        const std::string ID = "000000-000000-000000";

        const std::string tag = "None";

        const int stackSize = 64;

        int currentStack = 1;

        float weightPerItem = -1.0f;

        virtual bool operator== (const BaseItem& rhs)
        {
            return ID == rhs.ID && name == rhs.name && tag == rhs.tag;
        }
        virtual bool operator!= (const BaseItem& rhs)
        {
            return !(ID == rhs.ID);
        }
        virtual bool operator< (const BaseItem& rhs)
        {
            return currentStack < rhs.currentStack;
        }
        virtual bool operator> (const BaseItem& rhs)
        {
            return currentStack > rhs.currentStack;
        }
        virtual bool operator<= (const BaseItem& rhs)
        {
            return !(currentStack > rhs.currentStack);
        }
        virtual bool operator>= (const BaseItem& rhs)
        {
            return !(currentStack < rhs.currentStack);
        }


        bool IsValid() const
        {
            if (this == nullptr) return false;
            return currentStack > 0 && stackSize > 0 && !ID.empty() && !name.empty() && !tag.empty();
        }
        virtual bool IsStackFull() const
        {
            return stackSize == currentStack;
        }
        virtual float WeightOfStack() const
        {
            return weightPerItem * static_cast<float>(currentStack);
        }

        virtual std::string GetAsString(bool readable) const;
    };
}


