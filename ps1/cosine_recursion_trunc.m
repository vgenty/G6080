function [] = cosine_recursion_trunc()

%  An example of the propagation of errors in the trigonometric
%  identity cos((M+1)x) = 2 * cos(x) * cos(M * x) - cos((M-1) * x)

close all
clear all

%  Set precision to use during arithmetic operations
%  P sets the number of decimal places of accuracy
%  in the trunc() function, which truncates floating
%  point numbers towards zero.

%  If the value for P is zero we don't truncate and
%  run in MATLAB's native precision, which is double precision

P = [0 6 8];

%  x contains the x values at which we will do the recursion
%  N determines the value, cos(Nx), we will ultimately compute

x = [ 1e-2 1e-4 1e-5] ;
N = [  1e2 1e3 1e4 1e5];

%  Set format for printing variables in legend

formatSpec = '%7.2e';

for eP = 10.^P
    
    %  We have a new figure for each precision
    
    h = figure('DefaultLineLineWidth',2,'DefaultLineMarkerSize',15, ...
        'DefaultTextFontSize', 18,'DefaultTextFontWeight','bold', ...
        'DefaultAxesFontSize', 16);
    
    lg = {};  %  The string for the legend
    
    for xx = x
        
        %  This is where we store the relative differences between
        %  the recursion value and the actual value
        
        cdiffrel = [];
        
        %  Loop over different number of recursion steps.
        
        for NN = N
            
            %  Notice we wrap every arithmetic operation in the trunc()
            %  function (defined below), to make our arithmetic less
            %  accurate than native double precision.
            
            c = trunc(cos(trunc(xx)));
            
            cc = zeros(1,NN);
            
            cc(1,1) = c;
            cc(1,2) = 2 * trunc(c * c) - 1;
            
            for i=2:NN-1
                cc(1,i+1) = trunc(2 * trunc(c * cc(1,i)) - cc(1,i-1));
            end
            
            cdirect = cos(xx * NN);
            cdiffrel = [ cdiffrel abs(2 * (cc(NN) - cdirect)/ (cc(NN) + cdirect))];
            
        end
        
        loglog(N,cdiffrel);
        lg = vertcat(lg, ['x = ' num2str(xx,formatSpec)]);
        hold all;
        
    end
    
    if ( eP < 1 + eps )
        title( 'Error from cos(Nx) iteration in double precision', 'FontSize', 18 );
       
    else
        title( {'Error from cos(Nx) iteration'; ...
            ['Truncating arithmetic with precision of ' num2str(eP)]}, 'FontSize', 18 );
    end
    
    xlabel('Number of iterations, N','FontSize',18);
    ylabel('Relative error','FontSize',18);
    legend(lg,'Location','NorthWest');
    hold all;
    
    %  Print out a pdf file
    
    print(h, '-dpdf', [ 'cosine_recursion_truncation_precision_' num2str(log10(eP),'%2d') '.pdf' ]);
    
end

%  A function to truncate a floating point number

    function [r] = trunc(y)
        
        
        if ( eP < 1 + eps )
            
            r = y;
            
        else
            
            %  Preserve sign of y
            
            s = sign(y);
            
            %  First figure out how to make the value a number of
            %  order 1.
            
            fly = floor(log10(abs(y)));
            
            %  Make y a positive number of order 1
            
            y = abs(y * 10^(-fly));
            
            %  Now truncate y to P digits of precision
            %  floor() truncates towards zero
            
            y =  (floor(y * eP))/eP;
            
            %  Now multiply by 10 to the correct power
            
            r = s * y * 10^fly;
        end
        
    end


end





