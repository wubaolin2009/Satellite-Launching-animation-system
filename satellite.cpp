#include <vector>
#include <math.h>
#include "satellite.h"
//全局变量的初始化

SatelliteStatus::ViewPort SatelliteStatus::current_view_port = SatelliteStatus::FREE_VIEW;  //当前视角，默认为FREE_VIEW
float SatelliteStatus::near_radius;  //近地轨道半径
float SatelliteStatus::far_radius;   //远地轨道半径
SatelliteStatus::Status SatelliteStatus::current_status = SatelliteStatus::UNLAUNCHED; //默认为UNLAUNCHED
bool SatelliteStatus::show_track;  //是否显示卫星的轨道，默认为false
//卫星的坐标
float SatelliteStatus::satellite_x;
float SatelliteStatus::satellite_y;
float SatelliteStatus::satellite_z;
//卫星的自转角度，Y轴 Z X左手定则方向
float SatelliteStatus::satellite_angle;
//卫星线速度
float SatelliteStatus::satellite_lv;
//卫星角速度
float SatelliteStatus::satellite_wv;
//卫星线加速度
float SatelliteStatus::satellite_a; 

//这里是时间 程序中的每一秒代表生活中的多少小时
const double time_factor = 1.0; //这里暂时定为一个小时
//这里是距离 程序中的一点 代表生活中的多少km
const double distance_factor = 20.0;

using std::vector;
struct _Point3D
{
	double x;
	double y;
	double z;
};
//下邊代碼翻译自ComputerGraphic.as中的ComputeGraphic类
//计算中比例为20千米
const int K = 100;
//earth:GeoSphere
//statllite::Box
int time = 0;
const double earth_radius = 6371/20;
double G = 6.67;
double earth_mass = 5.98*pow(10.0,7.0);

double launch_initial_acceleration;
double launch_complete_acceleration;
_Point3D launch_initial_point;
_Point3D launch_complete_point;
int launch_time = 48;

double first_circle_radius;
double first_circle_velocity;
double first_circle_time;

double oval_long_axle;
double apogee;
double apogee_velocity;
double oval_short_axle;
int end_velocity_equal_to_time;
double perigee;
double perigee_velocity;
int oval_orbit_splice;
double oval_orbit_time;

double velocity;
bool complement_var;
double count;

double second_circle_radius;
double second_circle_velocity;
double second_circle_time;
#define PI (3.141593)

//这里是卫星的位置参数
double g_satelite_x = 0;
double g_satelite_y = 0;
double g_satelite_z = 0;

//设置卫星的参数
void SetParameter(double near,double far)
{
	launch_initial_acceleration = 50;
	launch_complete_acceleration = -500;
	launch_initial_point.x = earth_radius/sqrt(2.0);
	launch_initial_point.y = earth_radius/sqrt(2.0);
	launch_initial_point.z = 0;

	g_satelite_x = near;
	g_satelite_y = 0;
	g_satelite_z = launch_initial_point.z;

	launch_complete_point.x = near;
	launch_complete_point.y = launch_complete_point.z = 0;
	launch_time = 800;  //最开始是48

	first_circle_radius = near;
	//fristCricleVelocity= Math.sqrt(9.8*(earthRadius+300/20))/20;//感觉会有问题
	first_circle_velocity= sqrt((double)(G*earth_mass/first_circle_radius))/100;
	///////////trace(fristCricleVelocity);
	first_circle_time = int(2*PI*first_circle_radius/first_circle_velocity);
	//fristCricleTime= 72; 

	apogee = far;
	perigee = first_circle_radius;
	end_velocity_equal_to_time = K;	
	oval_long_axle = (perigee+apogee)/2;
	oval_short_axle = sqrt((double)(oval_long_axle*oval_long_axle-((oval_long_axle*2-perigee)/2)*((oval_long_axle*2-perigee)/2) ));		 
	perigee_velocity = sqrt((double)(G*earth_mass*apogee/(oval_long_axle*perigee) ))/100;
	apogee_velocity = sqrt((double)(G*earth_mass*perigee/(oval_long_axle*apogee)))/100;
	oval_orbit_splice= (PI*oval_short_axle + 2*(oval_long_axle - oval_short_axle))/((apogee_velocity+perigee_velocity)/2);
	oval_orbit_time = 2*(oval_orbit_splice*(end_velocity_equal_to_time-0.5)+oval_orbit_splice*oval_orbit_splice/2);
	//////////trace(perigeeVelocity,apogeeVelocity);


	velocity =oval_orbit_splice-1;
	complement_var = false;
	count = 1;

	second_circle_radius = apogee;
	second_circle_velocity = sqrt((double)(G*earth_mass/apogee ))/100;
	///////////trace(secondCricleVelocity);
	second_circle_time = int(2*PI*second_circle_radius/second_circle_velocity);
}

void SecondCircleOrbit(double time)
{
	SatelliteStatus::current_status = SatelliteStatus::FAR_TRACK;
	if(time > second_circle_time){
		time = (int)time % (int)second_circle_time;

	}
	_Point3D fistPoint;
	fistPoint.x = second_circle_radius*cos((double)((-time/second_circle_time*2-0.5)*PI));
	fistPoint.y = second_circle_radius*sin((double)((-time/second_circle_time*2-0.5)*PI));
	fistPoint.z = 0;

	_Point3D lastPoint;
	lastPoint.x = second_circle_radius*cos((double)((-(time+1)/second_circle_time*2-0.5)*PI));
	lastPoint.y = second_circle_radius*sin((double)((-(time+1)/second_circle_time*2-0.5)*PI));
	lastPoint.z = 0;


	///////////重设卫星的坐标 很重要!!!!!
	SatelliteStatus::satellite_x = g_satelite_x = lastPoint.x;
	SatelliteStatus::satellite_y = g_satelite_y = lastPoint.y;
	SatelliteStatus::satellite_z = g_satelite_z = lastPoint.z;	 
	/////然后再卫星2个点之间画线
	/////这里算线速度
	double last_v = SatelliteStatus::satellite_lv;
	SatelliteStatus::satellite_lv = sqrt((double)( (lastPoint.x-fistPoint.x)*(lastPoint.x-fistPoint.x) + (lastPoint.y-fistPoint.y)*(lastPoint.y-fistPoint.y) ) );
	SatelliteStatus::satellite_lv *= distance_factor;
	SatelliteStatus::satellite_lv /= time_factor;
	////这里算角速度,角速度算法是 
	if(lastPoint.x == 0 || fistPoint.x == 0){
		SatelliteStatus::satellite_wv = 0;
	}
	else{
		SatelliteStatus::satellite_wv = atan((double)((double)lastPoint.y/(double)lastPoint.x)) - atan((double)((double)fistPoint.y/(double)fistPoint.x));
		SatelliteStatus::satellite_wv *= 180.0;
		SatelliteStatus::satellite_wv /= 3.1415926;
		SatelliteStatus::satellite_wv /= time_factor;
	}
	///这里算线加速度
	if(last_v){
		SatelliteStatus::satellite_a = SatelliteStatus::satellite_lv - last_v;
		SatelliteStatus::satellite_a /= time_factor;
	}
	else{
		SatelliteStatus::satellite_a = 0;
	}
	/////速度部分结束
}
double ChangeOvalOrbit(double time,double velocity)
{
	SatelliteStatus::current_status = SatelliteStatus::ECLLIPSE;
	_Point3D fistPoint;
	fistPoint.x = oval_short_axle*cos((-(time)/oval_orbit_time*2+0.5)*PI);
	fistPoint.y = oval_long_axle*sin((-time/oval_orbit_time*2+0.5)*PI)-(oval_long_axle-first_circle_radius);
	fistPoint.z = 0;
	_Point3D lastPoint;
	lastPoint.x = oval_short_axle*cos((double)((-(time+velocity)/oval_orbit_time*2+0.5)*PI ));
	lastPoint.y = oval_long_axle*sin((double)((-(time+velocity)/oval_orbit_time*2+0.5)*PI))-(oval_long_axle-first_circle_radius);
	lastPoint.z = 0;
	///////////重设卫星的坐标 很重要!!!!!
	SatelliteStatus::satellite_x = g_satelite_x = lastPoint.x;
	SatelliteStatus::satellite_y = g_satelite_y = lastPoint.y;
	SatelliteStatus::satellite_z = g_satelite_z = lastPoint.z;
	
	/////然后再卫星2个点之间画线
	/////这里算线速度
	double last_v = SatelliteStatus::satellite_lv;
	SatelliteStatus::satellite_lv = sqrt((double)( (lastPoint.x-fistPoint.x)*(lastPoint.x-fistPoint.x) + (lastPoint.y-fistPoint.y)*(lastPoint.y-fistPoint.y) ) );
	SatelliteStatus::satellite_lv *= distance_factor;
	SatelliteStatus::satellite_lv /= time_factor;
	////这里算角速度,角速度算法是 
	if(lastPoint.x == 0 || fistPoint.x == 0){
		SatelliteStatus::satellite_wv = 0;
	}
	else{
		SatelliteStatus::satellite_wv = atan((double)((double)lastPoint.y/(double)lastPoint.x)) - atan((double)((double)fistPoint.y/(double)fistPoint.x));
		SatelliteStatus::satellite_wv *= 180.0;
		SatelliteStatus::satellite_wv /= 3.1415926;
		SatelliteStatus::satellite_wv /= time_factor;
	}
	///这里算线加速度
	if(last_v){
		SatelliteStatus::satellite_a = SatelliteStatus::satellite_lv - last_v;
		SatelliteStatus::satellite_a /= time_factor;
	}
	else{
		SatelliteStatus::satellite_a = 0;
	}
	/////速度部分结束

	return time + velocity;
}
void FirstCircleOrbit(double time)
{
	SatelliteStatus::current_status = SatelliteStatus::NEAR_TRACK;
	_Point3D fistPoint;
	fistPoint.x = first_circle_radius*cos((double)(-time/first_circle_time*2*PI));
	fistPoint.y = first_circle_radius*sin((double)(-time/first_circle_time*2*PI));
	fistPoint.z = 0;
	
	_Point3D lastPoint;
	lastPoint.x = first_circle_radius*cos((double)(-(time+1)/first_circle_time*2*PI));
	lastPoint.y = first_circle_radius*sin((double)(-(time+1)/first_circle_time*2*PI));
	lastPoint.z = 0;
	
	///////////重设卫星的坐标 很重要!!!!!
	SatelliteStatus::satellite_x = g_satelite_x = lastPoint.x;
	SatelliteStatus::satellite_y = g_satelite_y = lastPoint.y;
	SatelliteStatus::satellite_z = g_satelite_z = lastPoint.z;	
	/////然后再卫星2个点之间画线
	/////这里算线速度
	double last_v = SatelliteStatus::satellite_lv;
	SatelliteStatus::satellite_lv = sqrt((double)( (lastPoint.x-fistPoint.x)*(lastPoint.x-fistPoint.x) + (lastPoint.y-fistPoint.y)*(lastPoint.y-fistPoint.y) ) );
	SatelliteStatus::satellite_lv *= distance_factor;
	SatelliteStatus::satellite_lv /= time_factor;
	////这里算角速度,角速度算法是 
	if(lastPoint.x == 0 || fistPoint.x == 0){
		SatelliteStatus::satellite_wv = 0;
	}
	else{
		SatelliteStatus::satellite_wv = atan((double)((double)lastPoint.y/(double)lastPoint.x)) - atan((double)((double)fistPoint.y/(double)fistPoint.x));
		SatelliteStatus::satellite_wv *= 180.0;
		SatelliteStatus::satellite_wv /= 3.1415926;
		SatelliteStatus::satellite_wv /= time_factor;
	}
	///这里算线加速度
	if(last_v){
		SatelliteStatus::satellite_a = SatelliteStatus::satellite_lv - last_v;
		SatelliteStatus::satellite_a /= time_factor;
	}
	else{
		SatelliteStatus::satellite_a = 0;
	}
	/////速度部分结束
}
void LaunchOrbit(double time)
{
	vector<_Point3D> launchControlPoints;
	 
	launchControlPoints.push_back(launch_initial_point);
	launchControlPoints.push_back(launch_complete_point);
	_Point3D temp;
	temp.x = temp.z = 0;
	temp.y = launch_initial_acceleration;
	launchControlPoints.push_back(temp);
	temp.x = temp.z = 0;
	temp.y = launch_complete_acceleration;
	launchControlPoints.push_back(temp);
	//画一条hermit曲线
	//var hermiteArc:HermiteArc = new HermiteArc(launchControlPoints,time,launchTime,satellite);

}		

//每画一帧要调用的函数

void OnEnterFrame()
{
	time++;	
	if(time <= launch_time)
		LaunchOrbit(time); 
	else if(time < 1.75*first_circle_time + launch_time){
		FirstCircleOrbit(time - launch_time);
		/*if(time >= 1.75*first_circle_time + launch_time){
			time -= 1.75*first_circle_time;
		}*/
	}
	else if(time <= 0.5*oval_orbit_time + 1.75*first_circle_time + launch_time)
	{
		if(velocity+1)
		{
			time = ChangeOvalOrbit(time - 1.75*first_circle_time - launch_time,velocity+end_velocity_equal_to_time)+ 1.75*first_circle_time + launch_time - 1;
			velocity --;
		}
	}
	else if(time < 1*oval_orbit_time + 1.75*first_circle_time + launch_time)
	{
		velocity ++;
		time = ChangeOvalOrbit(time  - 1.75*first_circle_time - launch_time,velocity+end_velocity_equal_to_time)+ 1.75*first_circle_time + launch_time - 1;
	}
	else if(time <= 1.5*oval_orbit_time + 1.75*first_circle_time + launch_time)
	{

		if(velocity+1)
		{
			time = ChangeOvalOrbit(time -1.75*first_circle_time - launch_time,velocity+end_velocity_equal_to_time)+ 1.75*first_circle_time + launch_time - 1;
			velocity --;
		}
	}
	else if(time <= second_circle_time + 1.5*oval_orbit_time + 1.75*first_circle_time + launch_time)
	{
		second_circle_radius = 2*oval_long_axle-first_circle_radius;
		SecondCircleOrbit(time - 1.5*oval_orbit_time - 1.75*first_circle_time - launch_time);
	}
	else //无限制的转下去
	{
		//修正time
		//int new_time = time%(int)( (second_circle_time + 1.5*oval_orbit_time + 1.75*first_circle_time + launch_time) );
		SatelliteStatus::current_status = SatelliteStatus::OUT_CONTROL;
		second_circle_radius = 2*oval_long_axle-first_circle_radius;
		SecondCircleOrbit(time - 1.5*oval_orbit_time - 1.75*first_circle_time - launch_time);
	}
}

 
class ComputerGraphic
{
public:
	

};
