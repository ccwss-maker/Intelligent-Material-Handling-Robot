#ifndef Transport_h
#define Transport_h
#include <main.h>
#include "Arm.h"
enum{
	Ver_First,
	Hor_First
};

enum{
	Front,
	Back,
	Left,
	Right,
};

enum{
	Detecting_Left,
	Detecting_Mid,
	Detecting_Right
};

typedef struct{
	bool sign_Running;
	uint8_t Camera_Object_Detecting_Num; 	/*正在检测的物料编号*/
	uint8_t Camera_Object_Detect[3]; 		/*原料区物体颜色顺序*/
	uint8_t Camera_Object_Order[3]; 		/*搬运顺序*/
	Point3f_ Point_Car[3];					/*物料在小车上摆放的位置*/
}Transport_;

void Transport_Stop();
void Transport_main();
void Transport_Init();

extern Transport_ Transport;
#endif
