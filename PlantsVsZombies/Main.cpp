#include<iostream>		// io��
#include"PlantPea.h" //�㶹��
#include"tools.h"		//������
#include <graphics.h>	//easyx �ⲿ��
#include"SunShine.h"	//������
#include"SunShine.cpp"	//������� cpp�ļ�
#include<cstdlib>		//C ���� ����char ���� strcpy
#include<mmsystem.h>	
#pragma comment(lib,"winmm.lib")
#include<time.h>		//ʱ����
#include"Plantsunflower.h"	//���տ���
#include"PlantNut.h"		//�����
#include"CommonZombies.h"	//��ͨ��ʬ��
#include<cstring>			//���� ���� memset
#include"PeaGrains.h"		//�㶹����

using namespace std;

/*
* ����ȫ�ֱ���
*/

/*����ֲ��*/
PlantPea pea;
PlantNut Nut;
PlantSunflower Flower;

/*���彩ʬ*/
CommonZombies  Zombies[6];

/*��ʬ����Ϊ*/
MyPVZAction ZombiesAction[5]; //���� ���� վ�� ����

IMAGE background , backgroundBar ;// ���ñ�������Ϊȫ�ֱ���

int MouseX, MouseY , MouseXY = 0; // ��� x , y λ�ú� �Ƿ�ѡ��ֲ�� ;

IMAGE MovePlant[16]; // ����ƶ��е�ֲ��
IMAGE StatePlant[16]; // ״̬���е�ֲ��
MyPVZAction GrowPlant[16]; // ֲ��Ķ���״̬
int Mytime = 0, Patch[16][16], PatchPlantPH[16][16], PrePlantPH[] = {0,100,100,3000}; // ��¼ѭ��������ֲ����ֲ���,ֲ��Ѫ�� 

SunShine SunShinePool[16]; // ��������
MyPVZAction SunShineImageAction; // ����֡����
int SunShinesoce = 50; //��������ֵ �� �ҳ�ʼΪ50
int CostSunShine[] = {0,100,50,50};//ֲ�����ĵ�����
int PatchGrainsMax[16][16];
int PatchLastLaunch[16][16];
int PlantActionNum[] = { 0,13 ,13,3};//
int PlantedTime[16][16];
int ZombiesDeadNum = 0;//�����Ľ�ʬ����
int AlreadyCreatZombiesNum = 0;
int TheGameResult;


/*�����㶹��*/

PeaGrains PeaGrainsPool[30];

/*���庯��*/

/*����ҳ����ת*/

void ThisGameResultScream(int result)
{
	switch (result) {
		/*
		* 1 ʤ��
		* 2 ʧ��
		* 3.����
		*/
	case 1: Sleep(2000); loadimage(0, "PlantsVsZombiesAllMaterial/res/win2.png"); break;
	case 2: Sleep(2000); loadimage(0, "PlantsVsZombiesAllMaterial/res/fail2.png"); break;
	case 3: break;

	}
}


/*����*/
int CheckisWin() {
	if (ZombiesDeadNum >= 30) {
		TheGameResult = 1; // winner
		return 1;
	}
	if (TheGameResult == 2) return 2 ;  // false

	return 0;
}

 /*����ҳ��*/
void startUI() {
	IMAGE imgBa, imgMenu1, imgMenu2;
	loadimage(&imgBa, "PlantsVsZombiesAllMaterial/res/menu.png");
	loadimage(&imgMenu1, "PlantsVsZombiesAllMaterial/res/menu1.png");
	loadimage(&imgMenu2, "PlantsVsZombiesAllMaterial/res/menu2.png");
	int flag = 0;

	while (true)
	{
		BeginBatchDraw();
		putimage(0, 0, &imgBa);
		putimagePNG(474, 75, flag ? &imgMenu2 : &imgMenu1);


		ExMessage msg;
		if (peekmessage(&msg))
		{
			if (msg.message == WM_LBUTTONDOWN &&
				msg.x > 474 && msg.x < 474 + 300 &&
				msg.y > 75 && msg.y < 75 + 140) {
				flag = 1;  //ѡ��
			}
			else if (msg.message == WM_LBUTTONUP && flag)
			{
				return;
			}
		}
		EndBatchDraw();
	}
}
bool checkPeaGrinsisBoom(PeaGrains *pg) {

	//if (pg->index >= 500) return true;

	/*�㶹��ը��*/
	for (int i = 1; i <= 5; i++) {
		// ���� �����Ƿ��н�ʬ 
		int y;// ������ 
		if (Zombies[i].survive != 0) {// ��ʬ���� �� �� i ����ʬ���� 
			y = (Zombies[i].indey - 15) / 95 + 1;// ���� ������ֵ ת  ��ֵ�� y ��ֵ 
			int x = pg->originx; 

			//cout << " here is boom indey " << pg->indey <<  << <<endl;
			if ((y - 1) * 95 + 95 == pg->indey) { // 
				if (Zombies[i].index - pg->index <= -65 && Zombies[i].index - pg->index >= -100) {
					Zombies[i].ZombieHp -= 20;	//��ʬѪ�� ���� 20 
					PatchGrainsMax[x][y]++; // x , y λ�ÿɷ�����㶹+1


					// ���㶹����ʼ��
					pg->index = 0; 
					pg->indey = 0;
					pg->originx = 0;
					pg->originy = 0;


					return true;
				}
			}
		}
	}

	return false;
}
void checkPeaGrains() { 
		for (int i = 1; i <= 5; i++) {
			if (Zombies[i].survive != 0) {
				int y = (Zombies[i].indey - 15 )/95 + 1;
				//cout << " here is found Zombies y is ==  " << y  << endl; 
				for (int x = 1; x <= 8; x++) {
					if (Patch[x][y] == 1 && Zombies[i].index <800 && (x-1)*80+260 < Zombies[i].index) {
						if (PatchGrainsMax[x][y] > 0) {
							//PatchLastLaunch[x][y] = (PatchLastLaunch[x][y]++) % 10;
							//cout << " here is already creat PeaGrains the PatchGrainsMax " << x << "-------" << y << endl;
							if ((PatchLastLaunch[x][y]++)%200 == 0) {
								for (int z = 0; z < 30; z++) {
									if (PeaGrainsPool[z].indey == 0) {
										PatchGrainsMax[x][y]--;
										cout << " here is already creat PeaGrains the z " << z << endl;
										PeaGrainsPool[z].indey = (y - 1) * 95 + 95;
										PeaGrainsPool[z].index = (x - 1) * 80 + 315;
										PeaGrainsPool[z].originx = x;
										PeaGrainsPool[z].originy = y;

										cout << " here is PeaGrains indx , indy " << PeaGrainsPool[z].index << "-------" << PeaGrainsPool[z].indey << endl;
										return;
									}
								}
							}

						}
					}
				}
			}	
		}
}


void checkPlantPH() { // ���ÿ��ֲ���Ѫ��
	for (int i = 1; i <= 8; i++) { //���� ֲ��
		for (int j = 1; j <= 5; j++) { // ����ֲ��
			if (Patch[i][j] != 0 && PatchPlantPH[i][j] <= 0) { //���ֲ�� Ѫ��С�� 0 ����ظ��˴� ������
				Patch[i][j] = 0, PatchPlantPH[i][j] = 0;
			}
		}
	}
}
void CreatZombiesAttackPlant(CommonZombies* zm) {
	/*��ʬ���� x , y λ��ֲ�� ��ֲ���Ѫ*/
	int x = (zm->index - 240) / 80 + 2, y = (zm->indey - 90 + 75) / 95 + 1;

	PatchPlantPH[x][y]--;
}
int checkZombiesAction(CommonZombies* zm) {
	//���� ���� վ�� ����
	int res = 4; // Ĭ���ǵ��ĸ���Ϊ
	int x = (zm->index-240)/80+2, y = (zm->indey - 90 + 75) / 95 + 1;

	if (zm->ZombieHp <= 0) {
		zm->survive++;
		if (zm->survive == 120) {
			zm->survive = 0;
			ZombiesDeadNum++;
		}
		return 2;
	}

	if (Patch[x][y] > 0) {
		//cout << " pause " << endl; 
		return 1;
	}
	if (zm->index <= 100) {
		TheGameResult = 2;
	}	

	return res;;
}

void CreatZombies() {
	if (AlreadyCreatZombiesNum >= 30) return;
	
	if ((Mytime + 500) % 1500 == 0) {
		for (int i = 1; i <= 5; i++) {
			if (Zombies[i].survive == 0) {
				AlreadyCreatZombiesNum++;
				Zombies[i].survive = 1;
				Zombies[i].indey = -75 + (rand()%5)*95 + 90 ;//(rand() % 5) * 95 ; //(j-1)*95+90
				//Zombies[i].indey = 300;
				Zombies[i].index =  900;
				Zombies[i].ZombieHp = 100;
				//Zombies[i].Suntimer = 0;
				cout << "here is CreatZombies " << Mytime << " i == " << i << endl;
				cout << "here is CreatZombies Zombies[i].indey " << Zombies[i].indey  << endl;
				return;
			}
			//cout << "here  break down  " << Mytime << endl;
		}

	}
	return;

}

void checkSunCreatTimer() {
	for (int i = 1; i <= 5; i++) { // �������� �е� ����ʱ�� Ϊ 1250 ��̫���� �����������ֵ��̫����ʼ��
		if (SunShinePool[i].Suntimer == 1250) { //
			cout << " Timer " << SunShinePool[i].Suntimer  << " i = " << i  << endl;
			SunShinePool[i].aimY = 0;
			SunShinePool[i].Suntimer = 0;
		}
	}
	/**
	for (int i = 1; i <= 8; i++) {
		for (int j = 1; j <= 5; j++) {
			if (Patch[i][j] == 2 && SunflowerProduceSunShine[i][j].Suntimer == 1250 ) {
				cout << "here is CheckfkowersSunShine " << endl;
				SunflowerProduceSunShine[i][j].aimY = 0;
				SunflowerProduceSunShine[i][j].Suntimer = 0;
			}
		}
	}*/


}


void CreatSunShine() {
	/*�����������*/
	if ( (Mytime + 300)  % 800  == 0 ) { // ����̫����ʱ��� �� Ϊ���õ�һ��̫�� ���ֵıȽ��磬�ʼ�������
		for (int i = 1; i <= 5 ; i++) { // ��������δ���ɵ�̫�� �� 
			if (SunShinePool[i].aimY == 0) { //δ���ɵı�־
				SunShinePool[i].aimY = 500;
				SunShinePool[i].indey = 0;
				SunShinePool[i].index = rand() % 640 + 180; //���λ�� 180 - 820 ֮��
				SunShinePool[i].Suntimer = 0;	// ��ʼ���������ʱ�� �� ����������
				cout << "here is creatsun " << Mytime << " i == " << i  << endl;
				break ;
			}
			//cout << "here  break down  " << Mytime << endl;
		}
	}

	/*̫������������*/
	
	for (int i = 1; i <= 8; i++) {
		for (int j = 1; j <= 5; j++) {
			if (Patch[i][j] == 2 && PlantedTime[i][j] % 1200 == 0) { //��λ����̫���� �� ����ֲʱ��Ϊ1200�ı�������������������
				for (int k = 6; k <= 15; k++) { // ������ص� 6 - 15 ����ѡ��һ��δ���ɵ�̫�����ɳ���
					if (SunShinePool[k].aimY == 0) {
						SunShinePool[k].aimY = (j - 1) * 95 + 90; // Ŀ�� y �� ����y ��ͬ��ʹ֮������
						SunShinePool[k].indey = (j - 1) * 95 + 90;
						SunShinePool[k].index = (i - 1) * 80 + 260+10; // x��λ�� ͨ������λ������λ
						SunShinePool[k].Suntimer = 1000;//����������ʧ��ʱ�䣨����1250����ʧ�ˣ�
						cout << "here is creatsun " << Mytime << " i == " << i << endl;
						return;
					}
					//cout << "here  break down  " << Mytime << endl;
				}
			}
		}
	}
	

	return;
}


void useClick() {
	/*
	* ���������¼���Ϣ 
	*/
	ExMessage message; // ��Ϣ��
	int indx = 0; // ��ⱻ�����ֲ���Ǽ���
	static int sta = 0; //�������Ƿ����˴�ѡ���ڵ�ֲ��
	if (peekmessage(&message)) { // �۲�����Ƿ�����

		if (message.message == WM_LBUTTONDOWN) { // �����갴�������
			cout << 1 << endl; //����̨ ��ӡ���� 1
			cout << Mytime<<endl ; //�����ǰ Mytime����ֵ

			/*���������˴�ѡ���е�ֲ��
			*��ȡ�������λ�� �ж��ǵڼ���ֲ�ﱻ����ˣ���¼�� MouseXY ��ȥ
			*/
			if (message.x >= 338 && message.x < 338 + 65 * 3 && message.y >= 6 && message.y <= 96 ) {
				indx = (message.x - 338) / 65 + 1;
				sta = 1;
				MouseXY = indx;
			}

			/*
			* �ж��Ƿ�����������
			*/

			for (int i = 1; i <= 15 ; i++) {
				if ( SunShinePool[i].aimY > 0&&message.x >= SunShinePool[i].index && message.x <= SunShinePool[i].index + 80 && message.y >= SunShinePool[i].indey && message.y <= SunShinePool[i].indey + 80) {
					SunShinePool[i].aimY = 0;
					SunShinesoce += 25;
					SunShinePool[i].Suntimer = 0;
					  //thread t(PlayVideo);
				}
			}

		}
		/* ��갴�������ƶ� ���ڵ�һ���жϳɹ�ʱ���޸ı��� sta 
		*	����갴�� �� �� sta Ϊ 1 ����� MouseX �� MouseY ʹֲ���������ƶ�
		*/
		else if (message.message == WM_MOUSEMOVE && sta == 1) {
			cout << 2;
			
			MouseY = message.y;
			MouseX = message.x;
		}
		/*�ж��������ɿ��¼�
		* �ɿ�ʱ�ж����λ�ã��������λ�ÿ���ֲ
		* �Ǿ�����ֲ�� ,����ԭ ���� MouseXY ��
		*/
		else if (message.message == WM_LBUTTONUP) {
			cout << 3 << endl;
			
			sta = 0;
			MouseY = message.y;
			MouseX = message.x;

			if (MouseX >= 260 && MouseY >= 110 && MouseX <= 900 && MouseY <= 560) {
				if (Patch[(MouseX - 260) / 80 + 1][(MouseY - 110) / 95 + 1] == 0 && SunShinesoce >= CostSunShine[MouseXY]) {
					Patch[(MouseX - 260) / 80 + 1][(MouseY - 110) / 95 + 1] = MouseXY;
					SunShinesoce -= CostSunShine[MouseXY];
					PatchPlantPH[(MouseX - 260) / 80 + 1][(MouseY - 110) / 95 + 1] = PrePlantPH[MouseXY];
					PlantedTime[(MouseX - 260) / 80 + 1][(MouseY - 110) / 95 + 1] = 1;
				}
			}
			MouseXY = 0;

		}

	}

	return; 
}

void UpdateWindows() {
	/*
	* ����windows���ڽ���
	*/
	BeginBatchDraw(); // ����˫���� �ȴ��ڴ�������
	
	putimagePNG(0, 0, &background); // ˢ�±���ͼ
	putimagePNG(260, 0, &backgroundBar); // ˢ�´�ѡ��
	
	for (int i = 1; i <= 3; i++) {
		putimagePNG(338+(i-1)*65, 6, &StatePlant[i]); // �ڴ�ѡ���з���ֲ��
	}
	
	
	if (MouseXY > 0) { //��� ���ѡ����ֲ���ֲ���������ƶ�
		putimagePNG(MouseX-65/2, MouseY-90/2, &MovePlant[MouseXY]);
	}

	/*
	* ����ÿ��λ�ã�ˢ��ÿ��λ���ϵ�ֲ��״̬
	*/
	checkPlantPH();
	for (int i = 1; i <=8; i++) {
		for (int j = 1; j <= 5; j++) {
			if (Patch[i][j] == 3) {
				/*�����������Ⱦ*/
				if (PatchPlantPH[i][j] < 250) {
					putimagePNG((i - 1) * 80 + 260, (j - 1) * 95 + 90, &GrowPlant[Patch[i][j]].Myaction[2]);
				}
				else if(PatchPlantPH[i][j] < 500){
					putimagePNG((i - 1) * 80 + 260, (j - 1) * 95 + 90, &GrowPlant[Patch[i][j]].Myaction[1]);
				}
				else {
					putimagePNG((i - 1) * 80 + 260, (j - 1) * 95 + 90, &GrowPlant[Patch[i][j]].Myaction[0]);
				}
				PlantedTime[i][j]++ ;
			} else if (Patch[i][j] != 0 ) {
				PlantedTime[i][j]++;
				int used = GrowPlant[Patch[i][j]].used;
				putimagePNG((i-1)*80+260,(j-1)*95+90, &GrowPlant[Patch[i][j]].Myaction[(Mytime)/10%PlantActionNum[Patch[i][j]]]); // (Mytime /64) / 13
			}
		}
	}
	/**
	/*�㶹�����㶹��*/
	checkPeaGrains();
	/*��Ⱦ�㶹��*/
	/*for (int i = 0; i < 30; i++) {
		if (PeaGrainsPool[i].indey > 0)
			if(!checkPeaGrinsisBoom(&PeaGrainsPool[i])) putimagePNG(++PeaGrainsPool[i].index, PeaGrainsPool[i].indey, &PeaGrainsPool[i].grains);
			else putimagePNG(PeaGrainsPool[i].index, PeaGrainsPool[i].indey, &PeaGrainsPool[i].boomgrains);
	}*/


	/*��Ⱦ�������*/
	checkSunCreatTimer();
	for (int i = 1; i <= 5 ; i++) {
		if (SunShinePool[i].aimY > 0) {
			SunShine* sun = &SunShinePool[i];
			if (sun->indey <= sun->aimY - 10) sun->indey = (sun->Suntimer++)/2;
			else {
				sun->indey = sun->aimY;
				sun->Suntimer++;
			}
			//(sun->Suntimer) += 1;
			putimagePNG(sun->index, sun->indey, &SunShineImageAction.Myaction[(sun->Suntimer) % 29]);
		}
	}
	/*��Ⱦ̫��������������*/
	for (int i = 6; i <= 15; i++) {
		 if(SunShinePool[i].aimY > 0) {
			// cout << "here is UpDate flowesCreatSunShine ; i == " << i  << endl;
			SunShine* sun = &SunShinePool[i];
			putimagePNG(sun->index, sun->indey, &SunShineImageAction.Myaction[(sun->Suntimer++) % 29]);
		}

	}

	/*��Ⱦ�����ʬ*/
	
	for (int i = 1; i <= 5; i++) {
		if (Zombies[i].survive > 0) { //�����ʬ����
			CommonZombies* zm = &Zombies[i];

			int j = checkZombiesAction(zm);

			switch (j) {
			case 1: CreatZombiesAttackPlant(zm); putimagePNG(zm->index, zm->indey, &ZombiesAction[1].Myaction[(Mytime)/10%21]);  break;
			case 2: putimagePNG(zm->index, zm->indey, &ZombiesAction[2].Myaction[(Mytime) / 10 % 21]); break;
			case 3: putimagePNG(zm->index, zm->indey, &ZombiesAction[3].Myaction[(Mytime) / 10 % 21]); break;
			case 4: if (Mytime % 4 == 0)putimagePNG(--zm->index, zm->indey, &ZombiesAction[4].Myaction[(Mytime) / 10 % 22]);
				  else 	putimagePNG(zm->index, zm->indey, &ZombiesAction[4].Myaction[(Mytime) / 10 % 22]);
					break;
			}
			

			//if (zm->index > 0 ) zm->index -= 1;
			//else sun->indey = sun->aimY;
			//(sun->Suntimer) += 1;
			//cout << "zm->indey is " << zm->indey << endl;
			//putimagePNG(zm->index, zm->indey, &ZombiesAction[4].Myaction[1]);
		}

	}


	/*�㶹�����㶹��*/
	checkPeaGrains();
	/*��Ⱦ�㶹��*/
	for (int i = 0; i < 30; i++) {
		if (PeaGrainsPool[i].indey > 0)
			if (!checkPeaGrinsisBoom(&PeaGrainsPool[i])) putimagePNG(++PeaGrainsPool[i].index, PeaGrainsPool[i].indey, &PeaGrainsPool[i].grains);
			else putimagePNG(PeaGrainsPool[i].index, PeaGrainsPool[i].indey, &PeaGrainsPool[i].boomgrains);
	}


	/*��Ⱦ������ֵ*/

	char Sunshinesoce[16];
	char ZombiesDeadNum1[16], ZombiesDeadNum2[16], ZombiesDeadNum3[16];
	sprintf_s(Sunshinesoce, sizeof(Sunshinesoce), "%d", SunShinesoce);
	sprintf_s(ZombiesDeadNum1, sizeof(ZombiesDeadNum1), "%d", ZombiesDeadNum);
	sprintf_s(ZombiesDeadNum2, sizeof(ZombiesDeadNum2), "%c", '/');
	sprintf_s(ZombiesDeadNum3, sizeof(ZombiesDeadNum3), "%d", 30);

	outtextxy(286, 67, Sunshinesoce);
	outtextxy(770, 30, ZombiesDeadNum1);
	outtextxy(790, 30, ZombiesDeadNum2);
	outtextxy(800, 30, ZombiesDeadNum3);

	EndBatchDraw(); //�ر�˫���� �ȴ��ڴ�������
}

void initialize() {

	cout << " here is  initialize() " << endl;

	memset(PlantedTime, 0, sizeof(PlantedTime));
	for (int i = 1; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			PatchGrainsMax[i][j] = 3;
		}
	}
	memset(PatchLastLaunch, 0, sizeof(PatchLastLaunch));


	initgraph(900, 600,1); // ����windows ����� 900* 600 �ұ�������̨

	/*�����뱳��ͼƬ*/

	loadimage(&background, "PlantsVsZombiesAllMaterial/res/Map/map0.jpg");
	loadimage(&backgroundBar, "PlantsVsZombiesAllMaterial/res/bar5.png"); 
	
	/*��������ѡ����ֲ����*/


	/*��ֲ��Ԥ����*/
	for (int i = 1; i <= 3; i++) {
		cout << pea.PlantImageAction.used << endl;
		cout << Flower.PlantImageAction.used << endl;
		cout << Nut.PlantImageAction.used << endl;
		switch (i) {
		case 1: MovePlant[i] = pea.getPlantImageMove(); //���ֲ���������ͼƬ
				StatePlant[i] = pea.getPlantImage();	//���ֲ���ڴ�ѡ���е�ͼƬ
				GrowPlant[i].getMyAction(&pea.PlantImageAction); //���ֲ�����Ϊ���� Action 
				break;
		case 2:	MovePlant[i] = Flower.getPlantImageMove(); //���ֲ���������ͼƬ
				StatePlant[i] = Flower.getPlantImage();	//���ֲ���ڴ�ѡ���е�ͼƬ
				GrowPlant[i].getMyAction(&Flower.PlantImageAction); //���ֲ�����Ϊ���� Action 
				break;
		case 3:	MovePlant[i] = Nut.getPlantImageMove(); //���ֲ���������ͼƬ
				StatePlant[i] = Nut.getPlantImage();	//���ֲ���ڴ�ѡ���е�ͼƬ
				GrowPlant[i].getMyAction(&Nut.PlantImageAction); //���ֲ�����Ϊ���� Action 
				break;
		}
	}

	/*��ʼ����ʬ��Ϊ*/

	
	ZombiesAction[1].getMyAction(&Zombies[1].ZombieAttackAction);
	ZombiesAction[2].getMyAction(&Zombies[1].ZombieDeadAction);
	ZombiesAction[3].getMyAction(&Zombies[1].ZombieStandAction);
	ZombiesAction[4].getMyAction(&Zombies[1].ZombieWalkAction);

	/*�����ʼ��*/

	char ImagePath[64];
	SunShineImageAction.used = 29;
	for (int i = 0; i < SunShineImageAction.used; i++) {
		sprintf_s(ImagePath, sizeof(ImagePath), "PlantsVsZombiesAllMaterial/res/sunshine/%d.png", i + 1);
		loadimage(&SunShineImageAction.Myaction[i], ImagePath);
	}

	/*��������*/

	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 30;
	f.lfWeight = 15;
	strcpy_s(f.lfFaceName, "Segoe UI Black");
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	setcolor(BLACK);



	return;
}

int main() {

	//
	initialize();//Ԥ�ȼ���
	int tmer = 0;
	//thread t(PlayVideo);
	//t.join();

	startUI();

	while (true) {
		/*
		* ����ѭ��������ˢ��ҳ��
		*/
		//PlayVideo();

		Mytime = Mytime++ % 100000000;
		//Mytime = Mytime / 7;
		//tmer += getDelay();


		/*
		*  ���ѭ���Ĵ������Դ�������ͼƬ��ˢ��
		*	ģ1 ����Ϊ�˷�ֹ Mytime��int
		*/


		/*������ߵ���Ҫԭ��֮һ���ǣ��Ҹ��˾��� update ֻӦ������ˢ�� windows ҳ�棬����������Ǵ�������ͽ�ʬ��*/
		CreatSunShine();
		CreatZombies();
		

		UpdateWindows();
		useClick();

		if (CheckisWin()) {
			break;
		}
	}

	//TheGameResult = 1; //ȡ������ע�� ���õ������ߵ����� ������ The Power

	ThisGameResultScream(TheGameResult);
	/*
	switch (TheGameResult){
	case 1: cout << " this game winner is player  " << endl;  break; // ��һ�ʤ 
	case 2:	cout << " this game winner is Zombies  " << endl; break; // ��ʬ��ʤ
	
	}*/

	system("pause");

	return 0 ;
}
