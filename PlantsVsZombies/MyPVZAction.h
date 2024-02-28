#pragma once
#include<iostream>
#include <graphics.h>
using namespace std;

class MyPVZAction {
public :
	MyPVZAction();
	int used;//放置了多少个图片
	IMAGE Myaction[32];
	void getMyAction(MyPVZAction* a);
};
