#include <avr/io.h>

#define DRVSELPIN 4
#define DRVRDYPIN 3
#define CTRLRDYPIN 2
#define RESETSTEPPIN 1
#define DSKCHGPIN 0

#define diWrite(pin,value) \
          if (value == 0) { PORTB &= ~(1 << pin); } else { PORTB |= (1 << pin); }

#define diRead(pin) \
          ((PINB & (1 << pin)) != 0)

void send_step() {
  digitalWrite(RESETSTEPPIN, HIGH);
  delay(1);
  digitalWrite(RESETSTEPPIN, LOW);
}

void do_stepping()
{
    int i;

    for (i=0; i < 80; i++)
    {
        send_step();
        delay(2);
    }
}

void setup() {
// Set up the initial pin I/O states.
  pinMode(DRVSELPIN, OUTPUT);
  pinMode(DRVRDYPIN, INPUT);
  pinMode(CTRLRDYPIN, OUTPUT);
  pinMode(RESETSTEPPIN, OUTPUT);

// Disable the pull-up on the Drive READY signal line.

  digitalWrite(DRVRDYPIN, LOW);
  
// For the time being disable our STEP pin output.
  digitalWrite(RESETSTEPPIN, LOW);

// Initially pull Drive 1 select line low to enable the drive and then send 80 STEP pulses so as to try to make sure that the drive is at track zero so that the ROM will detect it.

  digitalWrite(DRVSELPIN, LOW);

  do_stepping();

  pinMode(DRVSELPIN, INPUT);
  digitalWrite(DRVSELPIN, HIGH);
}

void loop() {
  unsigned int rdyval, oldval = 0;
  
// Echo the input from the READY line from the floppy drive to the controller READY line.
  while(1)
  {
//      rdyval = digitalRead(DRVRDYPIN);
      rdyval = diRead(PORTB3);

      if (oldval != rdyval)
      {
//         digitalWrite(CTRLRDYPIN, rdyval);
         diWrite(PIN_PB2, rdyval);
         oldval = rdyval;
      }
   }
}
