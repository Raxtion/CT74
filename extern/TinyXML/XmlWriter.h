#pragma once
#include <stack>
#include <string>
#include "tinyxml.h"   
#include "tinystr.h"

using namespace std;
class XmlWriter
{
#pragma region Const
private:
#define DEFAULT_BUFFER_SIZE 512
#pragma endregion


#pragma region Var
private:
	char				m_cBuffer[DEFAULT_BUFFER_SIZE];
	string				_sFile;
	TiXmlDocument		_XmlDoc;	
	stack<TiXmlNode*>	_StartNodes;
#pragma endregion


#pragma region Private Property
private:
	__declspec(property(get=Get_sFile,put=Set_sFile))
		string m_sFile;

	__declspec(property(get=Get_XmlDoc,put=Set_XmlDoc))
		TiXmlDocument& m_XmlDoc;

	__declspec(property(get=Get_StartNodes))
		stack<TiXmlNode*>& m_StartNodes;

	__declspec(property(get=Get_pCurrentNode,put=Set_pCurrentNode))
		TiXmlNode* m_pCurrentNode;
#pragma endregion


#pragma region Constructor & DeConstructor
public:
	XmlWriter(string file);
	~XmlWriter(void); 
#pragma endregion


#pragma region Property Process Method
private:
	inline string Get_sFile()
	{
		return _sFile;
	}

	inline void Set_sFile(string value)
	{
		if(_sFile == value)
			return;

		_sFile = value;
	}

	inline TiXmlDocument& Get_XmlDoc()
	{
		return _XmlDoc;
	}

	inline void Set_XmlDoc(TiXmlDocument& value)
	{
		_XmlDoc = value;
	}

	inline TiXmlNode* Get_pCurrentNode()
	{		
		if(m_StartNodes.empty())
		{
			return &m_XmlDoc;
		}
		return m_StartNodes.top();
	}

	inline stack<TiXmlNode*>& Get_StartNodes()
	{
		return _StartNodes;
	}
#pragma endregion


#pragma region Protected Method
protected:
	void Reset();
#pragma endregion


#pragma region Public Method
public:
	XmlWriter& WriteStartElement(string localName);
	XmlWriter& WriteEndElement();
	XmlWriter& WriteElementValue(string localName, const char* value);
	XmlWriter& WriteElementValue(string localName, string value);
	XmlWriter& WriteElementValue(string localName, int value);
	void Close();
#pragma endregion
};