// ADOConn.cpp: implementation of the ADOConn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "HosptialMan.h"
#include "ADOConn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ADOConn::ADOConn()
{

}

ADOConn::~ADOConn()
{

}
// 初始化—连接数据库
void  ADOConn::OnInitADOConn()
{
	// 初始化OLE/COM库环境 
	::CoInitialize(NULL);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /*
	try
	{
		// 创建Connection对象
		m_pConnection.CreateInstance("ADODB.Connection");
		// 设置连接字符串，必须是BSTR型或者_bstr_t类型
		_bstr_t strConnect = "Provider=SQLOLEDB.1;Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=高校教材管理数据库;Data Source=ZZZ-8A74889FE59;";
		//Provider=SQLOLEDB.1;Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=高校教材管理数据库;Data Source=ZZZ-8A74889FE59
		//_bstr_t strConnect = "Provider=SQLOLEDB; Server=127.0.0.1;Database=高校教材管理数据库; uid=sa; pwd=sa;"; 以前错误的。
		m_pConnection->Open(strConnect,"","",adModeUnknown);
	}
	// 捕捉异常
	catch(_com_error e)
	{
		// 显示错误信息
		AfxMessageBox(e.Description());
	}*///第一次连接的代码
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HRESULT hr;
	try
  {
   hr=m_pConnection.CreateInstance(__uuidof(Connection));
   if(SUCCEEDED(hr))
   {
    m_pConnection->ConnectionString="File Name=data.udl";
    m_pConnection->ConnectionTimeout=10;
    hr = m_pConnection->Open("","","",adConnectUnspecified);
    if(FAILED(hr))
    {
     AfxMessageBox("数据库连接失败，请联系管理员。");
     //return true;
    }
   }
   else
   {
    AfxMessageBox("createinstance of Connection fail!");
    //return true;
   }
  }
  catch(_com_error e)
  {
   _bstr_t bstrSource (e.Source());
   _bstr_t bstrDescription(e.Description());
   AfxMessageBox(bstrSource + bstrDescription);
   //return true;
  }


}

// 执行查询
_RecordsetPtr&  ADOConn::GetRecordSet(_bstr_t bstrSQL)
{
	try
	{
		// 连接数据库，如果Connection对象为空，则重新连接数据库
		if(m_pConnection==NULL)
			OnInitADOConn();
		// 创建记录集对象
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		// 取得表中的记录
		m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	}
	// 捕捉异常
	catch(_com_error e)
	{
		// 显示错误信息
		AfxMessageBox(e.Description());
	}
	// 返回记录集
	return m_pRecordset;
}

 // 执行SQL语句，Insert Update _variant_t
BOOL ADOConn::ExecuteSQL(_bstr_t bstrSQL)
{
//	_variant_t RecordsAffected;
	try
	{
		// 是否已经连接数据库
		if(m_pConnection == NULL)
			OnInitADOConn();
		// Connection对象的Execute方法:(_bstr_t CommandText, 
		// VARIANT * RecordsAffected, long Options ) 
		// 其中CommandText是命令字串，通常是SQL命令。
		// 参数RecordsAffected是操作完成后所影响的行数, 
		// 参数Options表示CommandText的类型：adCmdText-文本命令；adCmdTable-表名
		// adCmdProc-存储过程；adCmdUnknown-未知
		m_pConnection->Execute(bstrSQL,NULL,adCmdText);
		return true;
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
}

void ADOConn::ExitConnect()
{
	// 关闭记录集和连接
	if (m_pRecordset != NULL)
		m_pRecordset->Close();
	m_pConnection->Close();
	// 释放环境
	::CoUninitialize();
}