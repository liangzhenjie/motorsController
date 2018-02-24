#ifndef MOTORSCONTROLL_GLOBAL_H
#define MOTORSCONTROLL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MOTORSCONTROLL_LIBRARY)
#  define MOTORSCONTROLLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MOTORSCONTROLLSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MOTORSCONTROLL_GLOBAL_H