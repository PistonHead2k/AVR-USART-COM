//Personal Standard Library for PistonHead2k

#ifndef std
#define std



/* Electrical Constants */

#define HIGH 1
#define LOW 0

/* Logic Constants */

#define true 1
#define false 0

/* I/O Constants */

#define OUTPUT 1
#define INPUT 0
/* Enable Pullup */
#define PULLUP 1
/* Disable Pullup */
#define FLOATING 0

/* Control Register Constants */

#define ENABLE 1
#define DISABLE 0

/* External Interrupt Flag Register Constants */ //#54

/* The low level of INT0 generates an interrupt request */
#define INT0LOWLEVEL  (0 << ISC01 | 0 << ISC00)
/* Any logical change on INT0 generates an interrupt request */
#define INT0ANYCHANGE (0 << ISC01 | 1 << ISC00)
/* The falling edge of INT0 generates an interrupt request */
#define INT0FALLING   (1 << ISC01 | 0 << ISC00)
/* The rising edge of INT0 generates an interrupt request */
#define INT0RISING    (1 << ISC01 | 1 << ISC00)

/* The low level of INT1 generates an interrupt request */
#define INT1LOWLEVEL  (0 << ISC11 | 0 << ISC10)
/* Any logical change on INT1 generates an interrupt request */
#define INT1ANYCHANGE (0 << ISC11 | 1 << ISC10)
/* The falling edge of INT1 generates an interrupt request */
#define INT1FALLING   (1 << ISC11 | 0 << ISC10)
/* The rising edge of INT1 generates an interrupt request */
#define INT1RISING    (1 << ISC11 | 1 << ISC10)

#endif