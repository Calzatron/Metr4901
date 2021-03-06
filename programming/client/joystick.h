/*
*	METR4901		2017-2018		UQ
*	Callum Rohweder
*	joystick.h
*
*	Joystick header file
*/


#ifndef JOYSTICK_H_
#define JOYSTICK_H_


/*	external functions	*/
extern void initialise_joystick_mutex(void);
void interpret_joystick(char* buffer);
void CreateChildProcess(void);
int joystick_input_available(void);
void joystick_clear_input_buffer(void);
void joystick_get_char(info* info_ptr);
void add_to_buffer(void);
uint8_t joystickEnabled(void);



#endif // !JOYSTICK.H
