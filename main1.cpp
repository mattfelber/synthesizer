#include <iostream>
#include <vector>
#include <string>

using namespace std;

#include "olcNoiseMaker.h"

// Convert frequency in hertz to angular velocity (radians per second)
// This is needed because C++ trigonometric functions use radians, not Hz
// Formula: Angular velocity = 2π × frequency (Hz)
double w(double dHertz)
{
    return dHertz * 2.0 * PI;
}

// Define constants for different oscillator types to make code more readable
#define OSC_SINE 0      // Sine wave - smooth, pure tone
#define OSC_SQUARE 1    // Square wave - harsh, buzzy sound with odd harmonics
#define OSC_TRIANGLE 2  // Triangle wave - smoother than square, still with harmonics
#define OSC_SAW_ANA 3   // Analog-style sawtooth - rich in harmonics, bright sound
#define OSC_SAW_DIG 4   // Digital sawtooth/ramp - similar to analog saw but implemented differently
#define OSC_NOISE 5     // Noise - random values, useful for percussion or effects

/**
 * Oscillator function that generates different waveforms
 * 
 * @param dHertz - The base frequency of the oscillator in Hz (e.g., 440 for A4 note)
 * @param dTime - The current time in seconds (used to calculate the phase of the wave)
 * @param nType - The type of waveform to generate (uses the OSC_* constants)
 * @param dLFOHertz - Frequency of the Low Frequency Oscillator in Hz (for vibrato/modulation effects)
 * @param dLFOAmplitude - Amount of LFO modulation to apply (0.0 = none, higher values = more)
 * 
 * @return A value between -1.0 and 1.0 representing the amplitude of the waveform at time dTime
 */
double osc(double dHertz, double dTime, int nType = OSC_SINE, double dLFOHertz = 0.0, double dLFOAmplitude = 0.0)
{
    // Calculate the frequency with LFO modulation applied
    // This creates a vibrato effect by slightly varying the frequency over time
    // The inner sin(w(dLFOHertz) * dTime) oscillates slowly at the LFO frequency
    // This is then scaled by dLFOAmplitude and the base frequency (dHertz)
    // The result modulates the phase of the main oscillator
    double dFreq = sin(w(dHertz) * dTime + dLFOAmplitude * dHertz * sin(w(dLFOHertz) * dTime));

    switch(nType)
    {
        case 0: // Sine wave
            // The simplest and purest waveform with only the fundamental frequency
            // No harmonics, produces a very clean tone
            //return sin(w(dHertz) * dTime + 0.01 * dHertz * sin(w(5.0) * dTime)); // Old implementation without variable LFO
            return sin(dFreq);
        
        case 1: // Square wave 
            // Creates a harsh, buzzy sound rich in odd harmonics
            // Implemented by checking if the sine wave is positive or negative
            // and returning either 1.0 or -1.0 accordingly
            // C++ feature: This uses the ternary operator (condition ? true_value : false_value)
            return sin(dFreq) > 0.0 ? 1.0 : -1.0;
        
        case 2: // Sawtooth wave
            // Rich in both odd and even harmonics, creates a bright, buzzy sound
            // The formula creates a ramp that rises and then sharply falls
            // Implementation steps:
            // 1. dHertz * dTime creates a linearly increasing value
            // 2. Add LFO modulation with sin(w(dLFOHertz) * dTime)
            // 3. Use floor(...+ 0.5) to round to nearest integer
            // 4. Scale to range -1.0 to 1.0 with the 2.0 * (...) part
            return (2.0 * (dHertz * dTime + dLFOAmplitude * sin(w(dLFOHertz) * dTime) - floor(dHertz * dTime + dLFOAmplitude * sin(w(dLFOHertz) * dTime) + 0.5)));
        
        case 3: // Triangle wave
            // Smoother than square wave but still contains odd harmonics
            // Uses arcsin (asin) of a sine wave scaled by 2/PI to create the triangular shape
            // Math concept: asin() is the inverse of sin(), and scaling shapes the output
            //return (2.0 * fabs(2.0 * (dHertz * dTime - floor(dHertz * dTime + 0.5))) - 1.0); // Old implementation using absolute value
            return asin(sin(dFreq) * (2.0 / PI));
            
        case 4: // Ramp wave
            // Similar to sawtooth but with a different implementation
            // Creates a ramp that rises linearly and then resets
            // Different from sawtooth: No 0.5 offset in the floor function
            // LFO modulation is applied to the phase calculation for vibrato effect
            return (2.0 * (dHertz * dTime + dLFOAmplitude * sin(w(dLFOHertz) * dTime) - floor(dHertz * dTime + dLFOAmplitude * sin(w(dLFOHertz) * dTime))));
        
        case 5: // Pulse wave
            // Similar to square wave but oscillates between 0 and 1 (not -1 and 1)
            // Often used for retro video game sounds and percussion
            // Uses the ternary operator to check if the sine wave is positive
            return (sin(dFreq) > 0.0) ? 1.0 : 0.0;
        
        case 6: // Noise wave
            // Generates random values between -1.0 and 1.0
            // For noise, we use the LFO to modulate the amplitude rather than frequency
            // This creates a tremolo effect (volume variation) rather than vibrato (pitch variation)
            // Implementation steps:
            // 1. (double)rand() / (double)RAND_MAX generates random value between 0.0 and 1.0
            // 2. Scale by a value that oscillates with the LFO
            // 3. Shift to center around zero
            // C++ feature: Type casting with (double) converts integers to floating point
            return ((double)rand() / (double)RAND_MAX) * (2.0 + dLFOAmplitude * sin(w(dLFOHertz) * dTime)) - (1.0 + dLFOAmplitude * sin(w(dLFOHertz) * dTime) * 0.5);
        
        case 7: // White noise
            // Another implementation of noise, identical to case 6
            // Could be modified to implement different noise colors (pink, brown, etc.)
            // White noise has equal energy per frequency band
            // Useful for percussion sounds, wind effects, and texture
            return ((double)rand() / (double)RAND_MAX) * (2.0 + dLFOAmplitude * sin(w(dLFOHertz) * dTime)) - (1.0 + dLFOAmplitude * sin(w(dLFOHertz) * dTime) * 0.5);
        
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
        + 1.0 * osc(dFrequencyOutput, dTime, OSC_SINE, 5.0, 0.01)
        //+ osc(dFrequencyOutput * 0.5, dTime, 0)   //sine
        //+ osc(dFrequencyOutput * 0.5, dTime, 1)   //square
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

    // ====================== BASE-FREQUENCY =====================================
    double dOctaveBaseFrequency = 110; // First note in the octave (A2 - 110Hertz)
    double d12thRootOf2 = pow(2.0, 1.0 / 12.0); 

    int nCurrentKey = -1; // Add this line before your while loop

    while (1)
    {
        bool bKeyPressed = false;

        for (int k = 0; k < 27; k++)
        {
            if (GetAsyncKeyState((unsigned char)("AZSXDCFVGBHNJMK\xbcL\xbeQWERTYUIOP"[k])) & 0x8000)
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