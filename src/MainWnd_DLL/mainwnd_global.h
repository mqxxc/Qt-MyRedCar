#pragma once
#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(MAINWND_LIB)
#  define MAINWND_EXPORT Q_DECL_EXPORT
# else
#  define MAINWND_EXPORT Q_DECL_IMPORT
# endif
#else
# define MAINWND_EXPORT
#endif