#ifndef FuncObject_h_
#define FuncObject_h_
namespace HDFTools
{
	class FuncObject
	{
	public:
		FuncObject();
		virtual ~FuncObject();
		virtual int Execute();
		virtual void SetParam(void * lparam,void *rparam){}
	};
}

#endif