//#include "stdafx.h"
#include "XmlWriter.h"

#pragma region Constructor & DeConstructor
XmlWriter::XmlWriter(string file)
{
	Reset();
	m_sFile = file;
}


XmlWriter::~XmlWriter(void)
{
	Reset();
} 
#pragma endregion



#pragma region Protected Method
void XmlWriter::Reset()
{
	_sFile			= "";
	_XmlDoc.Clear();
}
#pragma endregion


#pragma region Public Method
XmlWriter& XmlWriter::WriteStartElement(string localName)
{	
	m_StartNodes.push(m_pCurrentNode->InsertEndChild(TiXmlElement(localName.c_str())));

	return *this;
}

XmlWriter& XmlWriter::WriteEndElement()
{
	if(!m_StartNodes.empty())
		m_StartNodes.pop();

	return *this;
}

XmlWriter& XmlWriter::WriteElementValue(string localName, const char* value)
{
	m_pCurrentNode
		->InsertEndChild(TiXmlElement(localName.c_str()))
		->InsertEndChild(TiXmlText(value));

	return *this;
}

XmlWriter& XmlWriter::WriteElementValue(string localName, string value)
{
	return WriteElementValue(localName, value.c_str());
}

XmlWriter& XmlWriter::WriteElementValue(string localName, int value)
{
	_itoa(value, m_cBuffer,10);
	return WriteElementValue(localName, m_cBuffer);
}

void XmlWriter::Close()
{
	m_XmlDoc.SaveFile(m_sFile.c_str());
	Reset();
}
#pragma endregion