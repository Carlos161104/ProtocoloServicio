# numeros Pseudoaleatorio
#
#

# Una funcion ,odulo necesita una base y una variable
# el modulo 4

x=1;
base=4;
X=[];
for i=1:100
  %x=x+1 # Aritmetcia tradicional
  x=mod(x+1,base); # Aritmetica tradicional
  disp(x);
  X=[X;x];
end

#figure;
#plot(X);

# x=mod(f(x),base); Un anillo modular, anillo residual o  anillo congruencial
# f(x) Es una funcion congruencial...
#
#
# f(x) sea una funcion lineal... a*x+b
# x=mod(a*x+b,base)

clc
a=sigPrimo(20000);
b=sigPrimo(10000);
base=sigPrimo(100000); #Base debe ser impar (primo)
x=fix(time);
X=[];
for i=1:1000

  x = mod(a*x+b,base); # El multiplicador debe de ser impar, mientras mas
                      #diferente de la base es mejor
  X=[X;fix(x/base*256)]; # Normalizamos la secuencia pero llevada solamente a 256
  disp(fix(x/base*256));
end


