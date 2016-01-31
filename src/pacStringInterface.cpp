#include "pacStringInterface.h"

namespace pac {

ParamDictionaryMap  StringInterface::msDictionary;

//------------------------------------------------------------------
ParamCmd* ParamDictionary::getParamCmd(const std::string& name)
{
	ParamMap::iterator iter = mParamMap.find(name);
	//if(iter == mParamMap.end())
	//PAC_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
	//name + " not found ");

	return iter == mParamMap.end() ? 0 : iter->second.paramCmd;
}

//------------------------------------------------------------------
const ParamCmd* ParamDictionary::getParamCmd(const std::string& name) const
{
	ParamMap::const_iterator iter = mParamMap.find(name);
	//if(iter == mParamMap.end())
	//PAC_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
	//name + " not found ");

	return iter == mParamMap.end() ? 0 : iter->second.paramCmd;
}

//------------------------------------------------------------------
const std::string& ParamDictionary::getParamAhName(const std::string& name) const
{
	ParamMap::const_iterator iter = mParamMap.find(name);
	if(iter == mParamMap.end())
		PAC_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
				name + " not found ");

	return iter->first; 
}

//------------------------------------------------------------------
void ParamDictionary::addParameter(const ParamDef& paramDef)
{
	mParamMap.insert(std::make_pair(paramDef.name, paramDef));
}

//------------------------------------------------------------------
void ParamDictionary::addParameter(const std::string& name, const std::string& ahName,
		ParamCmd* paramCmd, const std::string& desc) 
{
	ParamDef paramDef(name, ahName, paramCmd, desc);
	addParameter(paramDef);
}

//------------------------------------------------------------------
StringVector ParamDictionary::getParameters(void) const
{
	return StdUtil::keys(mParamMap);
}

//------------------------------------------------------------------
bool StringInterface::createParamDict(const std::string& className)
{
	ParamDictionaryMap::iterator it = msDictionary.find(className);

	if ( it == msDictionary.end() )
	{
		mParamDict = &msDictionary.insert( 
				std::make_pair( className, ParamDictionary() ) ).first->second;
		mParamDictName = className;
		return true;
	}
	else
	{
		mParamDict = &it->second;
		mParamDictName = className;
		return false;
	}
}

//-----------------------------------------------------------------------
StringVector StringInterface::getParameters(void) const
{
	const ParamDictionary* dict = getParamDict();
	return dict->getParameters();
}

//-----------------------------------------------------------------------
bool StringInterface::setParameter(const std::string& name, const std::string& value)
{
	ParamDictionary* dict = getParamDict();

	ParamCmd* cmd = dict->getParamCmd(name);
	if (cmd)
	{
		cmd->doSet(this, value);
		return true;
	}

	// Fallback
	return false;
}

//------------------------------------------------------------------
bool StringInterface::setParameter(const std::string& name, ArgHandler* handler)
{
	ParamDictionary* dict = getParamDict();

	ParamCmd* cmd = dict->getParamCmd(name);
	if (cmd)
	{
		cmd->doSet(this, handler);
		return true;
	}

	// Fallback
	return false;
}

//-----------------------------------------------------------------------
void StringInterface::setParameterList(const NameValuePairList& paramList)
{
	NameValuePairList::const_iterator i, iend;
	iend = paramList.end();
	for (i = paramList.begin(); i != iend; ++i)
	{
		setParameter(i->first, i->second);
	}
}

//------------------------------------------------------------------
std::string StringInterface::getParameter(const std::string& name) const
{
	const ParamDictionary* dict = getParamDict();
	const ParamCmd* cmd = dict->getParamCmd(name);
	return cmd->doGet(this);
}

//------------------------------------------------------------------
const std::string& StringInterface::getValueArgHandler(const std::string& name)
{
	const ParamDictionary* dict = getParamDict();
	return dict->getParamAhName(name);
}

//------------------------------------------------------------------
void StringInterface::copyParametersTo(StringInterface* dest) const
{
	// Get dictionary
	const ParamDictionary* dict = getParamDict();

	if (dict)
	{
		// Iterate through own parameters
		ParamMap::const_iterator i;

		for (i = dict->mParamMap.begin(); 
				i != dict->mParamMap.end(); ++i)
		{
			dest->setParameter(i->first, getParameter(i->first));
		}
	}
}

//-----------------------------------------------------------------------
void StringInterface::cleanupDictionary ()
{
	msDictionary.clear();
}

}
