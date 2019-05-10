#include <string>

using namespace std;


class Room
{

private:

	
	static int _nextRoomId;

private:

	
	int _id;

	
	string _name;

	
	bool _lab;

	
	int _numberOfSeats;

public:

	
	Room(const string& name, bool lab, int numberOfSeats);

	
	inline int GetId() const { return _id; }

	
	inline const string& GetName() const { return _name; }

	
	inline bool IsLab() const { return _lab; }

	
	inline int GetNumberOfSeats() const { return _numberOfSeats; }

	
	static inline void RestartIDs() { _nextRoomId = 0; }

};
