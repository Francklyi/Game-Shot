#include "RoleSet.h"

RoleSet::RoleSet(){
	vRole=vector<Role*>(0);
}


void RoleSet::addRole(Role* pR){
	vRole.push_back(pR);
}
