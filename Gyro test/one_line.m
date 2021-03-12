function serialoutput = one_line(serialinput)

    starting = 0;
    ending = 0;
    
    for i = 1:length(serialinput)
        serialinput;
        
        if serialinput(i) == 13 && starting ~= 0
            ending = i-1;
            break
        end
        
        if serialinput(i) == 13 && starting == 0
            starting = i+2;
        end
               
    end
    
    serialoutput = char(serialinput(starting:ending));
end