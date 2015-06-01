numChannels = 6;
ppmValues = zeros(1,numChannels,'uint16');

for i=1:numChannels    
    ppmValues(i) = 1500;
end

for i=1:numChannels
    %should probably check if ppmValues are greater than the 11-bit number
    %we are expecting, with a range of 2047   
    i
    route = cast(i, 'uint16'); %turn routing information into a byte 
    route = bitshift(route, 13); %put the 3 routing bits into the most significant bits of route
    display(dec2bin(route))

    MSB = cast(0, 'uint16'); %create new blank MSB, the first byte to be sent
    msbMask = cast(1792, 'uint16'); %get 3 lsb of msb of uint16 in ppmValues(i)
    MSB = bitand(ppmValues(i), msbMask);
    MSB = bitor(MSB, route);        %conbine routing information with MSB
    
    lsbMask = cast(255, 'uint16');     %get the lower 8 bits of ppmValues(i)
    LSB = bitand(ppmValues(i), lsbMask);
    
    %dec2bin(bitor(MSB, LSB))

    %MSB = bitshift(MSB, -8);
    %MSB = cast(MSB, 'uint8');
    %LSB = cast(LSB, 'uint8');
    
    display(dec2bin(bitor(MSB,LSB)))
end