//Hydraxϵͳ
#ifndef _HYDRAX_H
#define _HYDRAX_H

#include "Ogre.h"
#include "OgreStringConverter.h"
#include "OgreException.h"
#include "OIS.h"
#include "Hydrax.h"
#include "Noise/Perlin/Perlin.h"
#include "Modules/ProjectedGrid/ProjectedGrid.h"
#include "OgreTextAreaOverlayElement.h"
#include "hydrax_frame_listener.h"


class ExampleHydraxDemoListener : public ExampleFrameListenerHydrax
{
public:
	SceneManager *mSceneMgr;
	Real mKeyBuffer;
	friend class TutorialApplication;

	ExampleHydraxDemoListener(RenderWindow* win, Camera* cam, SceneManager *sm)
		: ExampleFrameListenerHydrax(win,cam)
		, mSceneMgr(sm)
		, mKeyBuffer(-1)
	{
	}

	bool frameStarted(const FrameEvent &e);
	void changeSkyBox();
};

/** Just to locate palmiers with a pseudo-random algoritm
*/


float rnd_(const float& min, const float& max);

void createPalms(Ogre::SceneManager *mSceneMgr);

void createTextArea();

#endif