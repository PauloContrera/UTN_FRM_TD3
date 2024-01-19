function Hd = Final_TD3_filt
%FINAL_TD3_FILT Returns a discrete-time filter object.

% MATLAB Code
% Generated by MATLAB(R) 9.5 and Signal Processing Toolbox 8.1.
% Generated on: 26-Aug-2021 18:32:02

% Butterworth Bandpass filter designed using FDESIGN.BANDPASS.

% All frequency values are in Hz.
Fs = 10000;  % Sampling Frequency

N   = 6;     % Order
Fc1 = 1000;  % First Cutoff Frequency
Fc2 = 3000;  % Second Cutoff Frequency

% Construct an FDESIGN object and call its BUTTER method.
h  = fdesign.bandpass('N,F3dB1,F3dB2', N, Fc1, Fc2, Fs);
Hd = design(h, 'butter');

% [EOF]
