function y=esPrimo(x)
   y=1;
   for i=2:sqrt(x)
      if(mod(x,i)==0)
          y=0;
          return;
      end
   end
end
