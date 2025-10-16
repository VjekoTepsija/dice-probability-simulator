#pragma once	

class IRng {

public:
	virtual ~IRng() {};

	virtual int nextInt(int min, int max) = 0;
};