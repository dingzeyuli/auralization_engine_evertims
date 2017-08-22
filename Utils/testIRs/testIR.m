clear
close all
[h, Fs_IR] = audioread('Evertims_IR_Recording_binaural.wav');
[h, Fs_IR] = audioread('h-1300-to-6000.wav');
%[h, Fs_IR] = audioread('h178_OfficeFoyer_1txts.wav');

if (size(h,2) == 1)
    h(:,2) = h;  
end

[x, fs] = audioread('/Users/dinli/Codes/vocal-transfer-github/data/SpEARdatabase/TIMIT/housewives_16.wav');

x = resample(x, Fs_IR, fs);

y1= conv(x, h(:,1));
y2= conv(x, h(:,2));
y= [y1,y2];
y= y/max(max(abs(y))); % normalize max. amplitude to 1

sound(y, Fs_IR)

%audiowrite('h178.wav', y, Fs_IR)
%audiowrite('h35.wav', y, Fs_IR)
audiowrite('h-conf.wav', y, Fs_IR)
