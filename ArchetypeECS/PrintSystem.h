#pragma once
#include "System.h"

class PrintSystem : public ECS::System
{
public:
	virtual void Init(ECS::Engine* owner) override;
	virtual void Run() override;
};