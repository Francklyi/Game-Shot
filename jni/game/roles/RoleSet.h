#ifndef _ROLE_SET_
#define _ROLE_SET_
#include "Role.h"

#include <vector>
using std::vector;

class RoleSet{
	vector<Role*> vRole;


public:
	RoleSet();
	void addRole(Role*);

};

#endif //_ROLE_SET_
