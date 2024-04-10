#include "pch.h"
#include "BaseItem.h"

InventoryLib::BaseItem::BaseItem()
{
}

InventoryLib::BaseItem::BaseItem(BaseItem& other)
    :ID(other.ID), stackSize(other.stackSize)
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

InventoryLib::BaseItem::BaseItem(std::string newName, std::string newTag, std::string newID, int newStackSize, int newCurrentStack)
    : tag(newTag), ID(newID), stackSize(newStackSize)
{
    name = newName;
    currentStack = newCurrentStack;
}

InventoryLib::BaseItem::~BaseItem()
{
}
