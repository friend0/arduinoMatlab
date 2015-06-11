function [s, flag] = initSerial(port, channels);
pause on;

switch nargin
    case 1
        comport = port;
        numChannels = 6; %default
    case 2
        comport = port;
        numChannels = channels;
    otherwise
        comport = '/dev/tty.usbmodem1411';      %default
        numChannels = 6;                        %default
end
        
try

    %Serial setup
    s = serial('/dev/tty.usbmodem1411');    % define serial port
    %s = serial(comPort);
    s.BaudRate=9600;               % define baud rate
    s.DataBits=8;                  %default config of arduino 8 data bits, one stop bit, no parity
    s.StopBits=1;
    s.Parity= 'none';
    set(s, 'terminator', 'LF');

    fopen(s);              % open serial port
    %display(['Serial Open']);
    pause(2)
    w = 'X';
    %try                             % use try catch to ensure fclose
    while(w ~= 'A')    
        %w=fscanf(s,'%s'); 
        w=fread(s,1,'uchar');              % must define the input % d or %s, etc.
    end
    fprintf(s, '%d,', numChannels);     %indicate start of frame
    display(['ArduinoPPM found']);
    flag = 1;
    
catch ME
    fclose(s);
    flag = 0;
    ME
end




