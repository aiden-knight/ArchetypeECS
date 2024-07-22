#include <iostream>
#include <string>
#include <vector>
#include <tuple>
using namespace std;

template<typename Function>
void Each(int* arr, int arraySize, Function&& lambda)
{
	for (int i = 0; i < arraySize; i++)
	{
		lambda(arr[i]);
	}
}

template<typename... Component>
class System
{
public:
	vector<tuple<Component...>> vectors;

	void Each(void (*lambda)(Component&...))
	{
		int count = vectors.size();
		for (int i = 0; i < count; i++)
		{
			lambda(get<Component>(vectors[i])...);
		}
		
	}
};

int main()
{
	auto test = System<int, float>();
	test.vectors.push_back({4, 0.5f});
	test.vectors.push_back({3, 0.3333f});
	test.Each([](int& sInt, float& sFloat) {
		cout << sInt / sFloat << endl;
		});
}