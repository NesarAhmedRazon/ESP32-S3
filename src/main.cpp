
#include <SparkFun_TB6612.h>
#include <Arduino.h>
// Pins for all inputs, keep in mind the PWM defines must be on PWM pins
// the default pins listed are the ones used on the Redbot (ROB-12097) with
// the exception of STBY which the Redbot controls with a physical switch
#define AIN1 7
#define AIN2 44
#define PWMA 43
#define STBY 9
#define RED 1
#define GREEN 3
#define BLUE 5

// these constants are used to allow you to make your motor configuration
// line up with function names like forward.  Value can be 1 or -1
const int offsetA = 1;
const int offsetB = 1;
int hue = 0; // Hue value for the rainbow effect

// Initializing motors.  The library will allow you to initialize as many
// motors as you have memory for.  If you are using functions like forward
// that take 2 motors as arguements you can either write new functions or
// call the function more than once.
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
int calculateRed(int hue)
{
    // Calculate the red component based on the hue
    return map(hue, 0, 359, 255, 0);
}

int calculateGreen(int hue)
{
    // Calculate the green component based on the hue
    return map(hue, 0, 359, 0, 255);
}

int calculateBlue(int hue)
{
    // Calculate the blue component based on the hue
    return map(hue, 0, 359, 0, 255);
}

const int delayTime = 10; // Delay time between color changes
void setup()
{
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT); // Nothing here
}

void loop()
{
    // Use of the drive function which takes as arguements the speed
    // and optional duration.  A negative speed will cause it to go
    // backwards.  Speed can be from -255 to 255.  Also use of the
    // brake function which takes no arguements.
    motor1.drive(255, 1000);
    motor1.drive(-100, 1000);
    motor1.brake();
    delay(delayTime);
    // Calculate the RGB values based on the hue
    int r = calculateRed(hue);
    int g = calculateGreen(hue);
    int b = calculateBlue(hue);

    // Set the RGB colors using analogWrite
    analogWrite(RED, r);
    analogWrite(GREEN, g);
    analogWrite(BLUE, b);

    // Increment the hue for the next color
    hue += 1;
    if (hue > 359)
    {
        hue = 0;
    }

    delay(delayTime);
}