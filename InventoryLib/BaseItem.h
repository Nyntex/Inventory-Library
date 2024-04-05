#pragma once
#include <string>


namespace InventoryLib
{
    class BaseItem
    {
    public:
        BaseItem();
        ~BaseItem();

    private:
        std::string name = "None";

        const char ID[21] = "000000-000000-000000";

        int stacksize = 64;

    };
}


