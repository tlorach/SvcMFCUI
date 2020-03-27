/*
	Copyright (c) 2006 Tristan Lorach (lorachnroll@gmail.com)
	All rights reserved.

	Redistribution and use in source and binary forms, with or
	without modification, are permitted provided that the following
	conditions are met:

	 * Redistributions of source code must retain the above
	   copyright notice, this list of conditions and the following
	   disclaimer.

	   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
	   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
	   REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
	   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
	   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
	   POSSIBILITY OF SUCH DAMAGE. 

*/
#ifndef __FACTORYBASEH__
#define __FACTORYBASEH__

#include <assert.h>

#include <string>

#pragma warning(disable : 4786)
#include <map>
#pragma warning(disable : 4786)
#include <set>
#pragma warning(disable : 4200)
#include "ISvcBase.h"

using namespace std;

namespace nvSvc
{
/**
 ** this method is implemented by each object in order to instanciate itself.
 **/
typedef const void *(*t_NewObjectFunc)(size_t lparam1, size_t lparam2);

/**
 ** an object may derive from this class in order to be referenced by the factory
 **/
struct ObjectTypeDef
{
	t_NewObjectFunc lpNewObjectFunc;
	LPCSTR m_name;
	LPCSTR m_desc;
	LPCSTR m_categories[];
};
/// comparator of strings
struct ltstr
{
  bool operator()(LPCSTR s1, LPCSTR s2) const
  {
	return strcmp(s1, s2) < 0;
  }
};
/// this is the table of available types
typedef map<LPCSTR, ObjectTypeDef*, ltstr> t_objects;
/**
 **	implementation of the Factory of this module
 **/
class CRTFactory : public ISvcFactory
{
private:
	std::string m_strErrMsg;
	t_objects::const_iterator iObj;
	t_objects::const_iterator iSng;
public:
	t_objects m_objecttypes;
	t_objects m_sngtypes;
	/**
	 ** Creation of instances.
	 **/
	const void* CreateInstance(LPCSTR lpInstTypeName, size_t lparam1, size_t lparam2)
	{
		t_objects::const_iterator iObj;
		iObj = m_objecttypes.find(lpInstTypeName);
		if(iObj == m_objecttypes.end()) // Not found
		{
			char tmp[100];
#ifndef _CRT_SECURE_NO_DEPRECATE
			sprintf_s(tmp, 100, "could not find %s", lpInstTypeName);
#else
			sprintf(tmp, "could not find %s", lpInstTypeName);
#endif
			m_strErrMsg = tmp;
			return NULL;
		}
		assert(iObj->second);
		return iObj->second->lpNewObjectFunc(lparam1, lparam2);
	}
	/**
	 ** returns a singleton
	 **/
	const void* GetSingletonOf(LPCSTR lpInstTypeName, size_t lparam1, size_t lparam2)
	{
		t_objects::const_iterator iObj;
		iObj = m_sngtypes.find(lpInstTypeName);
		if(iObj == m_sngtypes.end()) // Not found
		{
			char tmp[100];
#ifndef _CRT_SECURE_NO_DEPRECATE
			sprintf_s(tmp, 100, "could not find %s", lpInstTypeName);
#else
			sprintf(tmp, "could not find %s", lpInstTypeName);
#endif
			m_strErrMsg = tmp;
			return NULL;
		}
		assert(iObj->second);
		return iObj->second->lpNewObjectFunc(lparam1, lparam2);
	}
	/**
	 ** In case of error, this should return the reason
	 **/
	LPCSTR GetLastErrorMsg()
	{
		return m_strErrMsg.c_str();
	}
	/// match categories. for internal use
	bool matchcategories(LPCSTR *objcats, LPCSTR cattofind)
	{
		char tmp[60], *pstrdest;
		LPCSTR pstrsrc;
		if(cattofind == NULL) // all category may be OK
			return true;
		if(*cattofind == '\0') // all category may be OK
			return true;
		pstrsrc = cattofind;
		// walk in the categories to find ',' separated
		while(*pstrsrc != '\0')
		{
			pstrdest = tmp;
			*pstrdest = '\0';
			while((*pstrsrc != ',')&&(*pstrsrc != '\0'))
			{
				*pstrdest++ = *pstrsrc++;
			}
			if(*pstrsrc == ',') 
				pstrsrc++;
			*pstrdest = '\0';
			// loop in the categories of object
			LPCSTR pstr;
			int i=0;
			while(pstr = objcats[i])
			{
				if(!strcmp(pstr, tmp))
					return true;
				++i;
			}
		}
		return false;
	}
	/// total amount of objects. categories is a string with coma separated categories : "cat1,cat2,..."
	virtual int GetNumberOfObjects(LPCSTR filtercat)
	{
		if(!filtercat)
			return (int)m_objecttypes.size();
		t_objects::const_iterator iO;
		// loop in the table of objects types
		int cnt=0;
		iO = m_objecttypes.begin();
		while(iO != m_objecttypes.end())
		{
			if(matchcategories(iO->second->m_categories, filtercat))
				cnt++;
			++iO;
		}
		return cnt;
	}
	/// total amount of singletons. categories is a table of strings
	virtual int GetNumberOfSingletons(LPCSTR filtercat)
	{
		if(!filtercat)
			return (int)m_sngtypes.size();
		t_objects::const_iterator iO;
		// loop in the table of objects types
		int cnt=0;
		iO = m_sngtypes.begin();
		while(iO != m_sngtypes.end())
		{
			if(matchcategories(iO->second->m_categories, filtercat))
				cnt++;
			++iO;
		}
		return cnt;
	}
	/// the brings the first object in the list. \retval returns != 0 if success.
	virtual int GetFirstObjectInfo(LPCSTR &objname, LPCSTR &objdesc, LPCSTR **categories, LPCSTR filtercat)
	{
		iObj = m_objecttypes.begin();
		return GetNextObjectInfo(objname, objdesc, categories, filtercat);
	}
	/// the brings the next object in the list. \retval returns != 0 if success.
	virtual int GetNextObjectInfo(LPCSTR &objname, LPCSTR &objdesc, LPCSTR **categories, LPCSTR filtercat)
	{
		if(filtercat)
			while(iObj != m_objecttypes.end())
			{
				if(matchcategories(iObj->second->m_categories, filtercat))
					break;
			}
		if(iObj == m_objecttypes.end())
			return 0;
		objname = iObj->second->m_name;
		objdesc = iObj->second->m_desc;
		if(categories) *categories = iObj->second->m_categories;
		++iObj;
		return 1;
	}
	/// the brings the first singleton in the list. \retval returns != 0 if success.
	virtual int GetFirstSingletonInfo(LPCSTR &objname, LPCSTR &objdesc, LPCSTR **categories, LPCSTR filtercat)
	{
		iSng = m_sngtypes.begin();
		return GetNextSingletonInfo(objname, objdesc, categories, filtercat);
	}
	/// the brings the next singleton in the list. \retval returns != 0 if success.
	virtual int GetNextSingletonInfo(LPCSTR &objname, LPCSTR &objdesc, LPCSTR **categories, LPCSTR filtercat)
	{
		if(filtercat)
			while(iSng != m_sngtypes.end())
			{
				if(matchcategories(iSng->second->m_categories, filtercat))
					break;
			}
		if(iSng == m_sngtypes.end())
			return 0;
		objname = iSng->second->m_name;
		objdesc = iSng->second->m_desc;
		if(categories) *categories = iSng->second->m_categories;
		++iSng;
		return 1;
	}
	virtual void GetModuleInfos(LPCSTR &desc, LPCSTR &revision, LPCSTR *CPPheader);
};

//
// NOTE: this is for the case where we use a DLL to embed the whole.
//
#define DECLARE_FACTORY_BASICS(s,r) \
	CRTFactory theFactory;\
	HINSTANCE ghModule;\
	__declspec(dllexport) ISvcFactory* GetRTFactory()\
	{\
		return &theFactory;\
	}\
	void CRTFactory::GetModuleInfos(LPCSTR &desc, LPCSTR &revision, LPCSTR *CPPheader)\
	{\
	desc = s;\
	revision = r;\
	if(!CPPheader)\
		return;\
	*CPPheader=NULL;\
	HRSRC HRes = FindResourceA(ghModule, "header","H");\
	if(!HRes)\
		return;\
	HANDLE hglobal	=	LoadResource(ghModule, HRes);\
	if(!hglobal) \
		return;\
	*CPPheader =	(LPCSTR)LockResource(hglobal);\
	}

#define REGISTER_MODULE(hModule) \
	ghModule = (HINSTANCE)hModule;

/// decl the static structure for object description.
#define OBJECT_DESCRIPTION_BEGIN(obj, name, desc, cat) \
	ObjectTypeDef obj::m_typedef =\
	{\
		obj::NewObjectFunc,\
		name,\
		desc,\
		{ cat,
/// To add more categories.
#define OBJECT_DESCRIPTION_ADD_CAT(cat) \
		cat,
/// finished to describe the object
#define OBJECT_DESCRIPTION_END() \
		NULL}\
	};
/// insert this into the class of the object to declare the static variable
#define DECL_OBJECTDESC_VARIABLE() \
	static ObjectTypeDef m_typedef;

/// To register an object to the factory. Use it in DllMain(DLL_PROCESS_ATTACH)
#define REGISTER_OBJECT_TO_FACTORY(obj) \
	theFactory.m_objecttypes[obj::m_typedef.m_name] = &(obj::m_typedef);

/// To register a Singleton to the factory. Use it in DllMain(DLL_PROCESS_ATTACH)
#define REGISTER_SINGLETON_TO_FACTORY(obj) \
	theFactory.m_sngtypes[obj::m_typedef.m_name] = &(obj::m_typedef);

/// To implement the typical instanciation of an object. Add it into your class
#define DECL_NEWOBJECTFUNC(obj, itf) \
	static const void *NewObjectFunc(size_t lparam1, size_t lparam2)\
	{\
		obj *pObj = new obj;\
		return (void*)(static_cast<itf*>(pObj));\
	}


} // namespace sapi

#endif
