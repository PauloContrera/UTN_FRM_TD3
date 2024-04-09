% ¡¡¡¡¡¡¡¡¡¡ EJERCICIO IIR RESUELTO EN ESTE SCRIPT !!!!!!!!!!!!!!!!!!!

%% Final TD3
clear
clc
close all

%% FRECUENCIAS.

fn1 = 1000; % Fundamental
fn2 = 50; % Interferencia
fs = 10000; % Frec. muestreo

t = 0:1/fs:0.05; % De 0 a 0.05, en incrementos de 1/fs

%% SEÑAL DE ENTRADA. 

signal = sin(2*pi*fn1*t) + (0.5*sin(2*pi*fn2*t));

%% FILTRO IIR.
Hd = ButterworthProbando;

SOS =  Hd.sosMatrix; % Extrae los coeficientes de segunda orden del objeto
% de filtro digital Hd y los almacena en la variable SOS
G = Hd.ScaleValues;

%% FILTRADO IIR

iir_output = filtfilt(SOS,G,signal);

%% GRAFICAS.

figure

title 'Dominio del tiempo'
plot(t,signal,'g',t,iir_output,'b');
legend('Señal original','Señal filtrada');
grid on
  
title 'Dominio Frecuencia'
[frec,mag] = my_dft(signal,fs);
[iir_frec,iir_mag] = my_dft(iir_output,fs);

figure

plot(frec,mag,'r',iir_frec,iir_mag,'b');
legend('Señal original','Señal filtrada');
grid on




