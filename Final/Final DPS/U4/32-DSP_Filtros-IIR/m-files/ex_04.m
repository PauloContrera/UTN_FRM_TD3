%% 4) Dise�o de un filtro IIR con transformada bilineal
% Dise�e un filtro digital tipo IIR con la transformada 
% bilineal a partir del dise�o de un filtro anal�gico.

%% a) Dise�e un filtro Chebyshev Tipo I pasa-banda entre 
% 300 y 3.400 Hz con 3 dB en la banda pasante. Aplique 
% precombado (pre-warping) a las frecuencias anal�gicas de
% inter�s. Use las funciones cheb1ap() y lp2bp().

[z,p,k]=cheb1ap(10,3);
[a,b]=zp2tf(z,p,k);

fs=900;
fc1=300;
fc2=3400;

u1= 2*fs*tan(fc1*(2*pi/fs)/2); %precombado
u2= 2*fs*tan(fc2*(2*pi/fs)/2); 

bw=u2-u1;
wo=sqrt(u1*u2); %frecuencia central

[bt,at]=lp2bp(a,b,wo,bw);

%% b) Grafique la respuesta en frecuencia y fase del filtro
% anal�gico. Use la funci�n freqs().

[h,w]=freqs(bt,at); %es freqs porq estamos en el dominio s
figure; plot (w,mag2db(abs(h)));

%% c) Discretice el filtro anal�gico para una frecuencia de 
% muestreo de 9600 Hz con el m�todo de interpolaci�n 
% �tustin�. Use la funci�n c2d().

H=tf(a,b); % TF en dom s
Hd=c2d(H,1/fs); % paso a dom Z

%% d) Grafique la respuesta en frecuencia y fase del filtro
% digital. Use la funci�n freqz().
B = Hd.num;
A = Hd.den;

[hz,wz]=freqz(Hd);
hold on
plot(wz,mag2db(abs(hz)));

%% e) Compare con la respuestas de ambos filtros.