// DropItem.cpp

#include "DropItem.h"
#include <iostream>
using namespace std;

DropItem::DropItem(string itemName)
{
	name = itemName;
}

string DropItem::getName() const
{
	return name;
}

int DropItem::getPrice() const
{
	return 50;
}

void DropItem::use(PlayerCharacter* PlayerCharacter)
{
	cout << name << "은(는) 사용할 수 없습니다." << endl;;
}