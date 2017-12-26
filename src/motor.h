#ifndef __MOTOR_H__
#define __MOTOR_H__

void motor_init();
void motor_on(int id, int msecs);
void motor_off(int id);
void motor_loop();

#endif