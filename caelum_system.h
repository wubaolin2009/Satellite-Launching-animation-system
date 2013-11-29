//Caelum插件的使用
#ifndef CAELUM_SYSTEM_H_
#define CAELUM_SYSTEM_H_

#include "Caelum.h"
#include "ExampleFrameListener.h"

class TutorialApplication;

//Caleum系统
class CaelumSampleFrameListener:public ExampleFrameListener
{
protected:
	
	Ogre::SceneManager *mScene;
	float mSpeedFactor;
	bool mPaused;
	float mTimeTillNextUpdate;
	friend class TutorialApplication;

public:
	Caelum::CaelumSystem *mCaelumSystem;
	CaelumSampleFrameListener(RenderWindow* win, Camera* cam);
	~CaelumSampleFrameListener() {
		if (mCaelumSystem) {
			///mWindow->removeListener(mCaelumSystem);
			///mCaelumSystem->shutdown(true);
			////mCaelumSystem = 0;
		}
	}
	void remove()
	{
		if(mCaelumSystem){
			mWindow->removeListener(mCaelumSystem);
			mCaelumSystem->shutdown(false);
			//delete mCaelumSystem;
			mCaelumSystem = NULL;
		}
		//mCaelumSystem = NULL;
	}

	// Update speed factors.
	void UpdateSpeedFactor(double factor)
	{
		mSpeedFactor = factor;
		mCaelumSystem->getUniversalClock ()->setTimeScale (mPaused ? 0 : mSpeedFactor);
	}

	bool frameEnded(const FrameEvent& evt)
	{
		///if (!ExampleFrameListener::frameEnded(evt)) {
			///return false;
		////}

		// Stop key repeat for these keys.
		/*mTimeTillNextUpdate -= evt.timeSinceLastFrame;
		if (mTimeTillNextUpdate <= 0) {
			if (mKeyboard->isKeyDown (OIS::KC_SPACE)) {
				mTimeTillNextUpdate = 1;
				mPaused = !mPaused;
				UpdateSpeedFactor(mSpeedFactor);
			}
			if (mKeyboard->isKeyDown (OIS::KC_X)) {
				mTimeTillNextUpdate = 0.25;
				UpdateSpeedFactor(mSpeedFactor / 2);
			}
			if (mKeyboard->isKeyDown (OIS::KC_C)) {
				mTimeTillNextUpdate = 0.25;
				UpdateSpeedFactor(mSpeedFactor * 2);
			}
			if (mKeyboard->isKeyDown (OIS::KC_Z)) {
				mTimeTillNextUpdate = 1;
				UpdateSpeedFactor(mSpeedFactor * -1);
			}
		}*/

		return true;
	}
};

#endif