#pragma once
#include <graphics.h>
#include"MyPVZAction.h"
using namespace std;

class PlantPea {
public:

	IMAGE PlantImage, PlantImageMove; //�㶹���ֵ�״̬��ͼƬ �� ��������ƶ�ʱ��ͼƬ
	int PlantHp;	// �㶹���ֵ�Ѫ��
	MyPVZAction PlantImageAction; // �㶹���ֵ�״̬
	int costSun ;

	PlantPea();// �㶹���ֵ��޲ι�����
	IMAGE getPlantImage();	// ����㶹���ֵ��㶹���ֵ�״̬��ͼƬ
	IMAGE getPlantImageMove(); //����㶹���ָ�������ƶ�ʱ��ͼƬ
	MyPVZAction getPlantImageStateAction(); // ����㶹���ֵĶ�������
	
	void setPlantImage();
};
