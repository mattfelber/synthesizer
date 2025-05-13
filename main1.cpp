#include <iostream>
#include <vector>
#include <string>

using namespace std;

#include "olcNoiseMaker.h"

double dFrequencyOutput = 0.0;

double MakeNoise(double dTime)
{
    double dOutput = 1.0 *sin(dFrequencyOutput * 2 *3.14159 * dTime);

    if (dOutput > 0.0)
        return 0.2;
    
    else
        return -0.2;

    //amplitute times and frequency in hertz which is A on the keyboard
    //return 0.3 * sin(320.0 * 2 * 3.14159 * dTime);
}

int main() 
{
    // No locale initialization needed
    // locale::global(locale("C"));
    
    cout << "oneloader tutorial - synthesizer part 1" << endl;

    // gets all sound hardware
    vector<string> devices = olcNoiseMaker<short>::Enumerate();

    // Check if any devices were found
    if (devices.empty()) {
        cout << "No audio devices found!" << endl;
        return 1;
    }

    // displays findings 
    for (auto d : devices) cout << "Found Output Device: " << d << endl;

    // creates sound machine
    olcNoiseMaker<short> sound(devices[0], 44100, 1, 8, 512);

    // links the noise function with sound machine class
    sound.SetUserFunction(MakeNoise);

    double dOctaveBaseFrequency = 110.0;
    double d12thRootOf2 = pow(2.0, 1.0 / 12.0);

    while (1)
    {


        //keyboard piano style

        bool bKeyPressed =false;
        for (int k = 0; k < 15; k++)
        {
            if (GetAsyncKeyState((unsigned char)("ZSXCFVGBNJMK\xbcL\xbe"[k])) & 0x8000)
            {
                dFrequencyOutput = dOctaveBaseFrequency * pow(d12thRootOf2, k);
                bKeyPressed = true;
            }
        }

        if (!bKeyPressed)
        {
            dFrequencyOutput = 0.0;
        }

        //Notes to play
       /* if (GetAsyncKeyState('A') & 0x8000)
        {
            dFrequencyOutput = dOctaveBaseFrequency * pow(d12thRootOf2, 0); // the 0 you can change to 1,2,3..12 and it goes up a note with each increment. 
        }
        else
        {
            dFrequencyOutput = 0.0;
        }
        */
    }



    return 0;
}