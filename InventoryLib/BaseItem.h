#pragma once
#include <string>


namespace InventoryLib
{
    class BaseItem
    {
    public:
        BaseItem();
        BaseItem(BaseItem& other);
        BaseItem(std::string newName, std::string newID, int newStackSize, int newCurrentStack);
        BaseItem(std::string newName, std::string newTag, std::string newID, int newStackSize, int newCurrentStack);
        virtual ~BaseItem();

        std::string name = "None";

        const std::string tag = "None";

        const std::string ID = "000000-000000-000000";

        const int stackSize = 64;

        int currentStack = 1;

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

        virtual bool IsValid() const
        {
            bool validity = false;
            validity = currentStack > 0;
         

            return validity;
        }

        virtual bool IsStackFull()
        {
            return stackSize == currentStack;
        }
    };
}


