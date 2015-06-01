function [s, flag] = transmitSerial()

try
    ppmValues = [2000, 2000, 2000, 2000, 2000, 2000];
    numChannels = size(ppmValues);
    numChannels = numChannels(2);

    %Serial setup
    s = serial('/dev/tty.usbmodem1411');    % define serial port
    %s = serial(comPort);
    s.BaudRate=115200;               % define baud rate
    s.DataBits=8;
    s.StopBits=1;
    s.Parity= 'none';
    set(s, 'terminator', 'LF');

    fopen(s);              % open serial port
    %display(['Serial Open']);
    pause(2);

    w = 'X';


    %try                             % use try catch to ensure fclose
    while(w ~= 'A')    
        %w=fscanf(s,'%s'); 
        w=fread(s,1,'uchar');              % must define the input % d or %s, etc.
    end

    display(['ArduinoPPM found']);
    flag = 1;
    
    % Packets are organized into two bytes, MSB and LSB:
    % MSB is given by: xxx00yyy 
    % LSB is given by: yyyyyyyy 
    % xxx is routing information, yyyyyyyyyyyy is a 12 bit number, and the two zero's are for
    % padding and to aid in framing on the Arduino end. 

    %fprintf(s,'%c', 's');     % the arduino is waiting to receive 'ACK,' send it now 
    %fprintf(s,'%c', 't');         % the ardunio expects a 't' following 's' to signal start of frame
%    for i=1:numChannels
%         route = cast(i, 'uint16'); %turn routing information into a byte 
%         route = bitshift(route, 13); %put the 3 routing bits into the most significant bits of route
% 
%         MSB = cast(0, 'uint16'); %create new blank MSB, the first byte to be sent
%         msbMask = cast(1792, 'uint16'); %get 3 lsb of msb of uint16 in ppmValues(i)
%         MSB = bitand(ppmValues(i), msbMask);
%         MSB = bitor(MSB, route);        %conbine routing information with MSB
%         
%         display(dec2bin(MSB))
%         display(dec2bin(ppmValues(i)))
% 
%         lsbMask = cast(255, 'uint16');     %get the lower 8 bits of ppmValues(i)
%         LSB = bitand(ppmValues(i), lsbMask);
%         
%         display(dec2bin(LSB))
%         display(dec2bin(bitor(MSB,LSB)))
%         
%         MSB = bitshift(MSB, -8);
%         MSB = cast(MSB, 'uint8');
%         LSB = cast(LSB, 'uint8');
        %fprintf(s, '%d\n', (i-1));
 %       fprintf(s,'%d\n', ppmValues(i));
 %       fprintf(s, '%s\n', '\n');
        %pause(1)
        %disp(fscanf(s,'%s'));

        %fprintf(s,'%d\n',LSB);

 %   end
    
    
    for i=1:numChannels
        fprintf(s, '%s\n', '\n');
        fprintf(s, '%s\n', '\n');
        fprintf(s, '%s\n', '\n');
        fprintf(s, '%s\n', '\n');
        fprintf(s,'%d\n', ppmValues(i));
        fprintf(s, '%s\n', '\n');
        fprintf(s, '%s\n', '\n');

    end
    
flag = 1;

catch ME
    fclose(s);
    ME
end




