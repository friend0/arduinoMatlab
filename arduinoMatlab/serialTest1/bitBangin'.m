numChannels = 8;
ppmValues = zeros(1,numChannels,'uint16');

for i=1:numChannels    
    ppmValues(i) = 1500
end

for i=1:numChannels
    %should probably check if ppmValues are greater than the 11-bit number
    %we are expecting, with a range of 2047    
    route = cast(i, uint8); %turn routing information into a byte 
    route = bitshift(route, 5); %put the 3 routing bits into the most significant bits of route
    
    MSB = cast(0, uint8); %create new blank MSB, the first byte to be sent
    msbMask = cast(1792, uint8); %get 3 lsb of msb of uint16 in ppmValues(i)
    MSB = bitand(ppmValues(i), msbMask);
    MSB = bitor(MSB, route);
    
    lsbMask = cast(255, uint8);     %get the lower 8 bits of ppmValues(i)
    LSB = bitand(ppmValues(i), lsbMask);
end