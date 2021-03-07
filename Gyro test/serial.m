

s = serialport("COM7",9600)
 configureTerminator(s,0);
 flush(s)
 
 xline = [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0];
 yline = [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0];
 zline = [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0];
 
 plotfigure = figure();
 graphfigure = figure();
 

while 1
   
    a = read(s,200,"uint8");
    x = one_line(a)
    
    values = rotations(one_line(a));
    
    X = categorical({'X - acc','Y - acc','Z - acc'});
    
    figure(graphfigure)
    bar(X,values)
    
    xline = [xline(2:end) values(1)];
    yline = [yline(2:end) values(2)];
    zline = [zline(2:end) values(3)];
    
    figure(plotfigure)
    plot(xline)
    hold on
    plot(yline)
    plot(zline)
    hold off
    
    pause(0.1)
    
    
    flush(s)
end

