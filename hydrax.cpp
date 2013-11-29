#include "hydrax.h"


//这些是Hydrax的一些变量声明，源自Hydrax的Demo
#define _def_SkyBoxNum 3

// Hydrax pointer
Hydrax::Hydrax *mHydrax = 0;

Ogre::String mSkyBoxes[_def_SkyBoxNum] = 
{"Sky/ClubTropicana",
"Sky/EarlyMorning",
"Sky/Clouds"};

Ogre::Vector3 mSunPosition[_def_SkyBoxNum] = 
{Ogre::Vector3(0,10000,0),
Ogre::Vector3(0,10000,90000),
Ogre::Vector3(0,10000,0)};

Ogre::Vector3 mSunColor[_def_SkyBoxNum] = 
{Ogre::Vector3(1, 0.9, 0.6),
Ogre::Vector3(1,0.6,0.4),
Ogre::Vector3(0.45,0.45,0.45)};

int mCurrentSkyBox = 0;

class TutorialApplication;

// Just to wshow skyboxes information
Ogre::TextAreaOverlayElement* mTextArea = 0;

bool ExampleHydraxDemoListener::frameStarted(const FrameEvent &e)
{
	// Check camera height
	/*Ogre::RaySceneQuery *raySceneQuery = 
		mSceneMgr->
		createRayQuery(Ogre::Ray(mCamera->getPosition() + Ogre::Vector3(0,1000000,0), 
		Vector3::NEGATIVE_UNIT_Y));
	Ogre::RaySceneQueryResult& qryResult = raySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator i = qryResult.begin();
	if (i != qryResult.end() && i->worldFragment)
	{
		if (mCamera->getPosition().y < i->worldFragment->singleIntersection.y + 30)
		{
			mCamera->
				setPosition(mCamera->getPosition().x, 
				i->worldFragment->singleIntersection.y + 30, 
				mCamera->getPosition().z);
		}
	}

	delete raySceneQuery;*/

	// Update Hydrax
	if(mHydrax){
		mHydrax->update(e.timeSinceLastFrame);
	}

	// Check for skyboxes switch
	/*mKeyboard->capture();

	if (mKeyboard->isKeyDown(OIS::KC_M) && mKeyBuffer < 0)
	{
		mCurrentSkyBox++;

		if(mCurrentSkyBox > (_def_SkyBoxNum-1))
		{
			mCurrentSkyBox = 0;
		}

		changeSkyBox();

		mKeyBuffer = 0.5f;
	}

	mKeyBuffer -= e.timeSinceLastFrame;*/

	return true;
}

void ExampleHydraxDemoListener::changeSkyBox()
{
	// Change skybox
	mSceneMgr->setSkyBox(true, mSkyBoxes[mCurrentSkyBox], 99999*3, true);

	// Update Hydrax sun position and colour
	mHydrax->setSunPosition(mSunPosition[mCurrentSkyBox]);
	mHydrax->setSunColor(mSunColor[mCurrentSkyBox]);

	// Update light 0 light position and colour
	mSceneMgr->getLight("Light0")->setPosition(mSunPosition[mCurrentSkyBox]);
	mSceneMgr->getLight("Light0")->setSpecularColour(mSunColor[mCurrentSkyBox].x,mSunColor[mCurrentSkyBox].y,mSunColor[mCurrentSkyBox].z);

	// Update text area
	mTextArea->setCaption("Hydrax 0.5.1 demo application\nCurrent water preset: "  + Ogre::StringUtil::split(mSkyBoxes[mCurrentSkyBox],"/")[1] + " (" +Ogre::StringConverter::toString(mCurrentSkyBox+1) + "/3). Press 'm' to switch water presets.");

	// Log
	LogManager::getSingleton().logMessage("Skybox " + mSkyBoxes[mCurrentSkyBox] + " selected. ("+Ogre::StringConverter::toString(mCurrentSkyBox+1)+"/"+Ogre::StringConverter::toString(_def_SkyBoxNum)+")");
}



float seed_ = 801;
float rnd_(const float& min, const float& max)
{
	seed_ += Ogre::Math::PI*2.8574f + seed_*(0.3424f - 0.12434f + 0.452345f);
	if (seed_ > 10000000000) seed_ -= 10000000000;
	return ((max-min)*Ogre::Math::Abs(Ogre::Math::Sin(Ogre::Radian(seed_))) + min);
}

void createPalms(Ogre::SceneManager *mSceneMgr)
{
	const int NumberOfPalms = 12;

	Ogre::SceneNode* mPalmsSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	for (int k = 0; k < NumberOfPalms; k++)
	{
		Ogre::Vector3 RandomPos = Ogre::Vector3(rnd_(500,2500),
			0,
			rnd_(500,2500));

		Ogre::RaySceneQuery * raySceneQuery = mSceneMgr->
			createRayQuery(Ogre::Ray(RandomPos + Ogre::Vector3(0,1000000,0), 
			Ogre::Vector3::NEGATIVE_UNIT_Y));

		Ogre::RaySceneQueryResult& qryResult = raySceneQuery->execute();
		Ogre::RaySceneQueryResult::iterator i = qryResult.begin();

		if (i != qryResult.end() && i->worldFragment)
		{
			if (i->worldFragment->singleIntersection.y>105 || i->worldFragment->singleIntersection.y<20)
			{
				k--;
				continue;
			}

			RandomPos.y = i->worldFragment->singleIntersection.y;
		}
		else
		{
			k--;
			continue;
		}

		Ogre::Entity *mPalmEnt = mSceneMgr->createEntity("Palm"+Ogre::StringConverter::toString(k), "Palm.mesh");
		Ogre::SceneNode *mPalmSN = mPalmsSceneNode->createChildSceneNode();

		mPalmSN->rotate(Ogre::Vector3(-1,0,rnd_(-0.3,0.3)), Ogre::Degree(90));
		mPalmSN->attachObject(mPalmEnt);
		Ogre::Real Scale = rnd_(50,75);
		mPalmSN->scale(Scale,Scale,Scale);
		mPalmSN->setPosition(RandomPos);
	}
}



void createTextArea()
{
	// Create a panel
	/*Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>(
		OverlayManager::getSingleton().createOverlayElement("Panel", "HydraxDemoInformationPanel"));
	panel->setMetricsMode(Ogre::GMM_PIXELS);
	panel->setPosition(10, 10);
	panel->setDimensions(400, 400);*/

	// Create a text area
	/*mTextArea = static_cast<Ogre::TextAreaOverlayElement*>(
		OverlayManager::getSingleton().createOverlayElement("TextArea", "HydraxDemoInformationTextArea"));
	mTextArea->setMetricsMode(Ogre::GMM_PIXELS);
	mTextArea->setPosition(0, 0);
	mTextArea->setDimensions(100, 100);
	mTextArea->setCharHeight(16);
	mTextArea->setCaption("Hydrax 0.5.1 demo application\nCurrent water preset: "  + Ogre::StringUtil::split(mSkyBoxes[mCurrentSkyBox],"/")[1] + " (" +Ogre::StringConverter::toString(mCurrentSkyBox+1) + "/3). Press 'm' to switch water presets.");
	mTextArea->setFontName("BlueHighway-8");
	mTextArea->setColourBottom(ColourValue(0.3, 0.5, 0.3));
	mTextArea->setColourTop(ColourValue(0.5, 0.7, 0.5));

	// Create an overlay, and add the panel
	Ogre::Overlay* overlay = OverlayManager::getSingleton().create("OverlayName");
	overlay->add2D(panel);

	// Add the text area to the panel
	panel->addChild(mTextArea);

	// Show the overlay
	overlay->show();*/
}