#include <FED3.h>  //Include the FED3 library

String sketch = "FR1 w/ Licks";  //Unique identifier text for each sketch
FED3 fed3(sketch);               //Start the FED3 object

void setup() {
  fed3.begin();  //Setup the FED3 hardware
}

void loop() {
  fed3.run();  //Call fed.run at least once per loop

  if (fed3.Left) {               //If left poke is triggered
    fed3.logLeftPoke();          //Log left poke
    fed3.ConditionedStimulus();  //Deliver conditioned stimulus (tone and lights for 200ms)
    fed3.Feed();                 //Deliver pellet
  }

  if (fed3.Right) {       //If right poke is triggered
    fed3.logRightPoke();  //Log right poke
  }

  if (fed3.touchSensor = true) {
    fed3.touchsensitive();
  }

  if ((fed3.millisSinceMidnight >= 27000000) & (fed3.millisSinceMidnight <= 70200000)) {  //turn off the houselights of 7:30 am and pack on at 7:30 pm
    fed3.houselights(false);
  }
  else {
    fed3.houselights(true);
  }
}