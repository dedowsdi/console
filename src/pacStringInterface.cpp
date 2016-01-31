#include "pacStringInterface.h"

namespace pac {

ParamDictionaryMap  StringInterface::msDictionary;

//------------------------------------------------------------------
ParamCmd* ParamDictionary::getParamCmd(const String& name)
{
	ParamMap::iterator iter = mParamMap.find(name);
	//if(iter == mParamMap.end())
	//PAC_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
	//name + " not found ", __FUNCTION__);

	return iter == mParamMap.end() ? 0 : iter->second.paramCmd;
}

//------------------------------------------------------------------
const ParamCmd* ParamDictionary::getParamCmd(const String& name) const
{
	ParamMap::const_iterator iter = mParamMap.find(name);
	//if(iter == mParamMap.end())
	//PAC_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
	//name + " not found ", __FUNCTION__);

	return iter == mParamMap.end() ? 0 : iter->second.paramCmd;
}

//------------------------------------------------------------------
const String& ParamDictionary::getParamAhName(const String& name) const
{
	ParamMap::const_iterator iter = mParamMap.find(name);
	if(iter == mParamMap.end())
		PAC_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
				name + " not found ", __FUNCTION__);

	return iter->name; 
}

//------------------------------------------------------------------
void ParamDictionary::addParameter(const ParamDef& paramDef)
{
	mParamMap[paramDef.name] = paramDef;
}

//------------------------------------------------------------------
void ParamDictionary::addParameter(const String& name, const String& ahName,
		ParamCmd* paramCmd, const String& desc = "") 
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
bool StringInterface::createParamDict(const String& className)
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

StringInterface::msDictionaryMutex;
ParamDictionaryMap StringInterface::msDictionary;

//-----------------------------------------------------------------------
StringVector StringInterface::getParameters(void) const
{
	const ParamDictionary* dict = getParamDict();
	return dict->getParameters();
}

//-----------------------------------------------------------------------
bool StringInterface::setParameter(const String& name, const String& value)
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
String StringInterface::getParameter(const String& name) const
{
	const ParamDictionary* dict = getParamDict();
	const ParamCmd* cmd = dict->getParamCmd(name);
	return cmd->doGet(this);
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

		for (i = dict->mParams.begin(); 
				i != dict->mParams.end(); ++i)
		{
			dest->setParameter(i->name, getParameter(i->name));
		}
	}
}

//-----------------------------------------------------------------------
void StringInterface::cleanupDictionary ()
{
	msDictionary.clear();
}

}
