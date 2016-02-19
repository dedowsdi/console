#include "pacStable.h"
#include "pacStdUtil.h"

namespace pac
{

//------------------------------------------------------------------------------
std::string StdUtil::getIdenticalString(StringVector::iterator beg,
		StringVector::iterator end)
{
	if(end == beg)
		return "";
	if(end == beg + 1)
		return *beg;

	std::stringstream ss;

	//loop every char of vec[0]
	size_t index = 0;
	for (; index != beg->size(); ++index)
	{
		bool match = std::all_of(beg + 1, end, [&](const std::string& v)
				{
				return v.size() > index && v[index] == (*beg)[index];
				});
		if(match)
			ss << (*beg)[index];
		else
			break;
	}
	return ss.str();
}

}
