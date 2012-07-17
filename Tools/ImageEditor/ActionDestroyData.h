/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _d92392a8_2d98_4639_8dfe_7cb945c817b5_
#define _d92392a8_2d98_4639_8dfe_7cb945c817b5_

#include "Action.h"
#include "Data.h"

namespace tools
{

	class ActionDestroyData :
		public Action
	{
	public:
		ActionDestroyData();
		virtual ~ActionDestroyData();

		virtual void doAction();
		virtual void undoAction();

		void setData(Data* _data);
		void setUniqueProperty(const std::string& _value);

	private:
		Data* mData;
		Data* mParent;
		bool mComplete;
		size_t mIndex;
		std::string mUniqueProperty;

		typedef std::pair<Property*, std::string> PairProprty;
		typedef std::vector<PairProprty> VectorPairProperty;
		VectorPairProperty mOldValues;
	};

}

#endif
