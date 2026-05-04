//------------------------------------------------------------------------------
//
//  Description: This is the master list of all my function prototypes.
//               I use this so the compiler knows what functions exist
//               across all my different .c files.
//
//  Ezana
//  Feb 2026
//  Built with Code Composer Studio
//------------------------------------------------------------------------------

// Main Operating System
void main(void);

// System Initialization
void Init_Conditions(void);
void Init_Clocks(void);
void Init_LEDs(void);

// Hardware Interrupts
void enable_interrupts(void);
__interrupt void Timer0_B0_ISR(void);
__interrupt void switch_interrupt(void);

// Display and LCD
void Display_Process(void);
void Display_Update(char p_L1, char p_L2, char p_L3, char p_L4);
void Init_LCD(void);
void lcd_clear(void);
void lcd_putc(char c);
void lcd_puts(char *s);
void lcd_BIG_mid(void);
void lcd_BIG_bot(void);

// Port Configurations
void Init_Ports(void);
void Init_Port1(void);
void Init_Port2(void);
void Init_Port3(void);
void Init_Port4(void);
void Init_Port5(void);
void Init_Port6(void);

// Timer Setup
void Init_Timers(void);
void Init_Timer_B0(void);
void Init_Timer_B1(void);
void Init_Timer_B3(void);

// Switch Processing
void Switches_Process(void);

// Motor and Wheel Control
void Wheels_All_Off(void);
void Forward_On(void);
void Forward_Off(void);
void Reverse_On(void);
void Reverse_Off(void);
void Spin_CW_On(void);
void Spin_CCW_On(void);

// My Project 5 Logic (Lives in project5.c)
void Run_Project5(void);

// Add this under your other function prototypes:
void Init_ADC(void);
void HEXtoBCD(int hex_value, char* bcd_string);
void Run_Project7(void);

void Init_DAC(void);

void Init_Serial_UCA0(void);
void Init_Serial_UCA1(void);
void Change_Baud_115200(void);
void Change_Baud_57600(void);
void Transmit_UCA0_String(char* str);



