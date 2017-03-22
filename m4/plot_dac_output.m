% data file format is:
% time value
% 
%[t, cells] = textread('dac.txt', '%f %s');
file = 'dac_1004hz_10ksps_3k.txt'

[t, hex] = textread(file, '%f %s');
ascii_hex = cell2mat(hex);

data = typecast(uint16(hex2dec(ascii_hex)'), 'int16');

n = length(data);

Ts = (t(end) - t(1)) / (n-1)

Fs= 1/Ts    

adc_min = -2^15;
adc_max = 2^15-1;


%signal = voltage_max * data / adc_max;
signal = double(data);

subplot(2,1,1);
plot(1000*t, signal);
title('Signal');
xlabel('time (ms)');
%ylabel('voltage');
ylabel('ADC reading');

dft = fft(signal);

% FFT calculation is symmetric about zero, take first half
mag = abs( dft(1:n/2+1) );

% frequencies are from 0 - Fs/2 (Nyquist freq)
F = linspace(0,Fs/2,length(mag));

subplot(2,1,2);
plot(F,mag);
xlabel('freq (Hz)');
ylabel('power');

% get the frequency with the highest magnitude, ignoring dc component
[max_mag,i] = max([0, mag(2:end)]);

fundamental = F(i)
