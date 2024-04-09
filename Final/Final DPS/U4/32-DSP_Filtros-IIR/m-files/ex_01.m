%% 1) Filtro Leaking Integrator (LI) con señales senoidales en MATLAB
%% a) Genere una señal senoidal con frecuencia fundamental de 100Hz.

fn=100;
fs=44100;
dt=1/fs;
P=1/fn;
t=0:dt:P*4;

signal=sin(2*pi*fn*t);

%% b) Agregue ruido a la señal senoidal tal que la relación señal a ruido entre la señal senoidal y la señal con ruido sea de 15 dB.

signal_n = awgn (signal,15);

%% c) Diseñe un filtro leaking integrator (LI) con ? igual a 0.7.
lamda = 0.7;
p = 1-lamda;
a = [1,-lamda];

signal_f=filter(p,a,signal_n);

%% d) Grafique la respuesta en frecuencia y fase del filtro LI. 
% Use la función freqz(). Determine la frecuencia de corte fco con:
% fco = - ln (lamda) . fs / pi

figure; freqz(signal_f);

fco = (- log (lamda) * fs) / pi
disp('fco='); disp(fco);

%% e) Determine el cero y el polo del filtro con la función zplane(). 
% ¿Es el filtro estable?.

figure; zplane(p,a);

%% f) Aplique el filtro LI a la señal con ruido. Utilice la función
% filter().
% signal_f=filter(p,a,signal_n);

% g) Grafique la respuesta en el tiempo de las señales original y 
% filtrada y compare.
figure; 
plot(t,signal); grid on;
hold on
plot(t,signal_n); 
plot(t,signal_f);
hold off

%% h) Grafique la respuesta en frecuencia de las señales original y 
% filtrada y compare. Utilice la función provista my_dft().
figure;
[frec,mag]=my_dft(signal,fs);
[frec_n,mag_n]=my_dft(signal_n,fs);
[frec_f,mag_f]=my_dft(signal_f,fs);

plot(frec,mag,'r'); 
hold on
plot(frec_n,mag_n,'b');
plot(frec_f,mag_f,'g');
hold off

%% i) Repita los puntos c) a h) para ? igual a 0.9 y 0.98. Analice el comportamiento de la
% fco.