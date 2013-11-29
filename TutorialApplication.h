/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
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
#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "BaseApplication.h"
#include "Hydrax.h"

class TutorialApplication : public BaseApplication
{
public:
    TutorialApplication(void);
    virtual ~TutorialApplication(void);
	virtual bool frameStarted(const Ogre::FrameEvent& evt);

protected:
    virtual void createScene(void);
private:
	Ogre::AnimationState* mAnimState;
	Ogre::ParticleSystem* smoke_particle; //火焰粒子
	bool bind_smoke_to_rocket; //火箭是否是了粒子系统

	Ogre::Entity* ent_rocket_;  //火箭
	Ogre::Entity* ent_earth_;  //地球
	Ogre::Entity* ent_satelite_; //卫星
	Ogre::SceneNode* ent_node_earth_; //挂在地球的
	Ogre::SceneNode* ent_node_satelite_; //卫星节点
	Ogre::SceneNode* ent_node_rocket_;  //火箭节点

	//函数 切换场景
	void ChangeScene(int time);

	ExampleHydraxDemoListener* mHydraxListener;

};

#endif // #ifndef __TutorialApplication_h_
