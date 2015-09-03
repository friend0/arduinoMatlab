function [flag] = transmitSerial(s, ppmValues)
pause on;
try
    %s = serialObject;
    numChannels = size(ppmValues);
    numChannels = numChannels(2);
        
    fprintf(s, '%d,', 65535);     %indicate start of frame (0b1111111111111111)
    for i=1:numChannels
    fprintf(s,'%d,', ppmValues(i));     %send all ppm values in frame
    end
    fprintf(s,'\n,');                   %indicate end of frame
    flag = 1;

catch ME
    fclose(s);
    flag = 0;
    ME
end




