#include "pch.h"
#include "BaseItem.h"

InventoryLib::BaseItem::BaseItem()
{
}

InventoryLib::BaseItem::BaseItem(const BaseItem& other)
    : ID(other.ID), tag(other.tag), stackSize(other.stackSize)
{
    name = other.name;
    currentStack = other.currentStack;
}

InventoryLib::BaseItem::BaseItem(std::string newName, std::string newID, int newStackSize, int newCurrentStack)
    : ID(newID), stackSize(newStackSize)
{
    name = newName;
    currentStack = newCurrentStack;
}

InventoryLib::BaseItem::BaseItem(std::string newName, std::string newTag, std::string newID, int newStackSize, int newCurrentStack, float newWeightPerItem)
    : ID(newID), tag(newTag), stackSize(newStackSize)
{
    name = newName;
    currentStack = newCurrentStack;
    weightPerItem = newWeightPerItem;
}

InventoryLib::BaseItem::~BaseItem()
{
}

std::string InventoryLib::BaseItem::GetAsString(bool readable) const
{
    std::string retVal{};

    if (!IsValid()) return retVal;

    if (readable)
    {
        retVal += "{\n";

        retVal += "\tItemID:" + ID + ":\n" +
            "\t{\n";

        retVal += "\t\tName:" + name + ";\n" +
            "\t\tTag:" + tag + ";\n" +
            "\t\tStackSize:" + std::to_string(stackSize) + ";\n" +
            "\t\tCurrentStack:" + std::to_string(currentStack) + ";\n";

        retVal += "\t}\n";
        retVal += "}\n";
    }
    else
    {
        retVal += "{ ItemID:" + ID + ": " +
            "{  Name:" + name + ";  Tag:" + tag +
            ";  StackSize:" + std::to_string(stackSize) +
            ";  CurrentStack:" + std::to_string(currentStack) +
            "; }}\n";
    }

    return retVal;
}
