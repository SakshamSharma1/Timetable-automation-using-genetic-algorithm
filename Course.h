#include <string>

using namespace std;


class Course
{

private:

	int _id;


	string _name;

public:


	Course(int id, const string& name);


	inline int GetId() const { return _id; }


	inline const string& GetName() const { return _name; }

};
