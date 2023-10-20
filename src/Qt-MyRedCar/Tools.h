#pragma once
class Tools
{
public:
	template<class T>
	static void Delete_Ex(T*& pointer)
	{
		if (pointer != nullptr)
		{
			delete pointer;
			pointer = nullptr;
		}
	}
};