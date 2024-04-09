%% Trabajo Practico DSP
clear
clc
close all
%% a) Cargue el archivo de audio provisto llamado Tchaikovsky.mat.

load Tchaikovsky.mat;

subplot(2,3,1); % crea 1 sub grafico de tama�o 2x3. Es el 1ero (indicado por el 1).
plot(signal(:,1),'r'); %  : se usa para seleccionar todas las filas de la matriz signal y 1 se utiliza para seleccionar la primera columna.
legend('Sin ruido');

%% b) Agregue ruido gaussiano a esta se�al tal que la relaci�n se�al-ruido entre la se�al y la se�al con ruido sea de 50 dB.

SNR = 50; %Relacion se�al a ruido.

signal_n = awgn(signal,SNR); %Se�al con ruido.

%Verifiacion del SNR.
SNR_test = snr(var(signal),var(signal_n-signal));

subplot(2,3,2);
plot(signal_n,'b');
legend('Con ruido');

%% c) Calcule el valor m�ximo de N (N_max), con las frecuencias fs = Fs y fco = 22.050Hz.

fco = 22050;
Fco = fco/Fs;

Nmax = sqrt((0.885894.^2)/(Fco.^2)+1);

%% d) Aplique filtrado del tipo moving average a la se�al con ruido para un filtro MA con dimensi�n igual N = N_max.

% M = round(Nmax/2);
M = round(Nmax);

%Coeficientes del filtro MA
% El filtro MA tiene la siguiente estructura:
% a*y[n] = b*(x[n]+x[n-1]+...+x[n-(N-1)])
% Donde a = 1 y b = 1/N (N es el orden del filtro).

a = 1;
b = ones(1,M)/M;

signal_ma = filter(b,a,signal_n); %Se�al filtrada.

subplot(2,3,3);
plot(signal_ma,'g');
legend('Filtrada');

%% e) Utilice la funci�n sound(signal_n, Fs) para reproducir las se�ales sin ruido, con ruido y filtrada.

% sound(signal,Fs);

% sound(signal_n,Fs);

% sound(signal_ma,Fs);

%% f) Grafique la respuesta en frecuencia de las se�ales original y filtrada y compare.

[frq,mag] = my_dft(signal,Fs); %Respuesta en frecuencia se�al original.
[frq_ma,mag_ma] = my_dft(signal_ma,Fs); %Respuesta en frecuencia se�al fitrada.

subplot(2,3,4);
plot(frq,mag,'r');
title('Respuesta en frecuencia')
legend('Original');

subplot(2,3,5);
plot(frq_ma,mag_ma,'b');
title('Respuesta en frecuencia')
legend('Filtrada');

%% h) Repita los puntos d) a g) para N = N_max / 2 y N = N_max * 10.
