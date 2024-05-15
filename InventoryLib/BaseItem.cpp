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
