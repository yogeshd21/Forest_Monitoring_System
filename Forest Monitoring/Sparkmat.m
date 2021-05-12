cam = webcam;
C = 20;
preview(cam)
for i=1:C
a = snapshot(cam);
q = rgb2gray(a);
imshow(q);
impixelinfo
z = 0;
b = size(q);
X = b(1);
Y = b(2);
for x=1:X
    for y=1:Y
        if a(x,y)>=200
            z=z+1;
        end
    end
end
disp(z);
if z>=70000
    [c,Fs]=audioread('Car_Alarm_Device_With_Horn_-_Ringtone.mp3');
    p = audioplayer(c,Fs);
    play(p)
    imshow(rgb2gray(a))
    impixelinfo
end
end