# C++ Synthesizer Project

A real-time software synthesizer built in C++ that demonstrates fundamental concepts of audio synthesis and digital signal processing.

## Features

- Real-time audio synthesis
- Multiple waveform types (Sine, Square, Triangle, Sawtooth, etc.)
- ADSR envelope control
- Low Frequency Oscillator (LFO) modulation
- Multiple preset sounds
- Keyboard input support

## Technical Details

### Core Components

1. **Oscillators**
   - `OSC_SINE` (0): Pure tone with no harmonics
   - `OSC_SQUARE` (1): Rich in odd harmonics, harsh sound
   - `OSC_SAWTOOTH` (2): Rich in both odd and even harmonics
   - `OSC_TRIANGLE` (3): Smoother than square, still has harmonics
   - `OSC_RAMP` (4): Similar to sawtooth
   - `OSC_PULSE` (5): Like square but between 0 and 1
   - `OSC_NOISE` (6): Random values for effects

2. **ADSR Envelope**
   - Attack: 100ms (time to reach full volume)
   - Decay: 10ms (time to reach sustain level)
   - Sustain: 80% (constant volume while note is held)
   - Release: 200ms (time to fade out)

3. **Sound Presets**
   - Rich Pad: Multiple oscillators with LFO modulation
   - Retro Game: Square and pulse waves
   - Bass: Triangle waves with sub-oscillator
   - Lead: Sawtooth with vibrato
   - Ambient Pad: Multiple sine waves
   - Hip Hop Bell: Triangle waves in perfect fifths

### Audio Generation

The synthesizer uses the following formula for frequency modulation:
```cpp
double dFreq = sin(w(dHertz) * dTime + dLFOAmplitude * dHertz * sin(w(dLFOHertz) * dTime));
```

Where:
- `w(dHertz)` converts frequency to angular velocity
- `dTime` is the current time in seconds
- `dLFOAmplitude` controls modulation depth
- `dLFOHertz` controls modulation rate

### Keyboard Mapping

The synthesizer uses a custom keyboard mapping:
```
A Z S X D C F V G B H N J M K , L . Q W E R T Y U I O P
```

Each key corresponds to a note in the scale, starting from A2 (110 Hz).

## Building and Running

1. Ensure you have a C++ compiler installed
2. Compile the project:
   ```bash
   g++ main1.cpp -o synthesizer
   ```
3. Run the executable:
   ```bash
   ./synthesizer
   ```

## Sound Design Concepts

### Waveform Types

1. **Sine Wave**
   - Pure tone with no harmonics
   - Used for clean, smooth sounds
   - Formula: `sin(θ)`

2. **Square Wave**
   - Rich in odd harmonics
   - Creates harsh, buzzy sounds
   - Formula: `sign(sin(θ))`

3. **Triangle Wave**
   - Smoother than square
   - Contains odd harmonics
   - Formula: `asin(sin(θ)) * (2/π)`

4. **Sawtooth Wave**
   - Rich in both odd and even harmonics
   - Creates bright, buzzy sounds
   - Formula: `2 * (t - floor(t + 0.5))`

### Modulation Techniques

1. **LFO (Low Frequency Oscillator)**
   - Used for vibrato and tremolo effects
   - Typically operates below 20 Hz
   - Can modulate frequency or amplitude

2. **Frequency Modulation**
   - Creates vibrato effect
   - Modulates the pitch of the sound
   - Controlled by LFO rate and amplitude

3. **Amplitude Modulation**
   - Creates tremolo effect
   - Modulates the volume of the sound
   - Can be used for rhythmic effects

### Envelope Shaping

The ADSR envelope controls how a sound evolves over time:

1. **Attack**
   - How quickly the sound reaches full volume
   - Short attack for percussive sounds
   - Long attack for pad sounds

2. **Decay**
   - How quickly the sound drops to sustain level
   - Short decay for sharp sounds
   - Long decay for smooth transitions

3. **Sustain**
   - The constant volume while note is held
   - Higher for continuous sounds
   - Lower for more dynamic sounds

4. **Release**
   - How quickly the sound fades out
   - Short release for tight sounds
   - Long release for ambient sounds

## Future Improvements

1. Add more waveform types
2. Implement filters (LPF, HPF, BPF)
3. Add effects (reverb, delay, chorus)
4. Implement MIDI support
5. Add GUI for real-time control
6. Add preset management system
7. Implement polyphony
8. Add recording functionality

## Dependencies

- olcNoiseMaker.h (included)
- Standard C++ libraries
- Windows API (for keyboard input)

## License

This project is open source and available for educational purposes.
