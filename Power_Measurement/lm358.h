#ifndef LM358_H
#define LM358_H

extern void LM358_Current_Init(void);
extern void LM358_Voltage_Init(void);
extern bool get_Flag_Status(void);
extern void reset_Flag_Status(void);
extern void get_Voltage_Timer_Value(int32_t* V);
extern void get_Current_Timer_Value(int32_t* C);

#endif /* LM358_H */
