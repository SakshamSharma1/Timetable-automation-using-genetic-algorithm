#include "Room.h"

int Room::_nextRoomId = 0;


Room::Room(const string& name, bool lab, int numberOfSeats) : _id(_nextRoomId++),
																	  _name(name),
																	  _lab(lab),
																	  _numberOfSeats(numberOfSeats) { }
