/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_OgreDataManager.h"
#include "MyGUI_OgreDiagnostic.h"

#include <Ogre.h>

#include "MyGUI_LastHeader.h"

#if MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>
#endif

namespace MyGUI
{

	#if MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
	// This function will locate the path to our application on OS X,
	// unlike windows you can not rely on the curent working directory
	// for locating your configuration files and resources.
	std::string MYGUI_EXPORT macBundlePath()
	{
		char path[1024];
		CFBundleRef mainBundle = CFBundleGetMainBundle();    assert(mainBundle);
		CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);    assert(mainBundleURL);
		CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);    assert(cfStringRef);
		CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);
		CFRelease(mainBundleURL);
		CFRelease(cfStringRef);
		return std::string(path);
	}
	#endif

	MYGUI_INSTANCE_IMPLEMENT(OgreDataManager);

	void OgreDataManager::initialise(const std::string& _group)
	{
		MYGUI_PLATFORM_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		mGroup = _group;

		MYGUI_PLATFORM_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void OgreDataManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		MYGUI_PLATFORM_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	Data* OgreDataManager::getData(const std::string& _name)
	{
		try
		{
			Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(_name, mGroup);

			Data* data = new Data();
			data->setSize(stream->size());
			stream->read(data->getData(), data->getSize());

			return data;
		}
		catch(Ogre::FileNotFoundException)
		{
		}

		return nullptr;
	}

	bool OgreDataManager::isDataExist(
		const std::string& _pattern,
		bool _unique,
		bool _fullmatch)
	{
		const VectorString& files = getVectorDataPath(_pattern, false, _fullmatch);
		if ((_unique && files.size() == 1) || !files.empty()) return true;
		return false;
	}

	std::string OgreDataManager::getDataPath(
		const std::string& _pattern,
		bool _fullpath,
		bool _unique,
		bool _fullmatch)
	{
		const VectorString& files = getVectorDataPath(_pattern, _fullpath, _fullmatch);
		if ((_unique && files.size() == 1) || !files.empty()) return files[0];
		return "";
	}

	const VectorString& OgreDataManager::getVectorDataPath(
		const std::string& _pattern,
		bool _fullpath,
		bool _fullmatch)
	{
		static VectorString result;
		result.clear();

//#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
//		Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(mGroup, convert::utf8_to_ansi(_pattern));
//#else
		Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(mGroup, _pattern);
//#endif

		result.reserve(pFileInfo->size());

		for (Ogre::FileInfoList::iterator fi = pFileInfo->begin(); fi != pFileInfo->end(); ++fi )
		{
			if (!_fullmatch || fi->path.empty())
			{
				if (_fullpath)
				{
					std::string path = fi->archive->getName() + "/" + fi->filename;
					bool find = false;
					for (VectorString::iterator iter=result.begin(); iter!=result.end(); ++iter)
					{
						if (*iter == path) { find = true; break; }
					}
					if (!find)
					{
//#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
//						result.push_back(convert::ansi_to_utf8(path));
//#else
						result.push_back(path);
//#endif
					}
				}
				else
				{
					bool find = false;
					for (VectorString::iterator iter=result.begin(); iter!=result.end(); ++iter)
					{
						if (*iter == fi->filename) { find = true; break; }
					}
					if (!find)
					{
//#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
//						result.push_back(convert::ansi_to_utf8(fi->filename));
//#else
						result.push_back(fi->filename);
//#endif
					}
				}

			}
		}

		pFileInfo.setNull();

		return result;
	}

} // namespace MyGUI