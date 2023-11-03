#pragma once
#include <QString>
#include "mainwnd_global.h"
class MAINWND_EXPORT SetConfig
{
public:
	static void SetRunPath(QString strPath);
	static void SetScaling(float fScaling);
};