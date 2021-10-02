#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>

std::string rgb2hex(unsigned int r, unsigned int g, unsigned int b)
{
  //convertes the rgb values to hex values
  std::stringstream ss;
  ss << std::hex << (r << 16 | g << 8 | b );

  std::string hex_value = ss.str();

  //corrects the hex value if it does not have 6 chars
  if(hex_value.length() < 6)
  {
    int number_of_missing_0s = 6 - hex_value.length();
    std::string missing_hex_values = "";

    //add the necessary number of 0s to the hex value to make it a valid one
    for(int i = 0; i < number_of_missing_0s; i++)
    {
      missing_hex_values = missing_hex_values.append("0");
    }

    //puts together both strings (the string with the needed hex values and the one that comes out of "std::hex")
    hex_value = missing_hex_values.append(hex_value);
  }

  //returns the rgb hex value
  return hex_value;
}

void setColor(unsigned int zones, unsigned int red, unsigned int green, unsigned int blue)
{
  //sets the color to the keyboard
  std::string color = rgb2hex(red, green, blue);
  for (unsigned int z = 0; z < zones; z++)
  {
    //writes new rgb value (in hex) to the files
    std::ofstream rgb_zone_file("/sys/devices/platform/hp-wmi/rgb_zones/zone0" + std::to_string(z));
    rgb_zone_file << color;
    rgb_zone_file.close();
  }
}

int main() {

  //defines how many zones the keyboard has, in my case its 4
  const unsigned int ZONES = 4;

  //declare an array of 8-bit integers to hold the color values
  unsigned int rgb[3];

  while (true)
  {
    //declares the increment color index variable
    unsigned int iC;

    rgb[0] = 255; //rgb[0] == red
    rgb[1] = 0;   //rgb[1] == green
    rgb[2] = 0;   //rgb[2] == blue

    //loop through each combination of colors
    //RED   /  GREEN    (dC = 0, iC = 1)
    //GREEN /  BLUE     (dC = 1, iC = 2)
    //BLUE  /  RED      (dC = 2, iC = 0)
    for ( unsigned int dC = 0; dC < 3; dC++ ) {

      //makes the incrementing color (iC) be one more than
      //the decrementing color (dC) or, if the iC is 2 (blue),
      //then the dC gets back to 0 (red).
      if (dC == 2) {
        iC = 0;
      }
      else
      {
        iC = dC + 1;
      }

      //increments the "incrementing color" and decrements the "decrementing color"
      for ( unsigned int i = 0; i < 255; i++ ) {

        rgb[dC]--;
        rgb[iC]++;

        //sets the color to the keyboard
        setColor(ZONES, rgb[0], rgb[1], rgb[2]);

        //slows down the color cycle
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
      }
    }
  }

  return 0;
}
