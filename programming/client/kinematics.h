#ifndef KINEMATICS_H_
#define KINEMATICS_H_
/*	Define motion struct	*/
#include "project.h"

void define_classic_parameters(move* move_ptr);
void fk_classic(move* move_ptr, info* info_ptr);
void fk_mod(move* move_ptr);
void ik_RRR_arm(move* move_ptr, char* plane);

int inverse_kinematics(move* move_ptr, info* info_ptr, float* position, double* angles);


void control_kinematics(info* info_ptr, move* move_ptr, float x, float y, float z);

void control_kinematics_v2(info* info_ptr, move* move_ptr, float x, float y, float z);

void inverse_kinematics_mode_toggle(char* inputCommand);

#endif // !KINEMATICS.H