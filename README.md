# OLC Noise Maker Synthesizer

A simple audio synthesizer built using the OneLoneCoder NoiseMaker framework that allows you to play musical notes using your keyboard.

![Synthesizer](https://img.shields.io/badge/Audio-Synthesizer-brightgreen)
![Platform](https://img.shields.io/badge/Platform-Windows-blue)
![License](https://img.shields.io/badge/License-GNU%20GPL%20v3-yellow)

## Overview

This project is a simple synthesizer that generates sound waves in real-time and allows you to play them using your keyboard in a piano-like layout. It demonstrates fundamental concepts of digital audio synthesis including:

- Waveform generation
- Real-time audio processing
- Musical note frequency calculation
- Keyboard input handling

## Features

- Real-time audio synthesis
- Keyboard-based musical input (piano layout)
- Square wave generation
- Adjustable base frequency
- Uses Windows audio API for low-latency output

## Requirements

- Windows operating system
- C++ compiler (supporting C++11 or later)
- Windows Multimedia library (winmm.lib)

## Installation

1. Clone this repository:
   ```
   git clone https://github.com/mattfelber/synthesizer.git
   ```

2. Compile the project using your preferred C++ compiler:
   ```
   g++ main1.cpp -o synthesizer -lwinmm
   ```
   
   Or using Visual Studio:
   - Open the project in Visual Studio
   - Build the solution (make sure winmm.lib is linked)

## Usage

1. Run the compiled executable
2. The program will list available audio output devices and select the default one
3. Play notes using the following keyboard layout:
   ```
   Z S X C F V G B N J M K , L .
   ```
   This corresponds to a piano-like layout with white and black keys

4. Press Ctrl+C to exit the program

## How It Works

The synthesizer uses a template-based audio engine (`olcNoiseMaker`) that handles the low-level audio output through the Windows API. The main components are:

1. **Audio Engine**: Manages audio device communication, buffering, and threading
2. **Waveform Generator**: The `MakeNoise` function that creates a square wave based on frequency
3. **Keyboard Input**: Maps keyboard keys to musical frequencies using the 12th root of 2 formula

The base frequency is set to 110Hz (A2), and each key increases the pitch by one semitone according to the standard musical scale.

## Customization

You can modify the `MakeNoise` function to create different waveforms:
- Sine waves: `return amplitude * sin(frequency * 2 * PI * dTime);`
- Square waves: Currently implemented
- Triangle waves: Implement using conditional logic based on time
- Sawtooth waves: `return amplitude * (2 * (frequency * dTime - floor(0.5 + frequency * dTime)));`

## Credits

- Original framework by Javidx9 (OneLoneCoder.com)
- Tutorial: [Simple Audio Noisy Thing](https://youtu.be/tgamhuQnOkM)

## License

This project is licensed under GNU GPL v3 - see the original framework for details.

## Future Improvements

- Add envelope generation (ADSR)
- Implement multiple waveform types
- Add effects (reverb, delay, etc.)
- Create a graphical user interface
- Support for MIDI input devices
