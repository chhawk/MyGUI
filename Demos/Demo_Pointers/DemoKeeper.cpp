/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "ResourcePointerContext.h"

namespace demo
{

	Ogre::RaySceneQuery* gRaySceneQuery = 0;

	DemoKeeper::DemoKeeper() :
		mEnemyPanel(nullptr),
		mFriendPanel(nullptr),
		mControlPanel(nullptr),
		mPointerContextManager(nullptr),
		mRightButtonPressed(false),
		mAngleH(90),
		mAngleV(-25)
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Demos/Demo_Pointers");
		addResourceLocation(getRootMedia() + "/Common/Scene");
		addResourceLocation(getRootMedia() + "/Common/Wallpapers");
	}

	void DemoKeeper::createScene()
	{
		createEntities();

		//getGUI()->load("Wallpaper0.layout");
		MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().load("BackHelp.layout");
		root.at(0)->findWidget("Text")->setCaption("");

		MyGUI::FactoryManager::getInstance().registryFactory<ResourcePointerContext>("Resource");

		getGUI()->load("Contexts.xml");

#ifdef MYGUI_SAMPLES_INPUT_WIN32
		getGUI()->load("Pointers_W32.xml");
#else
		getGUI()->load("Pointers.xml");
#endif

		mPointerContextManager = new PointerContextManager(this);
		mPointerContextManager->addContext("ptrx_Normal");
		mPointerContextManager->setPointer("default");

		mEnemyPanel = new EnemyPanel();
		mFriendPanel = new FriendPanel();
		mControlPanel = new ControlPanel(mPointerContextManager);

		MyGUI::IntSize size = getGUI()->getViewSize();
		setMousePosition(size.width / 2, size.height / 2);
		updateCursorPosition();


		updateCamera(0, 0);
	}

	void DemoKeeper::destroyScene()
	{
		delete mEnemyPanel;
		mEnemyPanel = nullptr;
		delete mFriendPanel;
		mFriendPanel = nullptr;
		delete mControlPanel;
		mControlPanel = nullptr;

		delete mPointerContextManager;
		mPointerContextManager = nullptr;

		MyGUI::FactoryManager::getInstance().unregistryFactory<ResourcePointerContext>("Resource");

		destroyEntities();
	}

	void DemoKeeper::injectMouseMove(int _absx, int _absy, int _absz)
	{
		if (!getGUI())
			return;

		// ��� ������� ������ ������� �����
		if (mRightButtonPressed)
		{
			// ������������� ��������
			int rel_x = _absx - mSaveCursorX;
			int rel_y = _absy - mSaveCursorY;

			_absx = mSaveCursorX;
			_absy = mSaveCursorY;

			setMousePosition(mSaveCursorX, mSaveCursorY);

			// ������� �����
			updateCamera(rel_x, rel_y);
		}
		else
		{
			// ���� ���� �������� ��� ���
			if (!getGUI()->injectMouseMove(_absx, _absy, _absz))
			{
				// ������ �����
				std::string pointer = getCursorFromScene(_absx, _absy);
				mPointerContextManager->setPointer(pointer);
			}
		}
	}

	void DemoKeeper::injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (!getGUI())
			return;

		if (!getGUI()->injectMousePress(_absx, _absy, _id))
		{
			// ������� ����� ������ ����� �� ��� ����
			if (_id == MyGUI::MouseButton::Right)
			{
				mRightButtonPressed = true;
				mSaveCursorX = _absx;
				mSaveCursorY = _absy;
				setPointerVisible(false);
			}
		}
	}

	void DemoKeeper::injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (!getGUI())
			return;

		if (_id == MyGUI::MouseButton::Right)
		{
			mRightButtonPressed = false;
			setPointerVisible(true);
		}

		if (!getGUI()->injectMouseRelease(_absx, _absy, _id))
		{
		}
	}

	void DemoKeeper::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
	{
		// ��� ������� ������
		mControlPanel->injectKeyPress(_key);

		base::BaseManager::injectKeyPress(_key, _text);
	}

	void DemoKeeper::setPointer(const std::string& _name)
	{
		setPointerName(_name);
	}

	void DemoKeeper::updateCamera(int _x, int _y)
	{
		mAngleH += (float)_x * -0.1;
		//mAngleV += (float)_y * -0.05;

		Ogre::Quaternion quatH(Ogre::Radian(Ogre::Degree(mAngleH)), Ogre::Vector3::UNIT_Y);
		Ogre::Quaternion quatV(Ogre::Radian(Ogre::Degree(mAngleV)), Ogre::Vector3::UNIT_X);
		quatH = quatH * quatV;

		Ogre::Vector3 vec(0, 0, 1400);
		vec = quatH * vec;

		vec.y += 120;

		getCamera()->setPosition(vec);
		getCamera()->setOrientation(quatH);
	}

	void DemoKeeper::createEntities()
	{
		Ogre::Entity* entity = getSceneManager()->createEntity("friend", "Mikki_Mesh.mesh");
		Ogre::SceneNode* node = getSceneManager()->getRootSceneNode()->createChildSceneNode();
		node->attachObject(entity);
		node->setPosition(0, 0, 380);
		//node->showBoundingBox(true);

		entity = getSceneManager()->createEntity("enemy", "Mikki_Mesh.mesh");
		node = getSceneManager()->getRootSceneNode()->createChildSceneNode();
		node->attachObject(entity);
		node->setPosition(0, 0, -380);

        Ogre::MeshManager::getSingleton().createPlane(
            "FloorPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
            Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), 2000, 2000, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);

        entity = getSceneManager()->createEntity("floor", "FloorPlane");
        entity->setMaterialName("Ground");
		node = getSceneManager()->getRootSceneNode()->createChildSceneNode();
		node->attachObject(entity);

		gRaySceneQuery = getSceneManager()->createRayQuery(Ogre::Ray());
	}

	void DemoKeeper::destroyEntities()
	{
		getSceneManager()->destroyQuery(gRaySceneQuery);
	}

	std::string DemoKeeper::getCursorFromScene(int _x, int _y)
	{
		MyGUI::IntSize size = getGUI()->getViewSize();
		Ogre::Ray ray = getCamera()->getCameraToViewportRay(
			_x / float(size.width),
			_y / float(size.height));
		gRaySceneQuery->setRay(ray);
		gRaySceneQuery->setSortByDistance(true);
		Ogre::RaySceneQueryResult &result = gRaySceneQuery->execute();
		for (Ogre::RaySceneQueryResult::iterator iter = result.begin(); iter!=result.end(); ++iter)
		{
			if (iter->movable != 0)
			{
				if (iter->movable->getName() == "enemy")
					return "enemy";
				else if (iter->movable->getName() == "friend")
					return "friend";
			}
		}

		return "default";
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
