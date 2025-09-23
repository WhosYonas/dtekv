/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */


/* Below functions are external and found in other files. */
extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
extern void time2string(char*,int);
extern void tick(int*);
extern void delay(int);
extern int nextprime( int );

int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";


/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) 
{}

/* Add your code here for initializing interrupts. */
void labinit(void)
{}


int get_btn(void) {
    volatile int* btn = (int*)0x040000d0;
    return *btn & 0b1;
}

int get_sw(void) {
    volatile int* sw = (int*)0x04000010;
    return *sw & 0x3FF;
}

void set_leds(int led_mask) {
  volatile int* leds = ( int*)0x04000000;
  *leds = led_mask & 0x3FF;
}

void set_display(int display_number, int value) {
  volatile int* display = (int*)0x04000050;
  display = display + (display_number - 1) * (0x10 / sizeof(int));
  
  switch (value)
{
    case 0: *display = 0b11000000; break;
    case 1: *display = 0b11111001; break;
    case 2: *display = 0b10100100; break;
    case 3: *display = 0b10110000; break; 
    case 4: *display = 0b10011001; break;
    case 5: *display = 0b10010010; break;
    case 6: *display = 0b10000010; break;
    case 7: *display = 0b11111000; break;
    case 8: *display = 0b10000000; break;
    case 9: *display = 0b10010000; break;



  
  default: print("Invalid value for 7-segment display\n");
    break;
  }
}


void display_hours() {
  /*For seconds*/
  int second_tens = 0;
  int second_ones = 0;
  
  /*For minutes*/
  int minute_tens = 0;
  int minute_ones = 0;
 
  /*For hours*/
  int hour_tens = 0;
  int hour_ones = 0;


   while (1) {
    time2string( textstring, mytime ); // Converts mytime to string
    display_string( textstring ); //Print out the string 'textstring'
   
   if(get_btn() == 1) {
      int switch_state = get_sw() & 0b1100000000;
      int switch_modifier = get_sw() & 0b0000111111;

      switch (switch_state)
  {
      case 0b1100000000:
        hour_ones = switch_modifier % 10;
        hour_tens = switch_modifier / 10;
        break;

      case 0b1000000000:
        minute_ones = switch_modifier % 10;
        minute_tens = switch_modifier / 10;
        break;

      case 0b0100000000:
        second_ones = switch_modifier % 10;
        second_tens = switch_modifier / 10;
        break;

      default:
        break;
      }

   } 
      /*For seconds*/
      set_display(1, second_ones);
      set_display(2, second_tens);
      /*For minutes*/
      set_display(3, minute_ones);
      set_display(4, minute_tens);
      /*For hours*/
      set_display(5, hour_ones);
      set_display(6, hour_tens);
   
  /*Increment the times*/
  second_ones = second_ones + 1;
  
  if(second_ones == 10) {
    second_ones = 0;
    second_tens = second_tens + 1;
  } 
  
  if(second_tens == 6) {
    second_tens = 0;
    minute_ones = minute_ones + 1;
  } 
  
  if(minute_ones == 10) {
    minute_ones = 0;
    minute_tens = minute_tens + 1;
  } 
  
  if(minute_tens == 6) {
    minute_tens = 0;
    hour_ones = hour_ones + 1;
  } 
  
  if(hour_ones == 10) {
    hour_ones = 0;
    hour_tens = hour_tens + 1;
  } 
  
  if(hour_tens == 9 && hour_ones == 9) {
    second_ones = 0;
    second_tens = 0;
    minute_ones = 0;
    minute_tens = 0;
    hour_ones = 0;
    hour_tens = 0;
  }

    delay( 1300 );          // Delays 1 sec (adjust this value)
    tick( &mytime );     // Ticks the clock once
  }
}   



/* Your code goes into main as well as any needed functions. */
int main() {
  // Call labinit()
  labinit();
  display_hours();

  
}


