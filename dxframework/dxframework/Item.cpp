#include "Item.h"


Item::Item(void)
{
}

Item::Item(string itemName, int amountReceived)
	{
		itemName = name;
		amountReceived = itemAmount;
	}

void Item::setItemName(string *s)
{
	name = *s;
}

string Item::getItemName()
{
	return name;
}

bool Item::PlayerGotItem(bool gotItem)
{
	//set the player to get or not get item based on condition
	if(gotItem == true)
	{
		// add to vector list
	}
	else
		gotItem =  false;
	return gotItem;

}

void Item::AddItemToArray(bool gotItem)
{
	if(PlayerGotItem(gotItem))
	{
		// add to the array
	newItem.push_back(&name);
	}
}

//bool Item::RenderItem(bool renderItem)
//{
//
//}

Item::~Item(void)
{
}
