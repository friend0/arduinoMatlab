% Communications MatLab <--> Arduino
% Matlab file 1 for use with Arduino file 1
clc;
clear all;
numSec=1;
t=[];
v=[];

s1 = serial('/dev/cu.usbmodem1411');    % define serial port
s1.BaudRate=9600;               % define baud rate
set(s1, 'terminator', 'LF');    % define the terminator for println
fopen(s1);

try                             % use try catch to ensure fclose
                                % signal the arduino to start collection
w=fscanf(s1,'%s');              % must define the input % d or %s, etc.
if (w=='A')
    display(['Collecting data']);
    fprintf(s1,'%s\n','A');     % establishContact just wants 
                                % something in the buffer
end

i=0;
t0=tic;
while (toc(t0)<=numSec)
    i=i+1;
    t(i)=toc(t0);
    t(i)=t(i)-t(1);
    v(i)=fscanf(s1,'%d');       % must define the input % d or %s, etc.
end
fclose(s1);
plot(t,v,'*r')                  % a very interesting graph
                                % if you need precise timing
                                % you better get it from the 
                                % arduino (see file 2)
catch me
    fclose(s1);                 % always, always want to close s1
end                             