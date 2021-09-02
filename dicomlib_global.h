#ifndef DICOMLIB_GLOBAL_H
#define DICOMLIB_GLOBAL_H

#include <QtCore/qglobal.h>


#if defined(DICOMLIB_LIBRARY)
#  define DICOMLIB_EXPORT Q_DECL_EXPORT
#else
#  define DICOMLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // DICOMLIB_GLOBAL_H
