#include "callback.hpp"

#if 0
//putting this here actually prevented some optimizations, so still inline

CBFunctorBase::CBFunctorBase(const void *c,_Func f, const void *mf,size_t sz)
	{
	if(c)	//must be callee/memfunc
		{
		callee = (void *)c;
		memcpy(memFunc,mf,sz);
		if(sz<MEM_FUNC_SIZE)	//zero-out the rest if any, so comparisons work
			{
			memset(memFunc+sz,0,MEM_FUNC_SIZE-sz);
			}
		}
	else	//must be ptr-to-func
		{
		func = f;
		}
	}
#endif

RHCB_BOOL operator==(const CBFunctorBase &lhs,const CBFunctorBase &rhs)
	{
	if(!lhs.callee)
		{
		if(rhs.callee)
			return RHCB_FALSE;
		return lhs.func == rhs.func;
		}
	else
		{
		if(!rhs.callee)
			return RHCB_FALSE;
		return lhs.callee == rhs.callee &&
			!memcmp(lhs.memFunc,rhs.memFunc,CBFunctorBase::MEM_FUNC_SIZE);
		}
	}

RHCB_BOOL operator!=(const CBFunctorBase &lhs,const CBFunctorBase &rhs)
	{
	return !operator==(lhs,rhs);
	}

RHCB_BOOL operator<(const CBFunctorBase &lhs,const CBFunctorBase &rhs)
	{
	//must order across funcs and callee/memfuncs, funcs are first
	if(!lhs.callee)
		{
		if(rhs.callee)
			return RHCB_TRUE;
		else
			return lhs.func < rhs.func;
		}
	else
		{
		if(!rhs.callee)
			return RHCB_FALSE;
		if(lhs.callee != rhs.callee)
			return lhs.callee < rhs.callee;
		else
			return memcmp(lhs.memFunc,rhs.memFunc,CBFunctorBase::MEM_FUNC_SIZE)<0;
		}
	}

