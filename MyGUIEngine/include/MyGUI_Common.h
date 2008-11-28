/*!
	@file
	@author		Albert Semenov
	@author		baho_is
	@date		11/2007
	@module
*/
#ifndef __MYGUI_COMMON_H__
#define __MYGUI_COMMON_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Macros.h"
#include "MyGUI_LogManager.h"
#include "MyGUI_Instance.h"
#include "MyGUI_Types.h"
#include "MyGUI_RenderOut.h"
#include "MyGUI_ResourcePath.h"
#include "MyGUI_Utility.h"
#include "MyGUI_InputDefine.h"

#include <OgreException.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	#define MYGUI_LOG_SECTION "General"
	#define MYGUI_LOG_FILENAME "MyGUI.log"
	#define MYGUI_LOG(level, text) LOGGING(MYGUI_LOG_SECTION, level, text)

	class _MyGUIExport MyGUIException : public Ogre::Exception
	{
	public:
		MyGUIException(int number, const Ogre::String& description, const Ogre::String& source, const char* file, long line)
			: Exception(number, description, source, "MyGUIException", file, line) {}
	};

	// just other number
	#define ERR_MY_GUI Ogre::Exception::ERR_NOT_IMPLEMENTED+1
	static inline MyGUIException createException(
			Ogre::ExceptionCodeType<ERR_MY_GUI> code,
			const Ogre::String& desc,
			const Ogre::String& src, const char* file, long line)
		{
			return MyGUIException(code.number, desc, src, file, line);
		}

	// copy of OGRE_EXCEPT with MyGUIException create
	#define OGRE_BASED_EXCEPT(desc, src)	throw MyGUI::createException(Ogre::ExceptionCodeType<ERR_MY_GUI>(), desc, src, __FILE__, __LINE__ );

	#define MYGUI_EXCEPT(dest) \
	{ \
		MYGUI_LOG(Critical, dest); \
		std::ostringstream stream; \
		stream << dest << "\n"; \
		OGRE_BASED_EXCEPT(stream.str(), "MyGUI"); \
	}

	#define MYGUI_ASSERT(exp, dest) \
	{ \
		if ( ! (exp) ) { \
			MYGUI_LOG(Critical, dest); \
			std::ostringstream stream; \
			stream << dest << "\n"; \
			OGRE_BASED_EXCEPT(stream.str(), "MyGUI"); \
		} \
	}

	#define MYGUI_ERROR(_throw, dest)					\
	{													\
		MYGUI_LOG(Critical, dest); 						\
		if(_throw){										\
			std::ostringstream stream;					\
			stream << dest << "\n"; 					\
			OGRE_BASED_EXCEPT(stream.str(), "MyGUI");	\
		}												\
	}

	#define MYGUI_ASSERT_RANGE(index, size, owner) MYGUI_ASSERT(index < size, owner << " : index number " << index << " out of range [" << size << "]");
	#define MYGUI_ASSERT_RANGE_AND_NONE(index, size, owner) MYGUI_ASSERT(index < size || index == ITEM_NONE, owner << " : index number " << index << " out of range [" << size << "]");
	#define MYGUI_ASSERT_RANGE_INSERT(index, size, owner) MYGUI_ASSERT((index <= size) || (index == MyGUI::ITEM_NONE), owner << " : insert index number " << index << " out of range [" << size << "] or not ITEM_NONE");

	#if MYGUI_DEBUG_MODE == 1
		#define MYGUI_REGISTER_VALUE(map, value) \
		{ \
			MYGUI_LOG(Info, "Register value : '" << #value << "' = " << (int)value); \
			map[#value] = value; \
		}
		#define MYGUI_DEBUG_ASSERT(exp, dest) MYGUI_ASSERT(exp, dest)
	#else
		#define MYGUI_REGISTER_VALUE(map, value) map[#value] = value;
		#define MYGUI_DEBUG_ASSERT(exp, dest) ((void)0)
	#endif


/// for more info see: http://mdf-i.blogspot.com/2008/09/deprecated-gcc-vs-vs-vs-vs.html
	#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC
		#define MYGUI_OBSOLETE_START(text) __declspec(deprecated(text))
        #define MYGUI_OBSOLETE_END

	#elif MYGUI_COMPILER == MYGUI_COMPILER_GNUC
		#if MYGUI_PLATFORM == MYGUI_PLATFORM_LINUX && MYGUI_COMP_VER == 412
			#define MYGUI_OBSOLETE_START(text)
            #define MYGUI_OBSOLETE_END
		#else
            #define MYGUI_OBSOLETE_START(text)
			#define MYGUI_OBSOLETE_END __attribute__((deprecated))
		#endif
	#else
		#define MYGUI_OBSOLETE_START(text)
		#define MYGUI_OBSOLETE_END
	#endif

    #define MYGUI_OBSOLETE(text) MYGUI_OBSOLETE_START(text)MYGUI_OBSOLETE_END

} // namespace MyGUI


#endif // __MYGUI_COMMON_H__
