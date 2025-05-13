#include <iostream>
#include <vector>
#include <string>

using namespace std;

#include "olcNoiseMaker.h"

//Convert frequency in hertz to angular velocity 
double w(double dHertz)
{
    return dHertz * 2.0 * PI;
}

double osc(double dHertz, double dTime, int nType)
{
    switch(nType)
    {
        case 0: // Sine wave
            return sin(w(dHertz) * dTime);
        case 1: // Square wave 
            return (sin(w(dHertz) * dTime) > 0.0) ? 1.0 : -1.0;
        case 2: // Sawtooth wave
            return (2.0 * (dHertz * dTime - floor(dHertz * dTime + 0.5)));
        case 3: // Triangle wave
            return (2.0 * fabs(2.0 * (dHertz * dTime - floor(dHertz * dTime + 0.5))) - 1.0);
        case 4: // Ramp wave
            return (2.0 * (dHertz * dTime - floor(dHertz * dTime)));
        case 5: // Pulse wave
            return (sin(w(dHertz) * dTime) > 0.0) ? 1.0 : 0.0;
        case 6: // Noise wave
            return ((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;
        
    }
}


double dFrequencyOutput = 0.0; //

// This function is called by the olcNoiseMaker class to generate sound
// It returns a value between -1.0 and 1.0 which is the amplitude of the sound wave

double MakeNoise(double dTime) 
{
    // dTime is the time in seconds since the last call to this function
    // dFrequencyOutput is the frequency of the sound wave in hertz
    // dOutput is the amplitude of the sound wave

    // The output is a type of wave with a frequency of dFrequencyOutput hertz
    // The amplitude is scaled to be between -1.0 and 1.0
    // The output is multiplied by 0.4 to reduce the volume

    
    //double dOutput = sin(w(dFrequencyOutput) * dTime);
    double dOutput = osc(dFrequencyOutput, dTime, 4); // select the wave
    return dOutput * 0.4; // MASTER VOLUME
    
    //square waves:
    //if (dOutput > 0.0)
    //    return 0.2;
    
    //else
    //   return -0.2;

    // //amplitute times and frequency in hertz which is A on the keyboard
    // //return 0.3 * sin(320.0 * 2 * 3.14159 * dTime);
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
    olcNoiseMaker<short> sound(devices[0], 44100, 1, 16, 512); 

    // links the noise function with sound machine class
    sound.SetUserFunction(MakeNoise);

    double dOctaveBaseFrequency = 110.0; // First note in the octave
    double d12thRootOf2 = pow(2.0, 1.0 / 12.0); 

    while (1)
    {


        //keyboard piano style

        bool bKeyPressed = false;
        
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

        // Notes to play
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