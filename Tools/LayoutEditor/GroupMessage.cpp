#include "precompiled.h"
#include "GroupMessage.h"

inline const MyGUI::UString localise(const MyGUI::UString & _str)
{
	return MyGUI::LanguageManager::getInstance().getTag(_str);
}

MYGUI_INSTANCE_IMPLEMENT(GroupMessage);

void GroupMessage::addMessage(const MyGUI::UString & _message, MyGUI::LogManager::LogLevel _type)
{
	if (_type == MyGUI::LogManager::Error)
		mErrorMessages.push_back(_message);
	else /*if (_type == MyGUI::LogManager::Warning)*/
		mWarningMessages.push_back(_message);
}

void GroupMessage::showMessages()
{
	if (mWarningMessages.size())
	{
		MyGUI::UString warningMess;
		for (MyGUI::VectorString::iterator iter = mWarningMessages.begin(); iter != mWarningMessages.end(); ++iter)
		{
			if (warningMess.empty()) warningMess = warningMess + *iter;
			else warningMess = warningMess + "\n" + *iter;
		}
		if (!warningMess.empty())
			MyGUI::Message::createMessageBox("Message", localise("Warning"), warningMess, MyGUI::MessageBoxStyle::IconWarning | MyGUI::MessageBoxStyle::Ok, "Overlapped");

		mWarningMessages.clear();
	}

	if (mErrorMessages.size())
	{
		MyGUI::UString errorMessages;
		for (MyGUI::VectorString::iterator iter = mErrorMessages.begin(); iter != mErrorMessages.end(); ++iter)
		{
			if (errorMessages.empty()) errorMessages = errorMessages + *iter;
			else errorMessages = errorMessages + "\n" + *iter;
		}
		if (!errorMessages.empty())
			MyGUI::Message::createMessageBox("Message", localise("Error"), errorMessages , MyGUI::MessageBoxStyle::IconError | MyGUI::MessageBoxStyle::Ok, "Overlapped");

		mErrorMessages.clear();
	}
}