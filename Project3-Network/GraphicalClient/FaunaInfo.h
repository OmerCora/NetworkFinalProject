#ifndef _FLORAINFO_HG_
#define _FLORAINFO_HG_

#include <string>
#include <vector>

class FaunaInfo
{
public:
	FaunaInfo();
	~FaunaInfo();

	struct FoodPreference
	{
		FoodPreference(std::string a, int b) :name(a), priority(b) {}
		bool FaunaInfo::FoodPreference::operator<(const FoodPreference& right) const;
		std::string name;
		int priority; //lower is more important
	};

	std::string type;
	float initialCalories;
	float currentCalories;
	float reproductionDuration;
	std::vector<FoodPreference> foodPreference;
	bool dead;
	float acc_reproduce;
private:

};


#endif // !_FLORAINFO_HG_
