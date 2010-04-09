#pragma once

#include "stdafx.h"

struct sMaterial;


class TResourceMgr
{
public:
	TResourceMgr(){}
	~TResourceMgr(){}
	
	bool	LoadDae();
	uint32 RenderLoadTexture(const string& strFilePath);

protected:
private:
};