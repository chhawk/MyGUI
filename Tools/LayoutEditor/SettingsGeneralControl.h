/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/
#ifndef __SETTINGS_GENERAL_CONTROL_H__
#define __SETTINGS_GENERAL_CONTROL_H__

#include "BaseLayout/BaseLayout.h"

namespace tools
{
	class SettingsGeneralControl :
		public wraps::BaseLayout
	{
	public:
		SettingsGeneralControl(MyGUI::Widget* _parent = nullptr);
		virtual ~SettingsGeneralControl();

		void loadSettings();
		void saveSettings();

	private:
		void notifyNewGridStep(MyGUI::Widget* _sender, MyGUI::Widget* _new = 0);
		void notifyNewGridStepAccept(MyGUI::Edit* _sender); // calls previous method

		int getGridStep();
		void setGridStep();

	private:
		int mGridStep;

		MyGUI::Edit* mGridEdit;
		MyGUI::ComboBox* mLayoutVersion;
	};

} // namespace tools

#endif // __SETTINGS_GENERAL_CONTROL_H__
