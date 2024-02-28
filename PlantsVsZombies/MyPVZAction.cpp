
#include"MyPVZAction.h"

MyPVZAction::MyPVZAction() {
	used = 0;
}

void MyPVZAction::getMyAction(MyPVZAction* a) {
	for (int i = 0; i < a->used; i++) {
		Myaction[i] = a->Myaction[i];
	}
}