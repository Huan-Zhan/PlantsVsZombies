#pragma once
#include<iostream>
#include <graphics.h>
using namespace std;

class MyPVZAction {
public :
	MyPVZAction();
	int used;//�����˶��ٸ�ͼƬ
	IMAGE Myaction[32];
	void getMyAction(MyPVZAction* a);
};
