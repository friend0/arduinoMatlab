% Communications MatLab <--> Arduino
% Matlab file 1 for use with Arduino file 1
clc;
clear all;
numChannels = 6;
numSec=1;
t=[];
v=[];
ppmValues = zeros(1,numChannels,'uint16');

%Serial setup
s1 = serial('/dev/cu.usbmodem1411');    % define serial port
s1.BaudRate=9600;               % define baud rate
set(s1, 'DataBits', 8);
set(s1, 'StopBits', 1);
set(s1, 'BaudRate', 9600);
set(s1, 'Parity', 'none');
%set(s1, 'terminator', 'LF');    % define the terminator for println
%fopen(s1);              % open serial port

try                             % use try catch to ensure fclose
                                % signal the arduino to start collection
w=fscanf(s1,'%s');              % must define the input % d or %s, etc.
if (w=='A')
    display(['Collecting data']);
    fprintf(s1,'%s\n','s');     % establishContact just wants 
                                % something in the buffer
end

%Now 's' and 't' have been sent the Arduino ought to be ready to receive
%all our packets

fprintf(s1,'%s\n','t');

%initialize ppmValues to default of 1500mS
for i=1:numChannels    
    ppmValues(i) = 1500;
end

%for each channel send routing information, MSB and LSB
for i=1:numChannels
    %should probably check if ppmValues are greater than the 11-bit number
    %we are expecting, with a range of 2047    

    route = cast(i, 'uint16'); %turn routing information into a byte 
    route = bitshift(route, 13); %put the 3 routing bits into the most significant bits of route
    %dec2bin(route)

    MSB = cast(0, 'uint16'); %create new blank MSB, the first byte to be sent
    msbMask = cast(1792, 'uint16'); %get 3 lsb of msb of uint16 in ppmValues(i)
    MSB = bitand(ppmValues(i), msbMask);
    MSB = bitor(MSB, route);        %conbine routing information with MSB
    
    lsbMask = cast(255, 'uint16');     %get the lower 8 bits of ppmValues(i)
    LSB = bitand(ppmValues(i), lsbMask);
    dec2bin(MSB)
    dec2bin(LSB)
    %dec2bin(bitor(MSB, LSB))
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