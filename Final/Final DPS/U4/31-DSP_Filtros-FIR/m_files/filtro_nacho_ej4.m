function Hd = filtro_nacho_ej4
%FILTRO_NACHO_EJ4 Returns a discrete-time filter object.

% MATLAB Code
% Generated by MATLAB(R) 9.0 and the DSP System Toolbox 9.2.
% Generated on: 05-May-2023 13:29:59

% FIR Window Bandpass filter designed using the FIR1 function.

% All frequency values are in Hz.
Fs = 8000;  % Sampling Frequency

N    = 10;       % Order
Fc1  = 300;      % First Cutoff Frequency
Fc2  = 3400;     % Second Cutoff Frequency
flag = 'scale';  % Sampling Flag
Beta = 7.5;      % Window Parameter
% Create the window vector for the design algorithm.
win = kaiser(N+1, Beta);

% Calculate the coefficients using the FIR1 function.
b  = fir1(N, [Fc1 Fc2]/(Fs/2), 'bandpass', win, flag);
Hd = dfilt.dffir(b);

% [EOF]