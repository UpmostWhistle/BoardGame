#pragma once
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <windows.h>
#include <vector>
#include <iostream>
#include <string>

#pragma comment(lib, "Strmiids.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "xinput.lib")
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}

using namespace std;

class Item
{

private:
	string		name;			// name of the Item
	int			itemAmount;		// how many of the item
	bool		gotItem;		//	check if player has item						
	bool		renderItem;		// render item  if player has it in the hud/ inventory list
	int itemAmountReceived[15];	// store the quantity of the item maximum amount array 

 public:
	 std::vector<string*> newItem;  // store the name of the item
	 // need to create item
	Item();
	Item(string itemName, int itemAmount);
	
		
	// need to name the item

	//set name of item
	void setItemName(string *s);

	//return the item name
	string getItemName();

	// Check if player has possesition
	bool PlayerGotItem(bool gotItem);

	//If item created add to array/vector list of items
	void AddItemToArray(bool gotItem);

	//have hud render if the player has item
	bool RenderItem(bool renderItem);


	~Item(void);
};

