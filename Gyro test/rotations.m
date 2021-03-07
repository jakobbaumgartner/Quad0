function result = rotations(inputstring)

inputstring = inputstring(5:length(inputstring));

%     find xrot
    for i = 1:length(inputstring)

       if inputstring(i) == 124

           xrot = str2double(inputstring(1:i-1));
           inputstring = inputstring(i+6: end);
           break
       end

    end
    
    
%    find yrot

    for i = 1:length(inputstring)

           if inputstring(i) == 124

               yrot = str2double(inputstring(1:i-1));
               inputstring = inputstring(i+6: end);
               break
           end

    end
    
%    find zrot

    for i = 1:length(inputstring)

           if inputstring(i) == 124

               zrot = str2double(inputstring(1:i-1));
               inputstring = inputstring(i+1: end);
               break
           end

    end    

result = [xrot, zrot, yrot]
end

