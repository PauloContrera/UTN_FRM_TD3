% ���������� EJERCICIO FIR RESUELTO EN ESTE SCRIPT !!!!!!!!!!!!!!!!!!!

% EJERCICIO FINAL TECNICAS DIGITALES III
%
% Genere una se�al que sume una se�al senoidal con frecuencia 
% fundamental fn = 1000 Hz con una se�al de 50 Hz. Esta �ltima se
% considera como una interferencia. Elija una frecuencia de muestreo 
% adecuada.
% 
% Se desea dise�ar un filtro pasa-banda que rechace la se�al de 50 Hz y
% que deje el tono de 1000 Hz.
% 
% Use la herramienta filterDesigner para dise�ar un filtro FIR
% pasa-banda con frecuencias de corte de 500 Hz y 1500 Hz tipo ventana de 
% Kaiser con � = 7.5, orden 100 y formato punto flotante, precisi�n 
% doble. 
% 
% Procese la se�al de inter�s con el filtro dise�ado.
% 
% Grafique la se�al de entrada al filtro y la se�al de salida del
% filtro, ambas en el dominio del tiempo y de la frecuencia.

clc
clear
close all

%% FRECUENCIAS

fs  = 10000;            % Sampling freq.
fn1 = 1000;
fn2 = 50;  
 
t = 0:1/fs:0.05;

%% SENAL DE ENTRADA

signal = sin(2*pi*fn1*t) + (0.5*sin(2*pi*fn2*t));

%% FILTRO FIR

Hd = filtroFIR;

% Coeficiente del filtro por ventana.
B = Hd.Numerator;
A = 1;

signal_f = filter(B,A,signal);


%% FILTRADO FIR

% fir_output = filtfilt(SOS, G, signal);
signal_f = filter(B,A,signal);

% subplot (2,1,2), plot(signal(:,1),'b'),grid on, title 'filtrada';

%% GRAFICAS

figure

title 'Dominio del tiempo'
plot(t,signal,'g',t,signal_f,'b');
legend('Se�al original','Se�al filtrada');
grid on
%   
title 'Dominio Frecuencia'
[frec,mag] = my_dft(signal,fs);
[fir_frec,fir_mag] = my_dft(signal_f,fs);

figure

plot(frec,mag,'r',fir_frec,fir_mag,'b');
legend('Se�al original','Se�al filtrada');
grid on

