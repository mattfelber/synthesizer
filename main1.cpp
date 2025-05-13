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
        case 7: // White noise
            return ((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;
        
    }
}

struct sEnvelopeADSR
{

    double dAttackTime; // Time to reach max volume
    double dDecayTime; // Time to reach sustain level
    double dStartAmplitude; // Start volume
    double dSustainAmplitude; // Sustain level 
    double dReleaseTime; // Time to release sound
    double dTriggerOnTime;
    double dTriggerOffTime;
    bool bNoteOn;
    
    sEnvelopeADSR()
    {
        dAttackTime = 0.100; // 10ms
        dDecayTime = 0.01; // 100ms
        dStartAmplitude = 1.0; // 100%
        dSustainAmplitude = 0.8; // 80%
        dReleaseTime = 0.200; // 100ms
        dTriggerOnTime = 0.0;
        dTriggerOffTime = 0.0;
        bNoteOn = false;

    }


    double GetAmplitude(double dTime)
    {
        double dAmplitude = 0.0;
        double dLifeTime = dTime - dTriggerOnTime;

        if (bNoteOn)
        {
            // Attack phase
            if (dLifeTime <= dAttackTime)
            {
                dAmplitude = (dLifeTime / dAttackTime); // Scale from 0.0 to 1.0
            }

            // Decay phase
            if (dLifeTime > dAttackTime && dLifeTime <= (dAttackTime + dDecayTime))
            {
                
               dAmplitude = ((dLifeTime - dAttackTime) / dDecayTime) * (dSustainAmplitude - dStartAmplitude) + dStartAmplitude;
            }

            // Sustain phase
            if (dLifeTime > (dAttackTime + dDecayTime))
            {
                dAmplitude = dSustainAmplitude;
            }
        }
        else
        {
            // Release phase
            dAmplitude = ((dTime - dTriggerOffTime) / dReleaseTime) * (0.0 - dSustainAmplitude) + dSustainAmplitude;
        }

        // Ensure amplitude is not negative
        if (dAmplitude <= 0.0001)
        {
            dAmplitude = 0.0;
        }

        return dAmplitude;
    }

    void NoteOn(double dTimeOn)
    {
        dTriggerOnTime = dTimeOn;
        bNoteOn = true;
    }

    void NoteOff(double dTimeOff)
    {
        dTriggerOffTime = dTimeOff;
        bNoteOn = false;
    }

};

double dFrequencyOutput = 0.0; // Frequency of the sound wave in hertz
sEnvelopeADSR envelope; // Declare the envelope object



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

    

    // 0 = sine, 1 = square, 2 = sawtooth, 3 = triangle, 4 = ramp, 5 = pulse, 6 = noise
    

    double dOutput = envelope.GetAmplitude(dTime) * 
    ( 
        + osc(dFrequencyOutput * 0.5, dTime, 0)   //sine
        + osc(dFrequencyOutput * 0.5, dTime, 1)   //square
        //+ osc(dFrequencyOutput * 0.5, dTime, 2)   //sawtooth
        //+ osc(dFrequencyOutput * 0.5, dTime, 3)   //triangle
        //+ osc(dFrequencyOutput * 0.5, dTime, 4)   //ramp
        //osc(dFrequencyOutput * 0.5, dTime, 5)   //pulse
        //+ osc(dFrequencyOutput * 0.5, dTime, 6)   //noise
        //+ osc(dFrequencyOutput * 0.5, dTime, 7)     //white noise
    
    ); 
    
    return dOutput * 0.4; // MASTER VOLUME
    
   
}

int main() 
{

    
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

    // ================================================================== BASE FREQUENCY
    double dOctaveBaseFrequency = 110.0; // First note in the octave A2s
    double d12thRootOf2 = pow(2.0, 1.0 / 12.0); 

    int nCurrentKey = -1; // Add this line before your while loop

    while (1)
    {
        bool bKeyPressed = false;

        for (int k = 0; k < 15; k++)
        {
            if (GetAsyncKeyState((unsigned char)("ZSXCFVGBNJMK\xbcL\xbe"[k])) & 0x8000)
            {
                if (nCurrentKey != k)
                {
                    dFrequencyOutput = dOctaveBaseFrequency * pow(d12thRootOf2, k);
                    envelope.NoteOn(sound.GetTime());
                    nCurrentKey = k;
                }
                bKeyPressed = true;
            }
        }

        if (!bKeyPressed)
        {
            if (nCurrentKey != -1)
            {
                envelope.NoteOff(sound.GetTime());
                nCurrentKey = -1;
            }
        }
    }



    return 0;
}