#ifndef CANAPI_GLOBAL_H
#define CANAPI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CANAPI_LIBRARY)
#  define CANAPI_EXPORT Q_DECL_EXPORT
#else
#  define CANAPI_EXPORT Q_DECL_IMPORT
#endif

#endif // CANAPI_GLOBAL_H
