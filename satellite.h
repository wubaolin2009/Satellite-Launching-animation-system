#ifndef _SATELLITE_H
#define _SATELLITE_H


//初始化卫星的位置等
void SetParameter(double near = 318.55 + 300/2.0,double far = 800.0);

//每一帧要调用的函数
void OnEnterFrame();

//这里获得卫星的一些参数，和设置一些参数
//为了简便，我们将其作为一个静态类
//每次程序运行只有1颗卫星
//一些全局的状态信息也在这个类中，比如当前的视角
class SatelliteStatus
{
public:
	enum ViewPort{
		IN_ATMOSPHERE, //在大气层中
		OUTER_SPACE, //外太空,这个我们将摄像机初步定在-2000,2000,0这个点 look At 0，0，0
		ABOVE_NORTH_POLE, //北极上空，我们将摄像机初步定在 0,0,2000这个点，look At 0,0,0
		FREE_VIEW  //用键盘和鼠标来控制
	};  //当前的视角
	static ViewPort current_view_port;  //当前视角，默认为FREE_VIEW
	static float near_radius;  //近地轨道半径
	static float far_radius;   //远地轨道半径
	enum Status{  //卫星的状态
		UNLAUNCHED, //未发射
		RISING,  //上升中
		NEAR_TRACK, //近地轨道
		ECLLIPSE, //椭圆轨道
		FAR_TRACK, //远地轨道
		OUT_CONTROL //失控,这如果有时间可以实现下
	};
	static Status current_status; //默认为UNLAUNCHED
	static bool show_track;  //是否显示卫星的轨道，默认为false
	//卫星的坐标
	static float satellite_x;
	static float satellite_y;
	static float satellite_z;
	//卫星的自转角度，Y轴 Z X左手定则方向
	static float satellite_angle;
	//卫星线速度
	static float satellite_lv;
	//卫星角速度
	static float satellite_wv;
	//卫星线加速度
	static float satellite_a; 
};

#endif