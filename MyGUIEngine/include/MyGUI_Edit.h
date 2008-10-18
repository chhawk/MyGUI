/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_EDIT_H__
#define __MYGUI_EDIT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_TextChangeHistory.h"
#include "MyGUI_TextIterator.h"

namespace MyGUI
{

	class _MyGUIExport Edit : public Widget
	{
		// ��� ������ ��������� ������������
		friend class factory::BaseWidgetFactory<Edit>;

		MYGUI_RTTI_CHILD_HEADER;

	protected:
		Edit(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);
		virtual ~Edit();

	public:
		/** Set edit text applying tags */
		virtual void setCaption(const Ogre::UTFString & _caption);
		/** Get edit text with tags */
		virtual const Ogre::UTFString& getCaption();

		/** Set edit text without tags */
		void setOnlyText(const Ogre::UTFString & _text) {setText(TextIterator::toTagsString(_text), false);}
		/** Get edit text without tags */
		Ogre::UTFString getOnlyText() {return TextIterator::getOnlyText(getRealString());}

		// ���������� ���������
		/** Get selected text interval
			@param _start of interval will be written here
			@param _end of interval will be written here
		*/
		void getTextSelect(size_t & _start, size_t & _end);
		// ������������� ���������
		/** Set selected text interval
			@param _start of interval
			@param _end of interval
		*/
		void setTextSelect(size_t _start, size_t _end);
		// ������� �� �����
		/** Is any text selected */
		bool isTextSelect() {return ( (mStartSelect != ITEM_NONE) && (mStartSelect != mEndSelect) );}
		// ���������� ���������� �����
		/** Get selected text */
		Ogre::UTFString getSelectedText();

		/** Get text length excluding tags
			For example "#00FF00Hello" length is 5
		*/
		size_t getTextLength() {return mTextLength;}
		/** Get text cursor position */
		size_t getTextCursor() {return mCursorPosition;}

		/** Set text cursor position */
		void setTextCursor(size_t _index);

		//! @copydoc Widget::setTextAlign
		virtual void setTextAlign(Align _align);

		//! Sets if surplus characters should push characters off the left side rather than ignored
		void setOverflowToTheLeft(bool _overflowToLeft) { mOverflowToTheLeft = _overflowToLeft; }

		//! Returns true if surplus characters will be pushed off the left rather than ignored
		bool getOverflowToTheLeft() { return mOverflowToTheLeft; }

		//! Sets the max amount of text allowed in the edit field.
		void setMaxTextLength(size_t _maxTextLength) { mMaxTextLength = _maxTextLength; }

		//! Gets the max amount of text allowed in the edit field.
		size_t getMaxTextLength() { return mMaxTextLength; }

		//---------------------------------------------------------------//
		// ������ ���������� ������� �� ������� �������
		//---------------------------------------------------------------//

		// ���������� ����� � ������
		/** Get _count characters with tags from _start position */
		Ogre::UTFString getText(size_t _start, size_t _count);
		// ������� ��� ��� ���������
		/** Delete selected text */
		bool deleteTextSelect() {return deleteTextSelect(false);}
		// ��������� ����� � ��������� �������
		/** Inser text at _index position (text end by default) */
		void insertText(const Ogre::UTFString & _text, size_t _index = ITEM_NONE) {insertText(_text, _index, false);}
		// ��������� ����� � �����
		/** Add text */
		void addText(const Ogre::UTFString & _text) {insertText(_text, ITEM_NONE, false);}
		/** Erase _count characters from _start position */
		void eraseText(size_t _start, size_t _count = 1) {eraseText(_start, _count, false);}
		// �������� ������ ���������
		/** Colour selected text */
		void setTextSelectColour(const Ogre::ColourValue & _colour)
		{
			setTextSelectColour(_colour, false);
		}
		// �������� ������ ��������
		/** Colour interval */
		void setTextColour(size_t _start, size_t _count, const Ogre::ColourValue & _colour)
		{
			setTextColour(_start, _count, _colour, false);
		}

		/** Get edit read only mode flag */
		bool getEditReadOnly() {return mModeReadOnly;}
		/** Enable or disable edit read only mode */
		void setEditReadOnly(bool _read)
		{
			mModeReadOnly = _read;
			// ���������� �������
			commandResetHistory();
		}

		/** Get edit password mode flag */
		bool getEditPassword() {return mModePassword;}
		/** Enable or disable edit password mode */
		void setEditPassword(bool _password);

		/** Get edit multiline mode flag */
		bool getEditMultiLine() {return mModeMultiline;}
		/** Enable or disable edit multiline mode */
		void setEditMultiLine(bool _multi)
		{
			mModeMultiline = _multi;
			// �� ������, ��� �������� ���������
			if (false == mModeMultiline) {
				setText(getRealString(), false);
			}
			// ��������� �� ��������
			else updateView(false);
			// ���������� �������
			commandResetHistory();
		}

		/** Enable or disable edit static mode */
		void setEditStatic(bool _static)
		{
			mModeStatic = _static;
			resetSelect();
			if (mModeStatic) mWidgetClient->setPointer("");
			else mWidgetClient->setPointer(mOriginalPointer);
		}

		/** Get edit static mode flag */
		bool getEditStatic() {return mModeStatic;}

		/** Set edit password character ('*' by default) */
		void setPasswordChar(Char _char);
		/** Set edit password character ('*' by default). First character of string used. */
		void setPasswordChar(const Ogre::UTFString & _char)
		{
			if (false == _char.empty()) setPasswordChar(_char[0]);
		}
		/** Get edit password character */
		Char getPasswordChar() { return mCharPassword; }

		/** Get edit word wrap mode flag */
		bool getEditWordWrap() { return mModeWordWrap; }
		/** Enable or disable edit word wrap mode */
		void setEditWordWrap(bool _wordwrap);


		//! @copydoc Widget::setPosition(const IntPoint & _point)
		virtual void setPosition(const IntPoint & _point);
		//! @copydoc Widget::setSize(const IntSize& _size)
		virtual void setSize(const IntSize & _size);
		//! @copydoc Widget::setCoord(const IntCoord & _coord)
		virtual void setCoord(const IntCoord & _coord);

		/** @copydoc Widget::setPosition(int _left, int _top) */
		void setPosition(int _left, int _top) { setPosition(IntPoint(_left, _top)); }
		/** @copydoc Widget::setSize(int _width, int _height) */
		void setSize(int _width, int _height) { setSize(IntSize(_width, _height)); }
		/** @copydoc Widget::setCoord(int _left, int _top, int _width, int _height) */
		void setCoord(int _left, int _top, int _width, int _height) { setCoord(IntCoord(_left, _top, _width, _height)); }

		MYGUI_OBSOLETE("use Widget::setCoord(const IntCoord& _coord)")
		void setPosition(const IntCoord & _coord) { setCoord(_coord); }
		MYGUI_OBSOLETE("use Widget::setCoord(int _left, int _top, int _width, int _height)")
		void setPosition(int _left, int _top, int _width, int _height) { setCoord(_left, _top, _width, _height); }

		/** Show VScroll when text size larger than Edit */
		void showVScroll(bool _show) { mShowVScroll = _show; updateView(false); }
		/** Get Show VScroll flag */
		bool isShowVScroll() { return mShowVScroll; }

		/** Show HScroll when text size larger than Edit */
		void showHScroll(bool _show) { mShowHScroll = _show; updateView(false); }
		/** Get Show HScroll flag */
		bool isShowHScroll() { return mShowHScroll; }

		/** Event : Enter pressed (Ctrl+enter in multiline mode).\n
			signature : void method(MyGUI::WidgetPtr _sender)
		*/
		EventInfo_WidgetVoid eventEditSelectAccept;

		/** Event : Text changed.\n
			signature : void method(MyGUI::WidgetPtr _sender)
		*/
		EventInfo_WidgetVoid eventEditTextChange;

	protected:

		// ������������� �����
		void setText(const Ogre::UTFString & _text, bool _history);
		// ������� ��� ��� ���������
		bool deleteTextSelect(bool _history);
		// ��������� ����� � ��������� �������
		void insertText(const Ogre::UTFString & _text, size_t _index, bool _history);
		// ������� �����
		void eraseText(size_t _start, size_t _count, bool _history);
		// �������� ������ ���������
		void setTextSelectColour(const Ogre::ColourValue & _colour, bool _history);
		// �������� ������ ��������
		void setTextColour(size_t _start, size_t _count, const Ogre::ColourValue & _colour, bool _history);

	protected:
		void frameEntered(float _frame);

		// ����� ������ ��� ������ � �����
		void notifyMouseSetFocus(WidgetPtr _sender, WidgetPtr _old);
		void notifyMouseLostFocus(WidgetPtr _sender, WidgetPtr _new);
		void notifyMousePressed(WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyMouseReleased(WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyMouseDrag(WidgetPtr _sender, int _left, int _top);
		void notifyMouseButtonDoubleClick(WidgetPtr _sender);

		void notifyScrollChangePosition(WidgetPtr _sender, size_t _position);
		void notifyMouseWheel(WidgetPtr _sender, int _rel);

		virtual void onMouseDrag(int _left, int _top);
		virtual void onKeyLostFocus(WidgetPtr _new);
		virtual void onKeySetFocus(WidgetPtr _old);
		virtual void onKeyButtonPressed(KeyCode _key, Char _char);

		void updateEditState();

		// ��������� ������ �� ����������
		void updateSelectText();
		// ���������� �������������
		void updateView(bool _showCursor);

		void resetSelect();

		// ������ � ������� ������ � �������
		void commandPosition(size_t _undo, size_t _redo, size_t _length, VectorChangeInfo * _info = null);

		// �������� ������ � ������
		bool commandRedo();
		bool commandUndo();
		// ���������� ��������� ��� ��������
		void commandMerge();
		// �������
		void commandResetRedo() {mVectorRedoChangeInfo.clear();}
		void commandResetHistory() {mVectorRedoChangeInfo.clear();mVectorUndoChangeInfo.clear();}
		void saveInHistory(VectorChangeInfo * _info = null);

		// ������ � ������� ������
		void commandCut();
		void commandCopy();
		void commandPast();


		const Ogre::UTFString & getRealString();

		void setRealString(const Ogre::UTFString & _caption);

		void updateScroll();



	protected:
		// ������ �� ������
		bool mIsPressed;
		// � ������ �� ������
		bool mIsFocus;

		//WidgetPtr mWidgetUpper;

		bool mCursorActive;
		float mCursorTimer, mActionMouseTimer;

		// ������� �������
		size_t mCursorPosition;
		// ������������ �����������
		size_t mTextLength;

		// ���������
		size_t mStartSelect, mEndSelect;

		// ������� ��������� ��� ������ � �������
		DequeUndoRedoInfo mVectorUndoChangeInfo;
		DequeUndoRedoInfo mVectorRedoChangeInfo;

		bool mMouseLeftPressed;

		bool mModeReadOnly;
		bool mModePassword;
		bool mModeMultiline;
		bool mModeStatic;
		bool mModeWordWrap;

		// ��������� �����, �������� �� �����������
		Ogre::UTFString mPasswordText;

		// ��� ��������� ������ ������, ��� ������ �� �����
		std::string mOriginalPointer;

		Char mCharPassword;

		bool mOverflowToTheLeft;
		size_t mMaxTextLength;

		VScrollPtr mVScroll;
		HScrollPtr mHScroll;

		bool mShowHScroll;
		bool mShowVScroll;

		size_t mVRange;
		size_t mHRange;

	}; // class Edit : public Widget

} // namespace MyGUI

#endif // __MYGUI_EDIT_H__
