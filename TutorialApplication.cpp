/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#define OIS_DYNAMIC_LIB
#include <vector>
#include "TutorialApplication.h"
#include "satellite.h"
#include "MyGUI.h"
#include "caelum_system.h"
#include "hydrax.h"



//-------------------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
	mAnimState = NULL;
	bind_smoke_to_rocket = false;	
	ent_rocket_ = NULL;  //火箭
	ent_earth_ = NULL;  //地球
	ent_satelite_ = NULL; //卫星
	ent_node_earth_ = NULL; //挂在地球的
	ent_node_satelite_ = NULL; //卫星节点
	ent_node_rocket_ = NULL;  //火箭节点
}
//-------------------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

using namespace Ogre;
using namespace Caelum;

extern CaelumSystem* g_CaelumSystem;

extern double g_satelite_x;
extern double g_satelite_y;
extern double g_satelite_z;

extern Hydrax::Hydrax *mHydrax;

bool is_in_space = false; //是否在太空中

void TutorialApplication::ChangeScene(int time)
{
	//去掉地形
//	mSceneMgr->clearScene();

	//解除掉 caelum和Hydrax的引用
	if(time == 0){
		if(mFrameListener){
			//mRoot->removeFrameListener(mFrameListener->mCaelumSystem); //这行代码 让我浪费了近1天的时间。。。
			mRoot->removeFrameListener(mFrameListener);
			mFrameListener->remove();
			//mFrameListener->mCaelumSystem = NULL;
			//delete mFrameListener;
			//mFrameListener = NULL;
		}
		if(mHydraxListener)	{
			mRoot->removeFrameListener(mHydraxListener);
			mRoot->removeFrameListener(mHydraxListener);
			mHydrax->remove();
			//mHydraxListener->;
			delete mHydrax;
			mHydrax = NULL;
			
		}
		return ;
	}
	mSceneMgr->clearScene();

	
	
	//加入光照
	mSceneMgr->setAmbientLight( ColourValue(1.0,1.0,1.0) );
 
     // Create a light
    Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setType(Light::LightTypes::LT_DIRECTIONAL);
    l->setPosition(600,20,180);
	l->setDirection(-10,0,0);
	l->setDiffuseColour(1.0,1.0,1.0);
	l->setSpecularColour(1.0,1.0,1.0);

	//上边在家一个light 这主要是卫星看不清楚
	l = mSceneMgr->createLight("UpLight");
	l->setType(Light::LightTypes::LT_DIRECTIONAL);
    l->setPosition(-1500,0,0);
	l->setDirection(10,0,0);
	l->setDiffuseColour(1.0,1.0,1.0);
	l->setSpecularColour(1.0,1.0,1.0);

	//解除掉rocket节点
	mSceneMgr->getRootSceneNode()->detachAllObjects();

	//重新设置摄象机
	mCamera->setPosition(Ogre::Vector3(-1200,0,0));
	mCamera->lookAt(Ogre::Vector3(0,0,0));


	//挂载地球节点
	 ent_earth_ = mSceneMgr->createEntity("Earth", "earth.mesh");
 	ent_satelite_ = mSceneMgr->createEntity("Satellite", "ss.mesh");
	ent_node_earth_ = mSceneMgr->getRootSceneNode()->createChildSceneNode("earth_node");
	ent_node_earth_->attachObject(ent_earth_);
	ent_node_earth_->scale(Vector3(7.77,7.77,7.77) );  //我现在不知道地球这个模型究竟有多大 地球半径41，这个项目中应该是6371/20,放大6371/810=7.77

	//挂载卫星节点
	ent_node_satelite_ = mSceneMgr->getRootSceneNode()->createChildSceneNode("satellite_node");
	//ent_node_satelite->attachObject(ent_satelite);
	ent_node_satelite_->scale(Vector3(1.0,1.0,1.0) );
	ent_node_satelite_->attachObject(ent_satelite_);

	//创建动画
	//下面定义动画、动画轨迹以及关键帧：
	// 定义动画，指定动画的名称及长度（这里为10秒）
	Animation* anim = mSceneMgr->createAnimation("EarthRotate", 90);
	// 指定动画关键帧之间的插值方式（包括线性插值和样条插值）
	anim->setInterpolationMode(Animation::IM_SPLINE);
	// 定义动画的一个动画轨迹，并指定这个轨迹是作用到地球节点上的
	NodeAnimationTrack* track = anim->createNodeTrack(0, ent_node_earth_);

	TransformKeyFrame* key; // = track->createNodeKeyFrame(0); // startposition
	//创建180个关键帧，每个关键真 之间相隔2度
	for(int i = 0;i < 90;i++){
		key = track->createNodeKeyFrame(i);
		Quaternion rotate(Degree(4*i),Vector3::UNIT_Y);  //绕着y轴转
		rotate.FromAngleAxis(Radian(Degree(-i*4)),Vector3::UNIT_Y);
		key->setScale(Vector3(6.77,6.77,6.77) );
		key->setRotation(rotate);

	}

	//然后定义AnimationState类的对象，它和刚才定义的动画类相对应。设置动画的状态为启用：
	mAnimState = mSceneMgr->createAnimationState("EarthRotate");
	mAnimState->setEnabled(true);    // 启用该动画

	//绕着x轴转90度先
	Quaternion rotate_s;  //绕着x轴转
	rotate_s.FromAngleAxis(Radian(Degree(-90)),Vector3::UNIT_X);
	ent_node_satelite_->rotate(rotate_s);
	ent_node_satelite_->translate(Vector3(g_satelite_y,g_satelite_z,g_satelite_x ));

	//天空盒
	mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox",20000);

	//至卫星状态为NEAR_TRACK 这个是时间触发的 这里不需要改变
	//置全局变量 is in space为true
	is_in_space = true;
	//MessageBox(NULL,"FDSFS",NULL,NULL);

}

//-------------------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
	//初始化GUI
	MyGUI::getSingletonPtr()->init();
	//SkeletonPtr skel = SkeletonManager::getSingleton().load("jaiqua.skeleton",ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  
	SetParameter();

	//创建火箭
	ent_rocket_ = mSceneMgr->createEntity("Rocket", "rocket.mesh");
	ent_node_rocket_ = mSceneMgr->getRootSceneNode()->createChildSceneNode("rocket_node");
	ent_node_rocket_->scale(1.5,1.5,1.5);
	ent_node_rocket_->translate(Vector3(1197,208,1420 ));
    ent_node_rocket_->attachObject(ent_rocket_);
	

	
	//动画初始化结束
	//mSceneMgr->setAmbientLight( ColourValue(1.0,1.0,1.0) );
 
     // Create a light
   /*Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setType(Light::LightTypes::LT_DIRECTIONAL);
    l->setPosition(600,20,180);
	l->setDirection(-10,0,0);
	l->setDiffuseColour(1.0,1.0,1.0);
	l->setSpecularColour(1.0,1.0,1.0);

	//上边在家一个light 这主要是卫星看不清楚
	l = mSceneMgr->createLight("UpLight");
	l->setType(Light::LightTypes::LT_DIRECTIONAL);
    l->setPosition(-1500,0,0);
	l->setDirection(10,0,0);
	l->setDiffuseColour(1.0,1.0,1.0);
	l->setSpecularColour(1.0,1.0,1.0);*/

	
	//天空盒
	//mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox",20000);

	//初始化太阳？
	//mCaelumSystem = new CaelumSystem (mRoot, mSceneMgr, CaelumSystem::CAELUM_COMPONENTS_DEFAULT);
	//下边的代码是大气层中的影响，可能效果会很差，先渲染试试
	//mSceneMgr->setSkyBox(true,"Examples/CloudyNoonSkyBox",20000);
	//渲染地面
	//mSceneMgr->setWorldGeometry("terrain.cfg" );
	//这是试着产生粒子效果
	//"Examples/GreenyNimbus"
	smoke_particle = mSceneMgr->createParticleSystem("Nimbus","Examples/Smoke" );


	//这里初始化Hydrax系统
	
	
	// Set default ambient light
	mSceneMgr->setAmbientLight(ColourValue(1, 1, 1));
	// Create Hydrax object
	mHydrax = new Hydrax::Hydrax(mSceneMgr, mCamera, mWindow->getViewport(0));

	// Create our projected grid module  
	Hydrax::Module::ProjectedGrid *mModule 
		= new Hydrax::Module::ProjectedGrid(// Hydrax parent pointer
			mHydrax,
			// Noise module
			new Hydrax::Noise::Perlin(),
			// Base plane
			Ogre::Plane(Ogre::Vector3(0,1,0), Ogre::Vector3(0,0,0)),
			// Normal mode
			Hydrax::MaterialManager::NM_VERTEX,
			// Projected grid options
			Hydrax::Module::ProjectedGrid::Options()
		);

	// Set our module
	mHydrax->setModule(static_cast<Hydrax::Module::Module*>(mModule));

	// Load all parameters from config file
	// Remarks: The config file must be in Hydrax resource group.
	// All parameters can be set/updated directly by code(Like previous versions),
	// but due to the high number of customizable parameters, since 0.4 version, Hydrax allows save/load config files.
	mHydrax->loadCfg("HydraxDemo.hdx");

	// Create water
	mHydrax->create();

	// Hydrax initialization code end -----------------------------------------
	// ------------------------------------------------------------------------

	// Load island
	mSceneMgr->setWorldGeometry("Island.cfg");

	mHydrax->getMaterialManager()->addDepthTechnique(
		static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("Island"))
		->createTechnique());

	// Create palmiers
	createPalms(mSceneMgr);

	// Create text area to show skyboxes information
	createTextArea();

	// Add frame listener
	mHydraxListener = new ExampleHydraxDemoListener(mWindow, mCamera, mSceneMgr);
	mRoot->addFrameListener(mHydraxListener);

}

//这里保存的是卫星的运行轨迹
Ogre::ManualObject *pManual; //已经画了的点
//SubMesh* sm;  //用来保存的submesh
std::vector<Vector3> g_points;

#define MAX_FPS 30.0 //每秒最多30帧
bool TutorialApplication::frameStarted(const FrameEvent& evt)
{
	//mSceneMgr->setFog(FOG_LINEAR);
	static double accumulated_time = 0.0; //累计的这帧的时间
	if(mAnimState){
		mAnimState->addTime(evt.timeSinceLastFrame);
	}
	//更新GUI
	MyGUI::getSingletonPtr()->update(evt.timeSinceLastFrame);
	//mAnimState2->addTime(evt.timeSinceLastFrame);
	//这里更新卫星的状态
	static int frame = 0;
	static int rotate_angle = 0;
	double temp_x = g_satelite_x;
	double temp_y = g_satelite_y;
	double temp_z = g_satelite_z;
	//这里进行fps的平滑
	int time_of_frame = 0; //渲染0帧数
	accumulated_time += evt.timeSinceLastFrame;
	if(accumulated_time < 1.0/MAX_FPS){
		time_of_frame = 0;
	}
	else{
		time_of_frame = accumulated_time / (1.0/MAX_FPS);
		accumulated_time = 0;
	}
	//time_of_frame = 1;
	if(SatelliteStatus::current_status == SatelliteStatus::UNLAUNCHED){
		//OnEnterFrame();
		time_of_frame = 0;
	}
	for(int i = 0;i < time_of_frame;i++){
		OnEnterFrame(); //通知卫星画一帧
		//转卫星
		rotate_angle += 12;
		if(rotate_angle > 348)
			rotate_angle = 0;
		SatelliteStatus::satellite_angle = rotate_angle;
	}
	if(is_in_space){
		Quaternion rotate;  //绕着y轴转
		rotate.FromAngleAxis(Radian(Degree(12)),Vector3::UNIT_Z);
		ent_node_satelite_->rotate(rotate);

		ent_node_satelite_->translate(Vector3(g_satelite_y-temp_y,g_satelite_z-temp_z,g_satelite_x-temp_x));
	}
	//对摄像机进行动画
	//mCamera->setPosition(g_satelite_y,g_satelite_z + 100,g_satelite_x) ;

	//mCamera->setDirection(0,-100 ,0);

	//如果点的位置有所改变，则多画一个点
	static SceneNode* ent_node_track;  //轨迹加载的node
	//if(SatelliteStatus::show_track == true){
	static bool last_show = false;
	if(last_show == false && SatelliteStatus::show_track == true){
		if(pManual){
			ent_node_track->attachObject(pManual);
		}
	}
	if(last_show == true && SatelliteStatus::show_track == false){
		ent_node_track->detachObject("Track");
	}
	static int cheat = 0;
	last_show = SatelliteStatus::show_track;
	if((temp_x != g_satelite_x || temp_y != g_satelite_y || temp_z != g_satelite_z) ){
		Vector3 new_point(g_satelite_y,g_satelite_z,g_satelite_x);
		if(SatelliteStatus::current_status != SatelliteStatus::UNLAUNCHED && SatelliteStatus::current_status!= SatelliteStatus::RISING && SatelliteStatus::current_status != SatelliteStatus::OUT_CONTROL){
			cheat++;
			if(cheat > 30){
				if(cheat % 10 == 0)
					g_points.push_back(new_point);
			}
		}
		if((SatelliteStatus::show_track)&&(SatelliteStatus::current_status > SatelliteStatus::RISING)&&(SatelliteStatus::current_status != SatelliteStatus::OUT_CONTROL) ){
			//重画轨迹曲线
			//这里尝试用Manual Object 来画
			if(pManual){  //再加一个点 用来画
				//首先要detach
				//ent_node_track->detachObject("Track");
				pManual->begin("",Ogre::RenderOperation::OT_LINE_STRIP);
				for(int i = 0;i<g_points.size();i++){
					pManual->position(g_points[i].x,g_points[i].y,g_points[i].z);
					pManual->colour(ColourValue::Red);
				}
				pManual->end();
				//ent_node_track->attachObject(pManual);
				//pManual->position(g_satelite_y,g_satelite_z,g_satelite_x);
			}			
			else{
				pManual = new ManualObject("Track");
				pManual->begin("",Ogre::RenderOperation::OT_LINE_STRIP);
				pManual->position(g_satelite_y,g_satelite_z,g_satelite_x);
				pManual->colour(ColourValue::Red);
				pManual->end();
				ent_node_track = mSceneMgr->getRootSceneNode()->createChildSceneNode();
				ent_node_track->attachObject(pManual);
			}
		}
	}
	/* 如果卫星进入RISING状态 */
	if(SatelliteStatus::current_status == SatelliteStatus::RISING){
		if(!bind_smoke_to_rocket){  //没有应用到火箭上
			//将粒子效果绑定到rocket上
			ent_node_rocket_->attachObject(smoke_particle);
			bind_smoke_to_rocket = true;
		}
		//不断让火箭上升
		ent_node_rocket_->translate(Vector3(0,12,0) );
		//设置camera
		Vector3 position_now = mCamera->getPosition();
		position_now.y += 12;
		mCamera->setPosition(position_now);		
		//得到太阳状态并更新Hydrax
		Ogre::Vector3 sunPosition = mCamera->getDerivedPosition();
		sunPosition -= g_CaelumSystem->getSun()->getLightDirection() * 80000;
		Ogre::ColourValue sunColor = g_CaelumSystem->getSun()->getBodyColour();
		mHydrax->setSunPosition(sunPosition);
		Vector3 color;
		color.x=sunColor.r;
		color.y=sunColor.g;
		color.z=sunColor.b;
		mHydrax->setSunColor(color );
	}
	//如果卫星处于NEAR状态 则变换视角
	static int ccc = 0; //20镇后开始清除场景
	if(SatelliteStatus::current_status == SatelliteStatus::NEAR_TRACK){
		if(is_in_space == false){
			if(ccc == 0){
				ChangeScene(0);
			}
			ccc ++;
			if( ccc >= 60){
				is_in_space = true;
				ChangeScene(1);
			}
		}
	}
	//mSceneMgr->setFog(FOG_NONE);
	return true;
}



#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif


/*这段代码是创建SubMesh的，以后可能用得到
			// 创建顶格式 3个坐标 一个颜色
			VertexDeclaration* decl = HardwareBufferManager::getSingleton().createVertexDeclaration();
			decl->addElement(0, 0, VET_FLOAT3, VES_POSITION);
			decl->addElement(0, sizeof(float) * 3, VET_COLOUR_ARGB,VES_DIFFUSE);

			MeshPtr msh = MeshManager::getSingleton().createManual("Track",ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			sm = msh->createSubMesh();
			sm->useSharedVertices = false;
			sm->vertexData = new VertexData();
			sm->vertexData->vertexStart = 0;
			sm->vertexData->vertexCount = 3;
			//设置顶点格式
			VertexDeclaration* dcl = sm->vertexData->vertexDeclaration;
			size_t offset = 0;
			dcl->addElement(0, offset, VET_FLOAT3, VES_POSITION);
			offset += VertexElement::getTypeSize(VET_FLOAT3);
			dcl->addElement(0, offset, VET_FLOAT3, VES_NORMAL);
			offset += VertexElement::getTypeSize(VET_FLOAT3);
			dcl->addElement(0, offset, VET_COLOUR_ARGB, VES_DIFFUSE);
			offset += VertexElement::getTypeSize(VET_COLOUR_ARGB);
	
			HardwareVertexBufferSharedPtr vbuf = HardwareBufferManager::getSingleton().createVertexBuffer(
				offset, 3, HardwareBuffer::HBU_STATIC_WRITE_ONLY);
			float* pReal = static_cast<float*>(vbuf->lock(HardwareBuffer::HBL_DISCARD)); //定点缓冲区
			for(int i = 0;i<3;i++){ //一个三角形
				//位置x y z
				*pReal++ = 0;
				*pReal++ = i==1?1000:0;
				*pReal++ = 1000 - i*1000;
				//法向量
				*pReal++ = -1;
				*pReal++ = 0;
				*pReal++ = 0;
				//颜色
				ColourValue* v = (ColourValue*)pReal;
				pReal +=  VertexElement::getTypeSize(VET_COLOUR_ARGB);
				v->a = 0;
				v->r = v->g = v->b = 255;				
			}
		
			vbuf->unlock();
			sm->vertexData->vertexBufferBinding->setBinding(0, vbuf);
			//剩下的是Index相关的代码
			sm->indexData->indexCount = 3;
			sm->indexData->indexBuffer = HardwareBufferManager::getSingleton()
				.createIndexBuffer(HardwareIndexBuffer::IT_16BIT,3,
				HardwareBuffer::HBU_STATIC_WRITE_ONLY);
			uint16* pI = static_cast<uint16*>(sm->indexData->indexBuffer->lock(HardwareBuffer::HBL_DISCARD));
			pI[0] = 0;
			pI[1] = 1;
			pI[2] = 2;						
			sm->indexData->indexBuffer->unlock();
			//加载Mesh
			//msh->load();

			SceneNode* pp = mSceneMgr->getRootSceneNode()->createChildSceneNode();
			 Ogre::Entity* ent_earth = mSceneMgr->createEntity("Tran", "Track");
			pp->attachObject(ent_earth);*/


