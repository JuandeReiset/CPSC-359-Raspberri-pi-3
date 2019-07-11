// This program demonstrates how to initialize a frame buffer for a
// 1024 x 768 display, and how to draw on it using a simple checker board
// pattern.

// Included header files
#include "uart.h"
#include "framebuffer.h"
#include "gpio.h"
#include "systimer.h"

#define BUTTON_B         (1 << 0)
#define BUTTON_Y         (1 << 1)
#define BUTTON_SEL       (1 << 2)
#define BUTTON_START     (1 << 3)
#define BUTTON_UP        (1 << 4)
#define BUTTON_DOWN      (1 << 5)
#define BUTTON_LEFT      (1 << 6)
#define BUTTON_RIGHT     (1 << 7)
#define BUTTON_A         (1 << 8)
#define BUTTON_X         (1 << 9)
#define BUTTON_L         (1 << 10)
#define BUTTON_R         (1 << 11)
// Function prototypes
unsigned short get_SNES();
void init_GPIO9_to_output();
void set_GPIO9();
void clear_GPIO9();
void init_GPIO11_to_output();
void set_GPIO11();
void clear_GPIO11();
void init_GPIO10_to_input();
unsigned int get_GPIO10();

//Wall disolving colors
#define BLACK     0x00000000
#define GRAY1     0x37373700
#define GRAY2     0X69696900
#define GRAY3     0X9B9B9B00
#define GRAY4     0xCDCDCD00
#define WHITE     0x00FFFFFF


#define RED       0x00FF0000
#define LIME      0x0000FF00
#define BLUE      0x000000FF
#define AQUA      0x0000FFFF
#define FUCHSIA   0x00FF00FF
#define YELLOW    0x00FFFF00
#define GRAY      0x00808080
#define MAROON    0x00800000
#define OLIVE     0x00808000
#define GREEN     0x00008000
#define TEAL      0x00008080
#define NAVY      0x00000080
#define PURPLE    0x00800080
#define SILVER    0x00C0C0C0

#define PALE_RED  0xFF7F7F


void drawSquare(int rowStart, int columStart, int squaresize, unsigned int color);

int playerX = 0, playerY=2;

int start=0;
unsigned int lastmove= BUTTON_START;
int checkcollision(int x, int y);


int maze[12][16] = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
  {2, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1},
  {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
  {1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1},
  {1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1},
  {1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
  {1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1},
  {1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 3},
  {1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1},
  {1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

int originalmaze[12][16];

void savestateMaze(){
  for(unsigned int x = 0; x <16; x++){
    for(unsigned int y = 0; y < 12; y++){
      originalmaze[y][x]= maze[y][x];
    }
  }
}

void restoreMaze(){
  for(unsigned int x = 0; x <16; x++){
    for(unsigned int y = 0; y < 12; y++){
      maze[y][x]= originalmaze[y][x];
    }
  }
}

void refreshSquare(int x, int y){

      if(maze[y][x] == 0 ){
        //draw White square
        drawSquare(y*64, x*64, 64, WHITE);
      }
      else if(maze[y][x]==1){
        drawSquare(y*64, x*64, 64, BLACK);
      }
      else if(maze[y][x] == 2 ){
        drawSquare(y*64, x*64, 64, BLUE);
      }
      else if(maze[y][x]==3){
        drawSquare(y*64, x*64, 64, LIME);
      }

      else if(maze[y][x]==4){
        drawSquare(y*64, x*64, 64, PALE_RED);
      }

      else if(maze[y][x]==9){
          drawSquare(y*64, x*64, 64, GRAY1);
      }
      else if(maze[y][x]==8){
          drawSquare(y*64, x*64, 64, GRAY2);
      }
      else if(maze[y][x]==7){
          drawSquare(y*64, x*64, 64, GRAY3);
      }
      else if(maze[y][x]==6){
          drawSquare(y*64, x*64, 64, GRAY4);
      }

}





void drawMaze(){
  for(unsigned int x = 0; x <16; x++){
    for(unsigned int y = 0; y < 12; y++){
      refreshSquare(x,y);
    }
  }
}

void drawPlayer(unsigned int color){
  drawSquare(playerY*64, playerX*64, 64, color);
}

void drawPlayerstart(){
  drawSquare(2*64, 0*64, 64, RED);
}

int checkcollision(int x, int y){
  if(maze[y][x]==1 || maze[y][x]==9 ||maze[y][x]==8 || maze[y][x]==7 || maze[y][x]==6){
    return 1;
  }
  else{
    return 0;
  }

}

void useacid(int x, int y){

  if(maze[y][x]==1){
    maze[y][x]=9;
    }

  else if(maze[y][x]==9){
      maze[y][x]=8;
      }
    else if(maze[y][x]==8){
        maze[y][x]=7;
        }
      else  if(maze[y][x]==7){
          maze[y][x]=6;
          }

        else  if(maze[y][x]==6){
            maze[y][x]=0;
            }

}

void visitTile(int currentX, int currentY, int nextX, int nextY){
  if(maze[nextY][nextX]==0){
    maze[nextY][nextX]=4;
    maze[currentY][currentX]=4;
    }

  else if(maze[nextY][nextX]==4){

    maze[currentY][currentX]=0;
    }

}

int hewin(int x, int y){
  if(maze[y][x]==3){
    return 1;
  }
  else{
    return 0;
  }
}
////////////////////////////////////////////////////////////////////////////////
//
//  Function:       main
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function initializes the UART terminal and initializes
//                  a frame buffer for a 1024 x 768 display. Each pixel in the
//                  frame buffer is 32 bits in size, which encodes an RGB value
//                  (plus an 8-bit alpha channel that is not used). The program
//                  then draws and displays an 18 x 12 checker board pattern.
//
////////////////////////////////////////////////////////////////////////////////

void main()
{
    //char c;
    unsigned short data, currentState = 0xFFFF;

    // Initialize the UART terminal
    uart_init();

    // Set up GPIO pin #9 for output (LATCH output)
    init_GPIO9_to_output();

    // Set up GPIO pin #11 for output (CLOCK output)
    init_GPIO11_to_output();

    // Set up GPIO pin #10 for input (DATA input)
    init_GPIO10_to_input();

    // Clear the LATCH line (GPIO 9) to low
    clear_GPIO9();

    // Set CLOCK line (GPIO 11) to high
    set_GPIO11();


    // Initialize the frame buffer
    initFrameBuffer();

    savestateMaze();
    drawMaze();

    // Draw on the frame buffer and display it
    //displayFrameBuffer();

    // Loop forever, echoing characters received from the console
    // on a separate line with : : around the character
    while (1) {
        // Wait for a character input from the console
        data= get_SNES();
        if(data != currentState){

          //Process button inputs

          if((data & BUTTON_START)==BUTTON_START){
              refreshSquare(playerX, playerY);
              restoreMaze();
              drawMaze();
              playerX=0;
              playerY=2;
              drawPlayerstart();

              start=1;

          }

            if(start ==1){

              if((data & BUTTON_LEFT)== BUTTON_LEFT && (data & BUTTON_X)== BUTTON_X){
                if(playerX>0){

                    useacid(playerX-1, playerY);
                    refreshSquare(playerX-1, playerY);
                  }

              }

              if((data & BUTTON_RIGHT)== BUTTON_RIGHT && (data & BUTTON_X)== BUTTON_X){
                if(playerX<15){

                    useacid(playerX+1, playerY);
                  refreshSquare(playerX+1, playerY);
                  }

              }

              if((data & BUTTON_UP)== BUTTON_UP && (data & BUTTON_X)== BUTTON_X){
                if(playerX>0){

                    useacid(playerX, playerY-1);
                    refreshSquare(playerX, playerY-1);
                  }

              }

              if((data & BUTTON_DOWN)== BUTTON_DOWN && (data & BUTTON_X)== BUTTON_X){
                if(playerY<11){

                    useacid(playerX, playerY+1);
                    refreshSquare(playerX, playerY+1);
                  }

              }


          if((data & BUTTON_LEFT)== BUTTON_LEFT){
            if(playerX>0){

              if(checkcollision(playerX-1, playerY)==0){
              //visitSquare(playerX,playerY);

              visitTile(playerX, playerY, playerX-1, playerY);

            //  refreshSquare(playerX, playerY);
              playerX -= 1;
              refreshSquare(playerX+1, playerY);
              refreshSquare(playerX, playerY);
              drawPlayer(RED);

              }
            }
          }

          if((data & BUTTON_RIGHT)== BUTTON_RIGHT){
            if(playerX<15){
              if(checkcollision(playerX+1, playerY)==0){
            //  visitSquare(playerX,playerY);

            visitTile(playerX, playerY, playerX+1, playerY);
              //refreshSquare(playerX, playerY);
              playerX += 1;

              refreshSquare(playerX-1, playerY);
              refreshSquare(playerX, playerY);
              drawPlayer(RED);

              }
            }
          }

          if((data & BUTTON_UP)== BUTTON_UP){
            if(playerY>0){
              if(checkcollision(playerX, playerY-1)==0){
              //visitSquare(playerX,playerY);

              visitTile(playerX, playerY, playerX, playerY-1);

              //refreshSquare(playerX, playerY);
              playerY -= 1;

              refreshSquare(playerX, playerY+1);
              refreshSquare(playerX, playerY);
              drawPlayer(RED);

              }
            }
          }

          if((data & BUTTON_DOWN)== BUTTON_DOWN){
            if(playerY<11){
              if(checkcollision(playerX, playerY+1)==0){
              //visitSquare(playerX,playerY);
              visitTile(playerX, playerY, playerX, playerY+1);



              //refreshSquare(playerX, playerY);
              playerY += 1;
              refreshSquare(playerX, playerY-1);
              refreshSquare(playerX, playerY);
              drawPlayer(RED);

              }
            }
          }
            if(hewin(playerX, playerY)){
              drawPlayer(GREEN);
              start=0;
            }

          }
          currentState =data;
        }
	        microsecond_delay(16667);
    }
}

unsigned short get_SNES()
{
    int i;
    unsigned short data = 0;
    unsigned int value;


    // Set LATCH to high for 12 microseconds. This causes the controller to
    // latch the values of button presses into its internal register. The
    // first serial bit also becomes available on the DATA line.
    set_GPIO9();
    microsecond_delay(12);
    clear_GPIO9();

    // Output 16 clock pulses, and read 16 bits of serial data
    for (i = 0; i < 16; i++) {
	// Delay 6 microseconds (half a cycle)
	microsecond_delay(6);

	// Clear the CLOCK line (creates a falling edge)
	clear_GPIO11();

	// Read the value on the input DATA line
	value = get_GPIO10();

	// Store the bit read. Note we convert a 0 (which indicates a button
	// press) to a 1 in the returned 16-bit integer. Unpressed buttons
	// will be encoded as a 0.
	if (value == 0) {
	    data |= (0x1 << i);
	}

	// Delay 6 microseconds (half a cycle)
	microsecond_delay(6);

	// Set the CLOCK to 1 (creates a rising edge). This causes the
	// controller to output the next bit, which we read half a
	// cycle later.
	set_GPIO11();
    }

    // Return the encoded data
    return data;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       init_GPIO9_to_output
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets GPIO pin 9 to an output pin without
//                  any pull-up or pull-down resistors.
//
////////////////////////////////////////////////////////////////////////////////

void init_GPIO9_to_output()
{
    register unsigned int r;


    // Get the current contents of the GPIO Function Select Register 0
    r = *GPFSEL0;

    // Clear bits 27 - 29. This is the field FSEL9, which maps to GPIO pin 9.
    // We clear the bits by ANDing with a 000 bit pattern in the field.
    r &= ~(0x7 << 27);

    // Set the field FSEL9 to 001, which sets pin 9 to an output pin.
    // We do so by ORing the bit pattern 001 into the field.
    r |= (0x1 << 27);

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 0
    *GPFSEL0 = r;

    // Disable the pull-up/pull-down control line for GPIO pin 9. We follow the
    // procedure outlined on page 101 of the BCM2837 ARM Peripherals manual. The
    // internal pull-up and pull-down resistor isn't needed for an output pin.

    // Disable pull-up/pull-down by setting bits 0:1
    // to 00 in the GPIO Pull-Up/Down Register
    *GPPUD = 0x0;

    // Wait 150 cycles to provide the required set-up time
    // for the control signal
    r = 150;
    while (r--) {
	asm volatile("nop");
    }

    // Write to the GPIO Pull-Up/Down Clock Register 0, using a 1 on bit 9 to
    // clock in the control signal for GPIO pin 9. Note that all other pins
    // will retain their previous state.
    *GPPUDCLK0 = (0x1 << 9);

    // Wait 150 cycles to provide the required hold time
    // for the control signal
    r = 150;
    while (r--) {
        asm volatile("nop");
    }

    // Clear all bits in the GPIO Pull-Up/Down Clock Register 0
    // in order to remove the clock
    *GPPUDCLK0 = 0;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       set_GPIO9
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets the GPIO output pin 9
//                  to a 1 (high) level.
//
////////////////////////////////////////////////////////////////////////////////

void set_GPIO9()
{
    register unsigned int r;

    // Put a 1 into the SET9 field of the GPIO Pin Output Set Register 0
    r = (0x1 << 9);
    *GPSET0 = r;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       clear_GPIO9
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function clears the GPIO output pin 9
//                  to a 0 (low) level.
//
////////////////////////////////////////////////////////////////////////////////

void clear_GPIO9()
{
    register unsigned int r;

    // Put a 1 into the CLR9 field of the GPIO Pin Output Clear Register 0
    r = (0x1 << 9);
    *GPCLR0 = r;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       init_GPIO11_to_output
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets GPIO pin 11 to an output pin without
//                  any pull-up or pull-down resistors.
//
////////////////////////////////////////////////////////////////////////////////

void init_GPIO11_to_output()
{
    register unsigned int r;


    // Get the current contents of the GPIO Function Select Register 1
    r = *GPFSEL1;

    // Clear bits 3 - 5. This is the field FSEL11, which maps to GPIO pin 11.
    // We clear the bits by ANDing with a 000 bit pattern in the field.
    r &= ~(0x7 << 3);

    // Set the field FSEL11 to 001, which sets pin 9 to an output pin.
    // We do so by ORing the bit pattern 001 into the field.
    r |= (0x1 << 3);

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 1
    *GPFSEL1 = r;

    // Disable the pull-up/pull-down control line for GPIO pin 11. We follow the
    // procedure outlined on page 101 of the BCM2837 ARM Peripherals manual. The
    // internal pull-up and pull-down resistor isn't needed for an output pin.

    // Disable pull-up/pull-down by setting bits 0:1
    // to 00 in the GPIO Pull-Up/Down Register
    *GPPUD = 0x0;

    // Wait 150 cycles to provide the required set-up time
    // for the control signal
    r = 150;
    while (r--) {
	asm volatile("nop");
    }

    // Write to the GPIO Pull-Up/Down Clock Register 0, using a 1 on bit 11 to
    // clock in the control signal for GPIO pin 11. Note that all other pins
    // will retain their previous state.
    *GPPUDCLK0 = (0x1 << 11);

    // Wait 150 cycles to provide the required hold time
    // for the control signal
    r = 150;
    while (r--) {
        asm volatile("nop");
    }

    // Clear all bits in the GPIO Pull-Up/Down Clock Register 0
    // in order to remove the clock
    *GPPUDCLK0 = 0;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       set_GPIO11
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets the GPIO output pin 11
//                  to a 1 (high) level.
//
////////////////////////////////////////////////////////////////////////////////

void set_GPIO11()
{
    register unsigned int r;

    // Put a 1 into the SET11 field of the GPIO Pin Output Set Register 0
    r = (0x1 << 11);
    *GPSET0 = r;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       clear_GPIO11
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function clears the GPIO output pin 11
//                  to a 0 (low) level.
//
////////////////////////////////////////////////////////////////////////////////

void clear_GPIO11()
{
    register unsigned int r;

    // Put a 1 into the CLR11 field of the GPIO Pin Output Clear Register 0
    r = (0x1 << 11);
    *GPCLR0 = r;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       init_GPIO10_to_input
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets GPIO pin 10 to an input pin without
//                  any internal pull-up or pull-down resistors. Note that
//                  a pull-down (or pull-up) resistor must be used externally
//                  on the bread board circuit connected to the pin. Be sure
//                  that the pin high level is 3.3V (definitely NOT 5V).
//
////////////////////////////////////////////////////////////////////////////////

void init_GPIO10_to_input()
{
    register unsigned int r;


    // Get the current contents of the GPIO Function Select Register 1
    r = *GPFSEL1;

    // Clear bits 0 - 2. This is the field FSEL10, which maps to GPIO pin 10.
    // We clear the bits by ANDing with a 000 bit pattern in the field. This
    // sets the pin to be an input pin.
    r &= ~(0x7 << 0);

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 1
    *GPFSEL1 = r;

    // Disable the pull-up/pull-down control line for GPIO pin 10. We follow the
    // procedure outlined on page 101 of the BCM2837 ARM Peripherals manual. We
    // will pull down the pin using an external resistor connected to ground.

    // Disable internal pull-up/pull-down by setting bits 0:1
    // to 00 in the GPIO Pull-Up/Down Register
    *GPPUD = 0x0;

    // Wait 150 cycles to provide the required set-up time
    // for the control signal
    r = 150;
    while (r--) {
        asm volatile("nop");
    }

    // Write to the GPIO Pull-Up/Down Clock Register 0, using a 1 on bit 10 to
    // clock in the control signal for GPIO pin 10. Note that all other pins
    // will retain their previous state.
    *GPPUDCLK0 = (0x1 << 10);

    // Wait 150 cycles to provide the required hold time
    // for the control signal
    r = 150;
    while (r--) {
        asm volatile("nop");
    }

    // Clear all bits in the GPIO Pull-Up/Down Clock Register 0
    // in order to remove the clock
    *GPPUDCLK0 = 0;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       get_GPIO10
//
//  Arguments:      none
//
//  Returns:        1 if the pin level is high, and 0 if the pin level is low.
//
//  Description:    This function gets the current value of pin 10.
//
////////////////////////////////////////////////////////////////////////////////

unsigned int get_GPIO10()
{
    register unsigned int r;


    // Get the current contents of the GPIO Pin Level Register 0
    r = *GPLEV0;

    // Isolate pin 10, and return its value (a 0 if low, or a 1 if high)
    return ((r >> 10) & 0x1);
}
