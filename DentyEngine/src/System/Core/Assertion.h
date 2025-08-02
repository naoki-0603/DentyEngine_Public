#pragma once

namespace DentyEngine
{
#if defined( DEBUG ) || defined( _DEBUG )
#define DENTY_ASSERT(expr, msg) \
	(void)((!!(expr)) || \
	(1 != _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, "%s", msg)) || \
	(_CrtDbgBreak(), 0))
#else
#define  DENTY_ASSERT(expr, expr_str) ((void)0)
#endif
};
