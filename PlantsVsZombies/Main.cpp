#include<iostream>		// io类
#include"PlantPea.h" //豌豆类
#include"tools.h"		//工具类
#include <graphics.h>	//easyx 外部包
#include"SunShine.h"	//阳光类
#include"SunShine.cpp"	//阳光类的 cpp文件
#include<cstdlib>		//C 包类 调用char 函数 strcpy
#include<mmsystem.h>	
#pragma comment(lib,"winmm.lib")
#include<time.h>		//时间类
#include"Plantsunflower.h"	//向日葵类
#include"PlantNut.h"		//坚果类
#include"CommonZombies.h"	//普通僵尸类
#include<cstring>			//调用 函数 memset
#include"PeaGrains.h"		//豌豆粒类

using namespace std;

/*
* 定义全局变量
*/

/*定义植物*/
PlantPea pea;
PlantNut Nut;
PlantSunflower Flower;

/*定义僵尸*/
CommonZombies  Zombies[6];

/*僵尸的行为*/
MyPVZAction ZombiesAction[5]; //攻击 死亡 站立 行走

IMAGE background , backgroundBar ;// 设置背景数据为全局变量

int MouseX, MouseY , MouseXY = 0; // 鼠标 x , y 位置和 是否选中植物 ;

IMAGE MovePlant[16]; // 鼠标移动中的植物
IMAGE StatePlant[16]; // 状态栏中的植物
MyPVZAction GrowPlant[16]; // 植物的动作状态
int Mytime = 0, Patch[16][16], PatchPlantPH[16][16], PrePlantPH[] = {0,100,100,3000}; // 记录循环次数和植物种植情况,植物血量 

SunShine SunShinePool[16]; // 随机阳光池
MyPVZAction SunShineImageAction; // 阳光帧动画
int SunShinesoce = 50; //设置阳关值 ， 且初始为50
int CostSunShine[] = {0,100,50,50};//植物消耗的阳光
int PatchGrainsMax[16][16];
int PatchLastLaunch[16][16];
int PlantActionNum[] = { 0,13 ,13,3};//
int PlantedTime[16][16];
int ZombiesDeadNum = 0;//死亡的僵尸数量
int AlreadyCreatZombiesNum = 0;
int TheGameResult;


/*定义豌豆粒*/

PeaGrains PeaGrainsPool[30];

/*定义函数*/

/*结算页面跳转*/

void ThisGameResultScream(int result)
{
	switch (result) {
		/*
		* 1 胜利
		* 2 失败
		* 3.不管
		*/
	case 1: Sleep(2000); loadimage(0, "PlantsVsZombiesAllMaterial/res/win2.png"); break;
	case 2: Sleep(2000); loadimage(0, "PlantsVsZombiesAllMaterial/res/fail2.png"); break;
	case 3: break;

	}
}


/*结算*/
int CheckisWin() {
	if (ZombiesDeadNum >= 30) {
		TheGameResult = 1; // winner
		return 1;
	}
	if (TheGameResult == 2) return 2 ;  // false

	return 0;
}

 /*开场页面*/
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
				flag = 1;  //选中
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

	/*豌豆粒炸开*/
	for (int i = 1; i <= 5; i++) {
		// 遍历 五列是否有僵尸 
		int y;// 代表列 
		if (Zombies[i].survive != 0) {// 僵尸池中 ， 第 i 个僵尸存在 
			y = (Zombies[i].indey - 15) / 95 + 1;// 利用 像素数值 转  列值对 y 赋值 
			int x = pg->originx; 

			//cout << " here is boom indey " << pg->indey <<  << <<endl;
			if ((y - 1) * 95 + 95 == pg->indey) { // 
				if (Zombies[i].index - pg->index <= -65 && Zombies[i].index - pg->index >= -100) {
					Zombies[i].ZombieHp -= 20;	//僵尸血量 减少 20 
					PatchGrainsMax[x][y]++; // x , y 位置可发射的豌豆+1


					// 将豌豆粒初始化
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


void checkPlantPH() { // 检测每个植物的血量
	for (int i = 1; i <= 8; i++) { //八列 植物
		for (int j = 1; j <= 5; j++) { // 五行植物
			if (Patch[i][j] != 0 && PatchPlantPH[i][j] <= 0) { //如果植物 血量小于 0 ，则回复此处 的数据
				Patch[i][j] = 0, PatchPlantPH[i][j] = 0;
			}
		}
	}
}
void CreatZombiesAttackPlant(CommonZombies* zm) {
	/*僵尸攻击 x , y 位置植物 ，植物掉血*/
	int x = (zm->index - 240) / 80 + 2, y = (zm->indey - 90 + 75) / 95 + 1;

	PatchPlantPH[x][y]--;
}
int checkZombiesAction(CommonZombies* zm) {
	//攻击 死亡 站立 行走
	int res = 4; // 默认是第四个行为
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
	for (int i = 1; i <= 5; i++) { // 检测阳光池 中的 存在时间 为 1250 的太阳花 ，将等于这个值的太阳初始化
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
	/*生成随机阳光*/
	if ( (Mytime + 300)  % 800  == 0 ) { // 产生太阳的时间点 ， 为了让第一个太阳 出现的比较早，故加上三百
		for (int i = 1; i <= 5 ; i++) { // 遍历池中未生成的太阳 ， 
			if (SunShinePool[i].aimY == 0) { //未生成的标志
				SunShinePool[i].aimY = 500;
				SunShinePool[i].indey = 0;
				SunShinePool[i].index = rand() % 640 + 180; //随机位置 180 - 820 之间
				SunShinePool[i].Suntimer = 0;	// 初始化阳光存在时间 ， 后面有作用
				cout << "here is creatsun " << Mytime << " i == " << i  << endl;
				break ;
			}
			//cout << "here  break down  " << Mytime << endl;
		}
	}

	/*太阳花生成阳光*/
	
	for (int i = 1; i <= 8; i++) {
		for (int j = 1; j <= 5; j++) {
			if (Patch[i][j] == 2 && PlantedTime[i][j] % 1200 == 0) { //该位置是太阳花 ， 且种植时间为1200的倍数（产生阳光条件）
				for (int k = 6; k <= 15; k++) { // 在阳光池的 6 - 15 号中选择一个未生成的太阳生成出来
					if (SunShinePool[k].aimY == 0) {
						SunShinePool[k].aimY = (j - 1) * 95 + 90; // 目标 y 和 产生y 相同，使之不下落
						SunShinePool[k].indey = (j - 1) * 95 + 90;
						SunShinePool[k].index = (i - 1) * 80 + 260+10; // x的位置 通过花的位置来定位
						SunShinePool[k].Suntimer = 1000;//设置阳光消失的时间（到达1250就消失了）
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
	* 检测鼠标点击事件信息 
	*/
	ExMessage message; // 信息类
	int indx = 0; // 检测被点击的植物是几号
	static int sta = 0; //检测鼠标是否点击了待选框内的植物
	if (peekmessage(&message)) { // 观察鼠标是否点击了

		if (message.message == WM_LBUTTONDOWN) { // 如果鼠标按下了左键
			cout << 1 << endl; //控制台 打印数字 1
			cout << Mytime<<endl ; //输出当前 Mytime的数值

			/*如果鼠标点击了待选框中的植物
			*获取鼠标点击的位置 判断是第几个植物被点击了，记录到 MouseXY 中去
			*/
			if (message.x >= 338 && message.x < 338 + 65 * 3 && message.y >= 6 && message.y <= 96 ) {
				indx = (message.x - 338) / 65 + 1;
				sta = 1;
				MouseXY = indx;
			}

			/*
			* 判断是否点击到了阳光
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
		/* 鼠标按下且在移动 ，在第一个判断成功时，修改变量 sta 
		*	当鼠标按下 ， 且 sta 为 1 则更新 MouseX 和 MouseY 使植物跟随鼠标移动
		*/
		else if (message.message == WM_MOUSEMOVE && sta == 1) {
			cout << 2;
			
			MouseY = message.y;
			MouseX = message.x;
		}
		/*判断鼠标左键松开事件
		* 松开时判断鼠标位置，如果所在位置可种植
		* 那就种下植物 ,并还原 数据 MouseXY 和
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
	* 更新windows窗口界面
	*/
	BeginBatchDraw(); // 开启双缓冲 等待内存加载完毕
	
	putimagePNG(0, 0, &background); // 刷新背景图
	putimagePNG(260, 0, &backgroundBar); // 刷新待选框
	
	for (int i = 1; i <= 3; i++) {
		putimagePNG(338+(i-1)*65, 6, &StatePlant[i]); // 在待选框中放入植物
	}
	
	
	if (MouseXY > 0) { //如果 鼠标选中了植物，则植物跟随鼠标移动
		putimagePNG(MouseX-65/2, MouseY-90/2, &MovePlant[MouseXY]);
	}

	/*
	* 遍历每个位置，刷新每个位置上的植物状态
	*/
	checkPlantPH();
	for (int i = 1; i <=8; i++) {
		for (int j = 1; j <= 5; j++) {
			if (Patch[i][j] == 3) {
				/*坚果的特殊渲染*/
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
	/*豌豆发射豌豆粒*/
	checkPeaGrains();
	/*渲染豌豆粒*/
	/*for (int i = 0; i < 30; i++) {
		if (PeaGrainsPool[i].indey > 0)
			if(!checkPeaGrinsisBoom(&PeaGrainsPool[i])) putimagePNG(++PeaGrainsPool[i].index, PeaGrainsPool[i].indey, &PeaGrainsPool[i].grains);
			else putimagePNG(PeaGrainsPool[i].index, PeaGrainsPool[i].indey, &PeaGrainsPool[i].boomgrains);
	}*/


	/*渲染随机阳光*/
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
	/*渲染太阳花产生的阳光*/
	for (int i = 6; i <= 15; i++) {
		 if(SunShinePool[i].aimY > 0) {
			// cout << "here is UpDate flowesCreatSunShine ; i == " << i  << endl;
			SunShine* sun = &SunShinePool[i];
			putimagePNG(sun->index, sun->indey, &SunShineImageAction.Myaction[(sun->Suntimer++) % 29]);
		}

	}

	/*渲染随机僵尸*/
	
	for (int i = 1; i <= 5; i++) {
		if (Zombies[i].survive > 0) { //如果僵尸存在
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


	/*豌豆发射豌豆粒*/
	checkPeaGrains();
	/*渲染豌豆粒*/
	for (int i = 0; i < 30; i++) {
		if (PeaGrainsPool[i].indey > 0)
			if (!checkPeaGrinsisBoom(&PeaGrainsPool[i])) putimagePNG(++PeaGrainsPool[i].index, PeaGrainsPool[i].indey, &PeaGrainsPool[i].grains);
			else putimagePNG(PeaGrainsPool[i].index, PeaGrainsPool[i].indey, &PeaGrainsPool[i].boomgrains);
	}


	/*渲染阳光数值*/

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

	EndBatchDraw(); //关闭双缓冲 等待内存加载完毕
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


	initgraph(900, 600,1); // 生成windows 界面框 900* 600 且保留控制台

	/*加载入背景图片*/

	loadimage(&background, "PlantsVsZombiesAllMaterial/res/Map/map0.jpg");
	loadimage(&backgroundBar, "PlantsVsZombiesAllMaterial/res/bar5.png"); 
	
	/*加载事先选定的植物类*/


	/*将植物预处理*/
	for (int i = 1; i <= 3; i++) {
		cout << pea.PlantImageAction.used << endl;
		cout << Flower.PlantImageAction.used << endl;
		cout << Nut.PlantImageAction.used << endl;
		switch (i) {
		case 1: MovePlant[i] = pea.getPlantImageMove(); //获得植物跟随鼠标的图片
				StatePlant[i] = pea.getPlantImage();	//获得植物在待选框中的图片
				GrowPlant[i].getMyAction(&pea.PlantImageAction); //获得植物的行为对象 Action 
				break;
		case 2:	MovePlant[i] = Flower.getPlantImageMove(); //获得植物跟随鼠标的图片
				StatePlant[i] = Flower.getPlantImage();	//获得植物在待选框中的图片
				GrowPlant[i].getMyAction(&Flower.PlantImageAction); //获得植物的行为对象 Action 
				break;
		case 3:	MovePlant[i] = Nut.getPlantImageMove(); //获得植物跟随鼠标的图片
				StatePlant[i] = Nut.getPlantImage();	//获得植物在待选框中的图片
				GrowPlant[i].getMyAction(&Nut.PlantImageAction); //获得植物的行为对象 Action 
				break;
		}
	}

	/*初始化僵尸行为*/

	
	ZombiesAction[1].getMyAction(&Zombies[1].ZombieAttackAction);
	ZombiesAction[2].getMyAction(&Zombies[1].ZombieDeadAction);
	ZombiesAction[3].getMyAction(&Zombies[1].ZombieStandAction);
	ZombiesAction[4].getMyAction(&Zombies[1].ZombieWalkAction);

	/*阳光初始化*/

	char ImagePath[64];
	SunShineImageAction.used = 29;
	for (int i = 0; i < SunShineImageAction.used; i++) {
		sprintf_s(ImagePath, sizeof(ImagePath), "PlantsVsZombiesAllMaterial/res/sunshine/%d.png", i + 1);
		loadimage(&SunShineImageAction.Myaction[i], ImagePath);
	}

	/*设置字体*/

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
	initialize();//预先加载
	int tmer = 0;
	//thread t(PlayVideo);
	//t.join();

	startUI();

	while (true) {
		/*
		* 无限循环，不断刷新页面
		*/
		//PlayVideo();

		Mytime = Mytime++ % 100000000;
		//Mytime = Mytime / 7;
		//tmer += getDelay();


		/*
		*  检测循环的次数，以此来控制图片的刷新
		*	模1 亿是为了防止 Mytime爆int
		*/


		/*放在外边的主要原因之一就是，我个人觉得 update 只应该用来刷新 windows 页面，而这个函数是创建阳光和僵尸的*/
		CreatSunShine();
		CreatZombies();
		

		UpdateWindows();
		useClick();

		if (CheckisWin()) {
			break;
		}
	}

	//TheGameResult = 1; //取消该行注释 将得到开发者的力量 ！！！ The Power

	ThisGameResultScream(TheGameResult);
	/*
	switch (TheGameResult){
	case 1: cout << " this game winner is player  " << endl;  break; // 玩家获胜 
	case 2:	cout << " this game winner is Zombies  " << endl; break; // 僵尸获胜
	
	}*/

	system("pause");

	return 0 ;
}
